/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015-2016 Daniel Allendorf                                   //
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

namespace jrc
{
	// Error codes to be checked after initialisation.
	class Error
	{
	public:
		enum Code
		{
			NONE,
			CONNECTION,
			MISSING_FILE,
			WRONG_UI_FILE,
			GLFW,
			GLEW,
			FREETYPE,
			VERTEX_SHADER,
			FRAGMENT_SHADER,
			SHADER_PROGRAM,
			SHADER_VARS,
			WINDOW,
			AUDIO,
			LENGTH
		};

		constexpr Error(Code c)
			: code(c) {}

		constexpr operator bool() const
		{
			return code != NONE;
		}

		constexpr bool can_retry() const
		{
			return code == CONNECTION;
		}

		constexpr char* get_message() const
		{
			return messages[code];
		}

	private:
		Code code;

		static constexpr char* messages[LENGTH] =
		{
			"",
			"The server seems to be offline. Please start the server and enter 'retry'.",
			"Missing a game file.",
			"UI.nx has wrong version.",
			"Could not initialize glfw.",
			"Could not initialize glew.",
			"Could not initialize freetype.",
			"Failed to create vertex shader.",
			"Failed to create fragment shader.",
			"Failed to create shader program.",
			"Failed to locate shader variables.",
			"Failed to create window.",
			"Failed to initialize audio"
		};
	};
}