/////////////////////////////////////////////////////////////////////////////
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

#include "Journey.h"
#ifdef JOURNEY_USE_OPENGL
#include "IO\WindowGLFW.h"
#else
#include "IO\WindowD2D.h"
#endif

namespace Program
{
	class Client
	{
	public:
		// Error codes to be checked after initialisation.
		enum Error
		{
			NONE,
			NXFILES,
			CONNECTION,
			WINDOW,
			AUDIO
		};

		Client();
		~Client();

		// Initialise and return errors.
		Error init();
		// Checks for incoming packets and returns if the connection is still alive.
		bool receive() const;
		// Draws the window and all game objects.
		void draw(float) const;
		// Processes inputs and updates the window and all game objects.
		void update();

	private:

#ifdef JOURNEY_USE_OPENGL
		WindowGLFW window;
#else
		IO::WindowD2D window;
#endif
	};
}