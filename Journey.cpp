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
#include "Audio\AudioPlayer.h"
#include "Net\Session.h"
#include "IO\UI.h"
#include "IO\Window.h"
#include "Gameplay\Stage.h"
#include "Util\NxFiles.h"
#include "Data\DataFactory.h"
#include <iostream>

using Audio::AudioPlayer;
using Data::DataFactory;
using Gameplay::Stage;
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
	auto& config = Configuration::get();
	config.load();

	if (!NxFiles::get().init())
		return NXFILES;

	if (!Session::get().init())
		return CONNECTION;

	bool fullscreen = config.getbool(Settings::FULLSCREEN);
	if (!Window::get().init(fullscreen))
		return WINDOW;

	uint8_t sfxvolume = config.getbyte(Settings::SFX_VOLUME);
	uint8_t bgmvolume = config.getbyte(Settings::BGM_VOLUME);
	if (!AudioPlayer::get().init(sfxvolume, bgmvolume))
		return AUDIO;

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
		double elapsed = 0;

		// Run the game as long as the connection is alive.
		while (Session::get().receive())
		{
			// Update game with constant timestep as many times as possible.
			double elapsedmicro = Timer::get().stop();
			elapsed += elapsedmicro / 1000;
			while (std::floor(elapsed) >= Constants::TIMESTEP)
			{
				update();
				elapsed -= Constants::TIMESTEP;
			}

			// Draw the game. Interpolate to account for remaining time.
			float inter = static_cast<float>(std::floor(elapsed) / Constants::TIMESTEP);
			draw(inter);
		}

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