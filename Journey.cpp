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
#include "Program\Client.h"
#include "Program\Constants.h"
#include "Util\StopWatch.h"
#include <iostream>

// Print errors to the console while testing.
void showerror(const char* error)
{
	std::cout << error << std::endl;
}

// Main entry point.
int main()
{
	// Create our game.
	using::Program::Client;
	Client client;

	// Initialise and check for errors.
	Client::Error error = client.init();
	if (error == Client::NONE)
	{
		// No error occured. We can start the main loop.
		Util::StopWatch stopwatch;
		int64_t remain = 0;

		// Run the game as long as the connection is alive.
		while (client.receive())
		{
			remain += stopwatch.stop();
			while (remain >= Constants::TIMESTEP)
			{
				// Update game with constant timestep as many times as possible.
				client.update();
				remain -= Constants::TIMESTEP;
			}

			// Draw the game. Interpolate to account for remaining time.
			float inter = static_cast<float>(remain) / Constants::TIMESTEP;
			client.draw(inter);
		}
	}
	else
	{
		// Display a critical error. These are errors that prevent the game from starting.
		switch (error)
		{
		case Client::NXFILES :
			showerror("Error: Could not find valid game files.");
			break;
		case Client::CONNECTION :
			showerror("Error: Could not connect to server.");
			break;
		case Client::WINDOW:
			showerror("Error: Could not initialize graphics.");
			break;
		case Client::AUDIO:
			showerror("Error: Could not initialize audio.");
			break;
		}

		// Run an infinite loop to keep the console on screen.
		while (error != Client::NONE) {}
	}

	return 0;
}