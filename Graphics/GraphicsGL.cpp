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
#include "GraphicsGL.h"
#ifdef JOURNEY_USE_OPENGL
#include "Console.h"
#include "glm.hpp"
#include <algorithm>

namespace Graphics
{
	bool GraphicsGL::init()
	{
		if (glewInit())
			return false;

		if (FT_Init_FreeType(&ftlibrary))
			return false;

		GLint result = GL_FALSE;

		GLuint vs = glCreateShader(GL_VERTEX_SHADER);
		const char *vs_source =
			"#version 120\n"
			"attribute vec4 coord;"
			"attribute vec4 color;"
			"varying vec2 texpos;"
			"varying vec4 colormod;"
			"uniform vec2 screensize;"

			"void main(void) {"
			"	float x = -1.0 + coord.x * 2.0 / screensize.x;"
			"	float y = 1.0 - (coord.y + 10) * 2.0 / screensize.y;"
			"   gl_Position = vec4(x, y, 0.0, 1.0);"
			"	texpos = coord.zw;"
			"	colormod = color;"
			"}";
		glShaderSource(vs, 1, &vs_source, NULL);
		glCompileShader(vs);
		glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
		if (!result)
			return false;

		GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
		const char *fs_source =
			"#version 120\n"
			"varying vec2 texpos;"
			"varying vec4 colormod;"
			"uniform sampler2D texture;"
			"uniform vec2 atlassize;"
			"uniform int fontregion;"

			"void main(void) {"
			"	if (texpos.y == 0) {"
			"		gl_FragColor = colormod;"
			"	} else if (texpos.y <= fontregion) {"
			"		gl_FragColor = vec4(1, 1, 1, texture2D(texture, texpos / atlassize).r) * colormod;"
			"	} else {"
			"		gl_FragColor = texture2D(texture, texpos / atlassize) * colormod;"
			"	}"
			"}";
		glShaderSource(fs, 1, &fs_source, NULL);
		glCompileShader(fs);
		glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
		if (!result)
			return false;

		program = glCreateProgram();
		glAttachShader(program, vs);
		glAttachShader(program, fs);
		glLinkProgram(program);
		glGetProgramiv(program, GL_LINK_STATUS, &result);
		if (!result)
			return false;

		attribute_coord = glGetAttribLocation(program, "coord");
		attribute_color = glGetAttribLocation(program, "color");
		uniform_texture = glGetUniformLocation(program, "texture");
		uniform_atlassize = glGetUniformLocation(program, "atlassize");
		uniform_screensize = glGetUniformLocation(program, "screensize");
		uniform_fontregion = glGetUniformLocation(program, "fontregion");
		if (attribute_coord == -1 || attribute_color == -1 || uniform_texture == -1 || uniform_atlassize == -1 || uniform_screensize == -1)
			return false;

		glGenBuffers(1, &vbo);

		glGenTextures(1, &atlas); 
		glBindTexture(GL_TEXTURE_2D, atlas);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA, ATLASW, ATLASH, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, nullptr);

		fontborder.sety(1);

		addfont("arial.ttf", Text::A11L, 0, 11);
		addfont("arial.ttf", Text::A11M, 0, 11);
		addfont("arialbd.ttf", Text::A11B, 0, 11);
		addfont("arial.ttf", Text::A12M, 0, 12);
		addfont("arialbd.ttf", Text::A12B, 0, 12);
		addfont("arial.ttf", Text::A13M, 0, 13);
		addfont("arialbd.ttf", Text::A13B, 0, 13);
		addfont("arial.ttf", Text::A18M, 0, 18);

		fontymax += fontborder.y();

