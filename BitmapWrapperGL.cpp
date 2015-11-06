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
#include "BitmapWrapperGL.h"

namespace Graphics
{
	BitmapWrapperGL::BitmapWrapperGL(bitmap bmp)
	{
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA,
			bmp.width(), bmp.height(), 0,
			GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, bmp.data());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	BitmapWrapperGL::~BitmapWrapperGL()
	{
		glDeleteTextures(1, &id);
	}

	void BitmapWrapperGL::draw(rectangle2d<int32_t> rect, float xscale, float yscale, vector2d<int32_t> center, float alpha) const
	{
		GLfloat left = center.x() + xscale * (rect.l() - center.x());
		GLfloat right = center.x() + xscale * (rect.r() - center.x());
		GLfloat top = center.y() + yscale * (rect.t() - center.y());
		GLfloat bottom = center.y() + yscale * (rect.b() - center.y());

		glBindTexture(GL_TEXTURE_2D, id);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(left, top, 0);
		glTexCoord2f(0, 1); glVertex3f(left, bottom, 0);
		glTexCoord2f(1, 1); glVertex3f(right, bottom, 0);
		glTexCoord2f(1, 0); glVertex3f(right, top, 0);
		glEnd();
	}
}
#endif