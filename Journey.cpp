//////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015 Daniel Allendorf                                        //
//                                                                          //
// This program is free software: you can redistribute it and/or modify     //
// it under the terms of the GNU Affero General Public License as           //
// published by the Free Software Foundation, either version 3 of the       //
// License, or (at your option) any later version.                          //
//                                                                          //
// This program is distributed in the hope that it will be useful,          //
// but WITHOUT ANY WARRANTY; without even the implied warranty of           //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
// GNU Affero General Public License for more details.                      //
//                                                                          //
// You should have received a copy of the GNU Affero General Public License //
// along with this program.  If not, see <http://www.gnu.org/licenses/>.    //
//////////////////////////////////////////////////////////////////////////////
#include "Timer.h"
#include "Configuration.h"
#include "Constants.h"

#include "Audio\Audio.h"
#include "Character\Char.h"
#include "Data\DataFactory.h"
#include "Gameplay\Stage.h"
#include "IO\Components\DamageNumber.h"
#include "IO\UI.h"
#include "IO\Window.h"
#include "Net\Session.h"
#include "Util\NxFiles.h"

#include <iostream>

using Audio::Music;
using Audio::Sound;
using Character::Char;
using Data::DataFactory;
using Gameplay::Stage;
using Gameplay::MapPortals;
using Gameplay::MapDrops;
using Gameplay::Skill;
using IO::DamageNumber;
using IO::UI;
using IO::Window;
using Net::Session;
using Util::NxFiles;

// Print errors to the console while testing.
void showerror(const char* error)
{
	std::cout << error << std::endl;
}

// Error codes to be checked after initialisation.
enum Error
{
	NONE,
	NXFILES,
	CONNECTION,
	WINDOW,
	AUDIO
};

Error init()
{
	Configuration::get().load();

	if (!NxFiles::get().init())
		return NXFILES;

	if (!Session::get().init())
		return CONNECTION;

	bool fullscreen = Configuration::get().getbool(Settings::FULLSCREEN);
	if (!Window::get().init(fullscreen))
		return WINDOW;

	uint8_t sfxvolume = Configuration::get().getbyte(Settings::SFX_VOLUME);
	uint8_t bgmvolume = Configuration::get().getbyte(Settings::BGM_VOLUME);
	if (!Sound::init(sfxvolume) || !Music::init(bgmvolume))
		return AUDIO;

	Char::init();
	DamageNumber::init();
	MapPortals::init();
	MapDrops::init();

	DataFactory::get().init();
	UI::get().init();

	return NONE;
}

void update()
{
	Window::get().update();
	Stage::get().update();
	UI::get().update();
}

void draw(float inter)
{
	Window::get().begin();
	Stage::get().draw(inter);
	UI::get().draw(inter);
	Window::get().end();
}

int main()
{
	// Initialise and check for errors.
	Error error = init();
	if (error == NONE)
	{
		Timer::get().start();
		double elapsed = 0.0;
		//double invfps = 0.0;
		//int32_t samples = 0;

		// Run the game as long as the connection is alive.
		while (Session::get().receive())
		{
			// Update game with constant timestep as many times as possible.
			double lastelapsed = Timer::get().stop() / 1000;

			/*if (samples < 1000)
			{
				invfps += lastelapsed;
				samples++;
			}
			else
			{
				int32_t fps = static_cast<int32_t>((samples * 1000) / invfps);
				std::cout << "FPS: " << std::to_string(fps) << std::endl;
					
				invfps = 0.0;
				samples = 0;
			}*/

			elapsed += lastelapsed;
			while (elapsed >= Constants::TIMESTEP)
			{
				update();
				elapsed -= Constants::TIMESTEP;
			}

			// Draw the game. Interpolate to account for remaining time.
			float inter = static_cast<float>(std::floor(elapsed) / Constants::TIMESTEP);
			draw(inter);
		}

		Sound::close();
		Configuration::get().save();
	}
	else
	{
		// Display a critical error. These are errors that prevent the game from starting.
		switch (error)
		{
		case NXFILES :
			showerror("Error: Could not find valid game files.");
			break;
		case CONNECTION :
			showerror("Error: The server seems to be offline.");
			break;
		case WINDOW:
			showerror("Error: Could not initialize graphics.");
			break;
		case AUDIO:
			showerror("Error: Could not initialize audio.");
			break;
		}

		// Run an infinite loop to keep the console on screen.
		while (error != NONE) {}
	}

	return 0;
}