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
#include "TextWrapperGL.h"
#include "LocatorGL.h"

namespace IO
{
	TextWrapperGL::TextWrapperGL(Font f, Textcolor c, string s, uint16_t m)
	{
		font = f;
		color = c;
		settext(s, m);

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		/*GLuint tex;
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glUniform1i(uniform_tex, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		GLuint vbo;
		glGenBuffers(1, &vbo);
		glEnableVertexAttribArray(attribute_coord);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);*/
	}

	TextWrapperGL::TextWrapperGL() {}

	void TextWrapperGL::settext(string s, uint16_t maxw)
	{
		if (str != s)
		{
			str = s;
		}
	}

	void TextWrapperGL::setfont(Font f)
	{
		font = f;
	}

	void TextWrapperGL::setcolor(Textcolor c)
	{
		color = c;
	}

	void TextWrapperGL::setback(TextBackground b)
	{
		back = b;
	}

	void TextWrapperGL::setalpha(float a)
	{
		alpha = a;
	}

	void TextWrapperGL::draw(vector2d<int32_t> pos) const
	{
		const FontsFT* fonts = LocatorGL::getfonts();
		if (false)
		{
			const FT_Face* fontface = fonts->getfont(font);
			if (*fontface)
			{
				float x = static_cast<float>(pos.x());
				float y = static_cast<float>(pos.y());

				//glBlendFunc(GL_SRC_ALPHA, GL_ZERO);

				for (size_t i = 0; i < str.size(); i++)
				{
					int8_t ch = str[i];
					FT_GlyphSlot g = (*fontface)->glyph; 
					
					FT_UInt  glyph_index = FT_Get_Char_Index(*fontface, ch);

					if (FT_Load_Glyph(*fontface, glyph_index, FT_LOAD_DEFAULT) == FT_Err_Ok)
					{
						if (FT_Render_Glyph(g, FT_RENDER_MODE_NORMAL) == FT_Err_Ok)
						{
							glBindTexture(GL_TEXTURE_2D, texture);
							glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA, g->bitmap.width, g->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

							x += g->advance.x >> 6;
						}
					}
				}

				/*glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				GLfloat left = pos.x();
				GLfloat right = pos.x() + x;
				GLfloat top = y;
				GLfloat bottom = y + 14;

				glBegin(GL_QUADS);
				glTexCoord2f(0, 0); glVertex3f(left, top, 0);
				glTexCoord2f(0, 1); glVertex3f(left, bottom, 0);
				glTexCoord2f(1, 1); glVertex3f(right, bottom, 0);
				glTexCoord2f(1, 0); glVertex3f(right, top, 0);
				glEnd();*/
			}
		}
	}

	uint16_t TextWrapperGL::getadvance(size_t pos) const
	{
		return 0;
	}

	size_t TextWrapperGL::getlength() const
	{
		return str.length();
	}

	string TextWrapperGL::gettext() const
	{
		return str;
	}

	vector2d<uint16_t> TextWrapperGL::getdimensions() const
	{
		return vector2d<uint16_t>();
	}
}
#endif