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
#include "Util\Singleton.h"
#include "GL\glew.h"
#include "glfw3.h"
#include <string>
#include <functional>

namespace IO
{
	using std::string;
	using std::function;

	class Window : public Singleton<Window>
	{
	public:
		Window();
		~Window();

		bool init();
		bool initwindow();
		bool notclosed() const;
		void update();
		void begin() const;
		void end() const;
		void fadeout(float step, function<void()> fadeprocedure);

		void setclipboard(string text) const;
		string getclipboard() const;

	private:
		void updateopc();

		GLFWwindow* glwnd;
		GLFWwindow* context;
		bool fullscreen;
		float opacity;
		float opcstep;
		function<void()> fadeprocedure;
	};
}

