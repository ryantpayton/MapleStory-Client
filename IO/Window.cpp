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
#include "Window.h"
#include "UI.h"

#include "..\Console.h"
#include "..\Constants.h"
#include "..\Timer.h"
#include "..\Configuration.h"
#include "..\Graphics\GraphicsGL.h"
#include "..\Gameplay\Stage.h"

#include <iostream>

namespace jrc
{
	Window::Window()
	{
		context = nullptr;
		glwnd = nullptr;
		opacity = 1.0f;
		opcstep = 0.0f;
	}

	Window::~Window()
	{
		glfwTerminate();
	}

	void error_callback(int error, const char* description)
	{
		std::cout << "Error no.:" << std::to_string(error) << " : " << description << std::endl;
	}

	void key_callback(GLFWwindow*, int key, int, int action, int)
	{
		UI::get().sendkey(key, action != GLFW_RELEASE);
	}

	void mousekey_callback(GLFWwindow*, int button, int action, int)
	{
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			switch (action)
			{
			case GLFW_PRESS:
				UI::get().sendmouse(true);
				break;
			case GLFW_RELEASE:
				UI::get().sendmouse(false);
				break;
			}
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			switch (action)
			{
			case GLFW_PRESS:
				UI::get().doubleclick();
				break;
			}
			break;
		}
	}

	void cursor_callback(GLFWwindow*, double xpos, double ypos)
	{
		int16_t x = static_cast<int16_t>(xpos);
		int16_t y = static_cast<int16_t>(ypos);
		Point<int16_t> pos = Point<int16_t>(x, y);
		UI::get().sendmouse(pos);
	}

	Error Window::init()
	{
		fullscreen = Setting<Fullscreen>::get().load();

		if (!glfwInit())
			return Error::GLFW;

		glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
		context = glfwCreateWindow(1, 1, "", nullptr, nullptr);
		glfwMakeContextCurrent(context);
		glfwSetErrorCallback(error_callback);
		glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		if (Error error = GraphicsGL::get().init())
			return error;

		return initwindow();
	}

	Error Window::initwindow()
	{
		if (glwnd)
			glfwDestroyWindow(glwnd);

		glwnd = glfwCreateWindow(
			Constants::VIEWWIDTH,
			Constants::VIEWHEIGHT,
			"Journey",
			fullscreen ? glfwGetPrimaryMonitor() : nullptr,
			context
			);

		if (!glwnd)
			return Error::WINDOW;

		glfwMakeContextCurrent(glwnd);

		bool vsync = Setting<VSync>::get().load();
		glfwSwapInterval(vsync ? 1 : 0);

		glViewport(0, 0, Constants::VIEWWIDTH, Constants::VIEWHEIGHT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glfwSetInputMode(glwnd, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		glfwSetInputMode(glwnd, GLFW_STICKY_KEYS, 1);
		glfwSetKeyCallback(glwnd, key_callback);
		glfwSetMouseButtonCallback(glwnd, mousekey_callback);
		glfwSetCursorPosCallback(glwnd, cursor_callback);

		GraphicsGL::get().reinit();

		return Error::NONE;
	}

	bool Window::notclosed() const
	{
		return glfwWindowShouldClose(glwnd) == 0;
	}

	void Window::update()
	{
		updateopc();
	}

	void Window::updateopc()
	{
		if (opcstep != 0.0f)
		{
			opacity += opcstep;

			if (opacity >= 1.0f)
			{
				opacity = 1.0f;
				opcstep = 0.0f;
			}
			else if (opacity <= 0.0f)
			{
				opacity = 0.0f;
				opcstep = -opcstep;

				fadeprocedure();
			}
		}
	}

	void Window::checkevents()
	{
		int32_t tabstate = glfwGetKey(glwnd, GLFW_KEY_F11);
		if (tabstate == GLFW_PRESS)
		{
			fullscreen = !fullscreen;
			initwindow();
		}
		glfwPollEvents();
	}

	void Window::begin() const
	{
		GraphicsGL::get().clearscene();
	}

	void Window::end() const
	{
		GraphicsGL::get().flush(opacity);
		glfwSwapBuffers(glwnd);
	}

	void Window::fadeout(float step, std::function<void()> fadeproc)
	{
		opcstep = -step;
		fadeprocedure = fadeproc;
	}

	void Window::setclipboard(const std::string& text) const
	{
		glfwSetClipboardString(glwnd, text.c_str());
	}

	std::string Window::getclipboard() const
	{
		const char* text = glfwGetClipboardString(glwnd);
		return text ? text : "";
	}
}