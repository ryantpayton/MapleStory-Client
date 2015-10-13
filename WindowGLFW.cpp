/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015 SYJourney                                               //
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
/*#include "WindowGLFW.h"
#include "nx.hpp"
#include "node.hpp"
#include "bitmap.hpp"

namespace Program
{
	WindowGLFW::WindowGLFW()
	{
		glwnd = 0;
	}

	WindowGLFW::~WindowGLFW()
	{
		glfwTerminate();
	}

	bool WindowGLFW::init()
	{
		if (glfwInit())
		{
			glfwWindowHint(GLFW_SAMPLES, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glwnd = glfwCreateWindow(800, 600, "Journey", 0, 0);
			if (glwnd)
			{
				glfwMakeContextCurrent(glwnd);
				glfwSetInputMode(glwnd, GLFW_STICKY_KEYS, GL_TRUE);
				glfwSwapInterval(1);
				glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
			}
		}
		nl::nx::load_all();
		return glwnd != 0;
	}

	void WindowGLFW::draw()
	{
		//glViewport(0, 0, 800, 600);
		nl::bitmap bmp = nl::nx::ui["Login.img"]["Title"]["11"];
		//update();
		//glBegin(GL_TEXTURE_2D);
		/*glColor3f(1.0, 0.0, 0.0);   glVertex3i(50, 200, 0);
		glColor3f(0.0, 1.0, 0.0);   glVertex3i(250, 200, 0);
		glColor3f(0.0, 0.0, 1.0);   glVertex3i(250, 350, 0);
		glColor3f(1.0, 1.0, 1.0);   glVertex3i(50, 350, 0);*/
		//glDrawPixels(bmp.width(), bmp.height(), GL_RGBA, GL_UNSIGNED_INT, bmp.data());
		//glEnd();
		/*glEnable(GL_TEXTURE_2D);

		GLuint texName;
		glGenTextures(1, &texName);
		glBindTexture(GL_TEXTURE_2D, texName);

		glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);
		glPixelStorei(GL_UNPACK_LSB_FIRST, GL_FALSE);
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
		glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, 0);
		glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
		glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
		glPixelStorei(GL_UNPACK_SKIP_IMAGES, 0);



		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
			bmp.width(), bmp.height(), 0,
			GL_BGRA, GL_UNSIGNED_INT_8_8_8_8, bmp.data());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		//glBindTexture(GL_TEXTURE_2D, texName);
		glBegin(GL_QUADS);

		glTexCoord2i(0, 600);

		glVertex2i(0, 0);

		glTexCoord2i(800, 600);

		glVertex2i(800, 0);

		glTexCoord2i(800, 0);

		glVertex2i(800, 600);

		glTexCoord2i(0, 0);

		glVertex2i(0, 600);
		/*glBegin(GL_TEXTURE_2D);
		glColor4f(1, 1, 1, 1);
		glTexCoord2i(0, 0);
		glVertex2i(0, 0);
		glTexCoord2i(1, 0);
		glVertex2i(800, 0);
		glTexCoord2i(1, 1);
		glVertex2i(800, 600);
		glTexCoord2i(0, 1);
		glVertex2i(0, 600);
		//glDrawArrays(GL_TEXTURE_2D, 0, 1);
		//glDisable(GL_TEXTURE_2D);

		//glDrawPixels(bmp.width(), bmp.height(), GL_BGRA, GL_UNSIGNED_BYTE, bmp.data());
		// Sets the size of the OpenGL viewport
		glViewport(0, 0, 800, 600);

		// Select the projection stack and apply
		// an orthographic projection
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0, 800, 600, 0.0, -1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);

		glClear(GL_COLOR_BUFFER_BIT);

		glBegin(GL_QUADS);
		glColor3f(1.0, 0.0, 0.0);   glVertex3i(50, 200, 0);
		glColor3f(0.0, 1.0, 0.0);   glVertex3i(250, 200, 0);
		glColor3f(0.0, 0.0, 1.0);   glVertex3i(250, 350, 0);
		glColor3f(1.0, 1.0, 1.0);   glVertex3i(50, 350, 0);
		glEnd();

		glBegin(GL_TRIANGLES);
		glColor3f(1.0, 0.0, 0.0);  glVertex3i(400, 350, 0);
		glColor3f(0.0, 1.0, 0.0);  glVertex3i(500, 200, 0);
		glColor3f(0.0, 0.0, 1.0);  glVertex3i(600, 350, 0);
		glEnd();

		glfwSwapBuffers(glwnd);
		glfwPollEvents();
	}

	void WindowGLFW::update()
	{
		
	}

	void WindowGLFW::fadeout()
	{

	}

	void WindowGLFW::togglemode()
	{

	}
}*/