		return true;
	}

	bool GraphicsGL::addfont(const char* name, Text::Font id, FT_UInt pixelw, FT_UInt pixelh)
	{
		FT_Face face;
		if (FT_New_Face(ftlibrary, name, 0, &face))
			return false;

		if (FT_Set_Pixel_Sizes(face, pixelw, pixelh))
			return false;

		FT_GlyphSlot g = face->glyph;

		GLshort width = 0;
		GLshort height = 0;
		for (uint8_t c = 32; c < 128; c++)
		{
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
				continue;

			GLshort w = static_cast<GLshort>(g->bitmap.width);
			GLshort h = static_cast<GLshort>(g->bitmap.rows);

			width += w;
			if (h > height)
				height = h;
		}

		if (fontborder.x() + width > ATLASW)
		{
			fontborder.setx(0);
			fontborder.sety(fontymax);
			fontymax = 0;
		}

		GLshort x = fontborder.x();
		GLshort y = fontborder.y();

		fontborder.shiftx(width);
		if (height > fontymax)
			fontymax = height;

		fonts[id] = Font(width, height);

		GLshort ox = x;
		GLshort oy = y;
		for (uint8_t c = 32; c < 128; c++)
		{
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
				continue;

			GLshort ax = static_cast<GLshort>(g->advance.x >> 6);
			GLshort ay = static_cast<GLshort>(g->advance.y >> 6);
			GLshort l = static_cast<GLshort>(g->bitmap_left);
			GLshort t = static_cast<GLshort>(g->bitmap_top);
			GLshort w = static_cast<GLshort>(g->bitmap.width);
			GLshort h = static_cast<GLshort>(g->bitmap.rows);

			glTexSubImage2D(GL_TEXTURE_2D, 0, ox, oy, w, h, 
				GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

			Offset offset = Offset(ox, oy, w, h);
			fonts[id].chars[c] = { ax, ay, w, h, l, t, offset };

			ox += w;
		}

		return true;
	}

	void GraphicsGL::reinit()
	{
		glUseProgram(program);

		glUniform1i(uniform_fontregion, fontymax);
		glUniform2f(uniform_atlassize, ATLASW, ATLASH);
		glUniform2f(uniform_screensize, Constants::VIEWWIDTH, Constants::VIEWHEIGHT);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(attribute_coord, 4, GL_SHORT, GL_FALSE, 24, 0);
		glVertexAttribPointer(attribute_color, 4, GL_FLOAT, GL_FALSE, 24, (const void*)8);

		glBindTexture(GL_TEXTURE_2D, atlas);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		clear();
	}

	void GraphicsGL::clear()
	{
		border = vector2d<GLshort>(0, fontymax);
		yrange = vector2d<GLshort>();

		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, fontborder.y(), 
			ATLASW, ATLASH - fontymax, GL_BGRA, GL_UNSIGNED_BYTE, nullptr);

		offsets.clear();
		leftovers.clear();
		rlid = 1;
		wasted = 0;
	}

	bool GraphicsGL::available(size_t id)
	{
		return offsets.count(id) > 0;
	}

	void GraphicsGL::addbitmap(const bitmap& bmp)
	{
		size_t id = bmp.id();
		if (available(id))
			return;

		GLshort x = 0;
		GLshort y = 0;
		GLshort w = bmp.width();
		GLshort h = bmp.height();

		if (w <= 0 || h <= 0)
			return;

		vector<size_t> expired;
		size_t lid = 0;
		for (auto& leftover : leftovers)
		{
			Leftover& lo = leftover.second;
			if (w <= lo.width() && h <= lo.height())
			{
				lid = leftover.first;
				break;
			}
			else
			{
				lo.trials -= 1;
				if (lo.trials == 0)
					expired.push_back(leftover.first);
			}
		}

		for (auto& exid : expired)
		{
			leftovers.erase(exid);
		}

		if (lid > 0)
		{
			Leftover leftover = leftovers[lid];
			leftovers.erase(lid);

			x = leftover.l;
			y = leftover.t;

			wasted -= w * h;

			GLshort wdelta = leftover.width() - w;
			if (wdelta >= MINLOSIZE)
			{
				leftovers[rlid] = Leftover(x + w, y, wdelta, h);
				rlid++;
			}

			GLshort hdelta = leftover.height() - h;
			if (hdelta >= MINLOSIZE)
			{
				leftovers[rlid] = Leftover(x, y + h, w, hdelta);
				rlid++;
			}

			if (wdelta >= MINLOSIZE && hdelta >= MINLOSIZE)
			{
				leftovers[rlid] = Leftover(x + w, y + h, wdelta, hdelta);
				rlid++;
			}
		}
		else
		{
			if (border.x() + w > ATLASW)
			{
				border.setx(0);
				border.shifty(yrange.y());
				if (border.y() + h > ATLASH)
				{
					clear();
				}
				else
				{
					yrange = vector2d<GLshort>();
				}
			}
			x = border.x();
			y = border.y();

			border.shiftx(w);

			if (h > yrange.y())
			{
				if (x >= MINLOSIZE && h - yrange.y() >= MINLOSIZE)
				{
					leftovers[rlid] = Leftover(0, yrange.x(), x, h - yrange.y());
					rlid++;
				}

				wasted += yrange.x() * (h - yrange.y());

				yrange.setx(y + h);
				yrange.sety(h);
			}
			else if (h < yrange.x() - y)
			{
				if (w >= MINLOSIZE && yrange.x() - y - h >= MINLOSIZE)
				{
					leftovers[rlid] = Leftover(x, y + h, w, yrange.x() - y - h);
					rlid++;
				}

				wasted += w * (yrange.x() - y - h);
			}
		}

		glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_BGRA, GL_UNSIGNED_BYTE, bmp.data());
		offsets[id] = Offset(x, y, w, h);

		/*size_t used = ATLASW * border.y() + border.x() * yrange.y();
		double usedpercent = static_cast<double>(used) / (ATLASW * ATLASH);
		double wastedpercent = static_cast<double>(wasted) / used;
		Console::get().print("Used: " + std::to_string(usedpercent) + ", wasted: " + std::to_string(wastedpercent));*/
	}

	void GraphicsGL::draw(size_t id, int16_t x, int16_t y, int16_t w, int16_t h, float alpha,
		float xscale, float yscale, int16_t centerx, int16_t centery) {

		if (!available(id))
			return;

		GLshort left = centerx + static_cast<GLshort>(xscale * (x - centerx));
		GLshort right = centerx + static_cast<GLshort>(xscale * (x + w - centerx));
		GLshort top = centery + static_cast<GLshort>(yscale * (y - centery));
		GLshort bottom = centery + static_cast<GLshort>(yscale * (y + h - centery));
		Offset offset = offsets[id];

		Quad quad = Quad(left, right, top, bottom, offset, 1.0f, 1.0f, 1.0f, alpha);
		quads.push_back(quad);
	}

	void GraphicsGL::flush()
	{
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);

		GLsizei csize = static_cast<GLsizei>(quads.size() * sizeof(Quad));
		GLsizei fsize = static_cast<GLsizei>(quads.size() * Quad::LENGTH);
		glEnableVertexAttribArray(attribute_coord);
		glEnableVertexAttribArray(attribute_color);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, csize, quads.data(), GL_STREAM_DRAW);
		glDrawArrays(GL_QUADS, 0, fsize);

		glDisableVertexAttribArray(attribute_coord);
		glDisableVertexAttribArray(attribute_color);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		quads.clear();
	}

	Text::Layout GraphicsGL::createlayout(const string& text, Text::Font id, Text::Alignment alignment, int16_t maxwidth)
	{
		Text::Layout layout = Text::Layout(alignment);

		size_t first = 0;
		size_t offset = 0;
		size_t last = text.length();

		const Font& font = fonts[id];

		int16_t totaladvance = 0;
		int16_t ax = 0;
		int16_t ay = font.linespace();
		while (offset < last)
		{
			size_t space = text.find(' ', offset + 1);
			if (space == string::npos)
				space = last;

			int16_t width = font.wordwidth(text.c_str(), offset, space);
			if (ax + width > maxwidth)
			{
				layout.addline(ax, ay, first, offset);
				first = offset;

				ax = 0;
				ay += font.linespace();
			}

			for (size_t pos = offset; pos < space; pos++)
			{
				char c = text[pos];
				const Font::Char& ch = font.chars[c];

				layout.advances[pos] = ax;

				if (ax == 0 && c == ' ')
					continue;

				ax += ch.ax;
				totaladvance += ch.ax;
			}

			offset = space;
		}
		layout.addline(ax, ay, first, offset);

		layout.advances[text.length()] = totaladvance;
		layout.dimensions = vector2d<int16_t>(std::min(totaladvance, maxwidth), layout.linecount * font.linespace());
		layout.endoffset = vector2d<int16_t>(ax % maxwidth, (layout.linecount - 1) * font.linespace());

		return layout;
	}

	void GraphicsGL::drawtext(const string& text, Text::Font id, Text::Alignment alignment, Text::Color colorid,
		Text::Background background, const Text::Layout& layout, float opacity, vector2d<int16_t> origin) {

		const Font& font = fonts[id];

		GLshort x = origin.x();
		GLshort y = origin.y();

		switch (background)
		{
		case Text::NAMETAG:
			for (auto& line : layout.lines)
			{
				GLshort left = x + line.position.x() - 2;
				GLshort right = left + layout.dimensions.x() + 4;
				GLshort top = y + line.position.y() - font.linespace() + 5;
				GLshort bottom = top + layout.dimensions.y() - 1;

				Quad quad = Quad(left, right, top, bottom, nulloffset, 0.0, 0.0, 0.0, 0.6);
				quads.push_back(quad);
			}
			break;
		}

		static const GLfloat colors[Text::NUM_COLORS][3] =
		{
			{ 0.0f, 0.0f, 0.0f }, // Black
			{ 1.0f, 1.0f, 1.0f }, // White
			{ 1.0f, 1.0f, 0.0f }, // Yellow
			{ 0.0f, 0.0f, 1.0f }, // Blue
			{ 1.0f, 0.0f, 0.0f }, // Red
			{ 0.5f, 0.25f, 0.0f }, // Brown
			{ 0.8f, 0.8f, 0.8f }, // Lightgrey
			{ 0.5f, 0.5f, 0.5f }, // Darkgrey
			{ 1.0f, 0.5f, 0.0f }, // Orange
			{ 0.0f, 0.75f, 1.0f }, // Mediumblue
			{ 0.5f, 0.0f, 0.5f } // Violet
		};
		if (opacity < 0.0f)
			opacity = 0.0f;
		const GLfloat* color = colors[colorid];

		for (auto& line : layout.lines)
		{
			GLshort ax = line.position.x();
			GLshort ay = line.position.y();

			for (size_t pos = line.first; pos < line.last; pos++)
			{
				const char c = text[pos];
				const Font::Char& ch = font.chars[c];

				GLshort chx = x + ax + ch.bl;
				GLshort chy = y + ay - ch.bt;
				GLshort chw = ch.bw;
				GLshort chh = ch.bh;

				if (ax == 0 && c == ' ')
					continue;

				ax += ch.ax;

				if (chw <= 0 || chh <= 0)
					continue;

				Quad quad = Quad(chx, chx + chw, chy, chy + chh, ch.offset, color[0], color[1], color[2], opacity);
				quads.push_back(quad);
			}
		}
	}
}
#endif
