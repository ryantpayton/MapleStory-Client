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
#pragma once
#include "Program\Configuration.h"
#include "Program\Constants.h"
#include "Audio\Audioplayer.h"
#include "Net\Session.h"
#include "IO\UI.h"
#include "IO\Window.h"
#include "IO\UITypes\UILogin.h"
#include "Gameplay\Stage.h"
#include "Util\NxFileMethods.h"
#include "Util\StopWatch.h"
#include "Data\DataFactory.h"
#include "Character\BuffEffects.h"
#include <iostream>

using namespace Program;
using namespace Net;
using namespace IO;
using namespace Gameplay;
using namespace Util;

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
	if (!NxFileMethods::init())
		return NXFILES;

	if (!Session::init())
		return CONNECTION;

	if (!Window::init())
		return WINDOW;

	if (!Audioplayer::init())
		return AUDIO;

	return NONE;
}

void update()
{
	Window::update();
	Stage::update();
	UI::update();
}

void draw(float inter)
{
	Window::begin();
	Stage::draw(inter);
	UI::draw(inter);
	Window::end();
}

int main()
{
	Configuration::load();

	// Initialise and check for errors.
	Error error = init();
	if (error == NONE)
	{
		Data::init();
		Character::initbuffeffects();

		UI::add(ElementLogin());

		StopWatch stopwatch;
		int64_t remain = 0;

		// Run the game as long as the connection is alive.
		while (Session::receive())
		{
			remain += stopwatch.stop();
			while (remain >= Constants::TIMESTEP)
			{
				// Update game with constant timestep as many times as possible.
				update();
				remain -= Constants::TIMESTEP;
			}

			// Draw the game. Interpolate to account for remaining time.
			float inter = static_cast<float>(remain) / Constants::TIMESTEP;
			draw(inter);
		}

		Audioplayer::close();
		Configuration::save();
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
			showerror("Error: Could not connect to server.");
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