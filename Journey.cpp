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
#include "Gameplay\Combat\DamageNumber.h"
#include "Gameplay\Stage.h"
#include "IO\UI.h"
#include "IO\Window.h"
#include "Net\Session.h"
#include "Util\NxFiles.h"

#include <iostream>

using namespace jrc;

// Print errors to the console while testing.
void showerror(const char* error)
{
	std::cout << error << std::endl;
}

// Error codes to be checked after initialisation.
enum Error
{
	NONE,
	CONNECTION,
	NXFILES,
	WINDOW,
	AUDIO
};

Error init()
{
	if (!Session::get().init())
		return CONNECTION;

	if (!NxFiles::get().init())
		return NXFILES;

	if (!Window::get().init())
		return WINDOW;

	if (!Sound::init() || !Music::init())
		return AUDIO;

	Char::init();
	DataFactory::get().init();
	DamageNumber::init();
	MapPortals::init();
	MapDrops::init();
	UI::get().init();

	return NONE;
}

void update()
{
	Window::get().checkevents();
	Window::get().update();
	Stage::get().update();
	UI::get().update();
}

void draw(float alpha)
{
	Window::get().begin();
	Stage::get().draw(alpha);
	UI::get().draw(alpha);
	Window::get().end();
}

void loop()
{
	Timer::get().start();
	int64_t timestep = Constants::TIMESTEP * 1000;
	int64_t elapsed = 0;
	int64_t total = 0;
	int32_t samples = 0;

	// Run the game as long as the connection is alive.
	while (Session::get().receive() && Window::get().notclosed())
	{
		int64_t lastelapsed = Timer::get().stop();
		elapsed += lastelapsed;

		// Update game with constant timestep as many times as possible.
		while (elapsed >= timestep)
		{
			update();
			elapsed -= timestep;
		}

		// Draw the game. Interpolate to account for remaining time.
		float alpha = static_cast<float>(elapsed) / timestep;
		draw(alpha);

		if (samples < 1000)
		{
			total += lastelapsed;
			samples++;
		}
		else
		{
			int64_t fps = (samples * 1000000) / total;
			std::cout << "FPS: " << std::to_string(fps) << std::endl;

			total = 0;
			samples = 0;
		}
	}
}

int start()
{
	// Initialise and check for errors.
	Error error = init();
	if (error == NONE)
	{
		loop();

		Sound::close();
	}
	else
	{
		// Display a critical error. These are errors that prevent the game from starting.
		bool canretry = false;
		switch (error)
		{
		case CONNECTION:
			showerror("Error: The server seems to be offline. Please start the server and enter 'retry'.");
			canretry = true;
			break;
		case NXFILES:
			showerror("Error: Could not find valid game files.");
			break;
		case WINDOW:
			showerror("Error: Could not initialize graphics.");
			break;
		case AUDIO:
			showerror("Error: Could not initialize audio.");
			break;
		}

		std::string command;
		std::cin >> command;
		if (canretry && command == "retry")
		{
			return start();
		}
	}
	return error;
}

int main()
{
	return start();
}