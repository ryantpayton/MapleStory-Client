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
#include "Console.h"
#include "glm.hpp"
#include <algorithm>

namespace Graphics
{
	GraphicsGL::GraphicsGL()
	{
		locked = false;
	}

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
			"uniform int yoffset;"

			"void main(void) {"
			"	float x = -1.0 + coord.x * 2.0 / screensize.x;"
			"	float y = 1.0 - (coord.y + yoffset) * 2.0 / screensize.y;"
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
		uniform_yoffset = glGetUniformLocation(program, "yoffset");
		uniform_fontregion = glGetUniformLocation(program, "fontregion");
		if (attribute_coord == -1 || attribute_color == -1 || uniform_texture == -1 
			|| uniform_atlassize == -1 || uniform_yoffset == -1 || uniform_screensize == -1)
			return false;

		glGenBuffers(1, &vbo);

		glGenTextures(1, &atlas); 
		glBindTexture(GL_TEXTURE_2D, atlas);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ATLASW, ATLASH, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

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

		leftovers = QuadTree<size_t, Leftover>([](const Leftover& first, const Leftover& second){
			bool wcomp = first.width() >= second.width();
			bool hcomp = first.height() >= second.height();
			if (wcomp && hcomp)
			{
				return QuadTree<size_t, Leftover>::RIGHT;
			}
			else if (wcomp)
			{
				return QuadTree<size_t, Leftover>::DOWN;
			}
			else if (hcomp)
			{
				return QuadTree<size_t, Leftover>::UP;
			}
			else
			{
				return QuadTree<size_t, Leftover>::LEFT;
			}
		});

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

		glUniform1i(uniform_yoffset, Constants::VIEWYOFFSET);
		glUniform1i(uniform_fontregion, fontymax);
		glUniform2f(uniform_atlassize, ATLASW, ATLASH);
		glUniform2f(uniform_screensize, Constants::VIEWWIDTH, Constants::VIEWHEIGHT);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(attribute_coord, 4, GL_SHORT, GL_FALSE, 24, 0);
		glVertexAttribPointer(attribute_color, 4, GL_FLOAT, GL_FALSE, 24, (const void*)8);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindTexture(GL_TEXTURE_2D, atlas);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		clearinternal();
	}

	void GraphicsGL::clearinternal()
	{
		border = Point<GLshort>(0, fontymax);
		yrange = Range<GLshort>();

		offsets.clear();
		leftovers.clear();
		rlid = 1;
		wasted = 0;
	}

	void GraphicsGL::clear()
	{
		size_t used = ATLASW * border.y() + border.x() * yrange.second();
		double usedpercent = static_cast<double>(used) / (ATLASW * ATLASH);
		if (usedpercent > 80.0)
		{
			clearinternal();
		}
	}

	const GraphicsGL::Offset& GraphicsGL::getoffset(const bitmap& bmp)
	{
		size_t id = bmp.id();
		auto offiter = offsets.find(id);
		if (offiter != offsets.end())
			return offiter->second;

		GLshort x = 0;
		GLshort y = 0;
		GLshort w = bmp.width();
		GLshort h = bmp.height();

		if (w <= 0 || h <= 0)
			return nulloffset;

		auto value = Leftover(x, y, w, h);
		size_t lid = leftovers.findnode(value, [](const Leftover& val, const Leftover& leaf){
			return val.width() <= leaf.width() && val.height() <= leaf.height();
		});

		if (lid > 0)
		{
			const Leftover& leftover = leftovers[lid];

			x = leftover.l;
			y = leftover.t;

			GLshort wdelta = leftover.width() - w;
			GLshort hdelta = leftover.height() - h;

			leftovers.erase(lid);

			if (wasted < w * h)
			{
				wasted = wasted;
			}
			wasted -= w * h;

			if (wdelta >= MINLOSIZE && hdelta >= MINLOSIZE)
			{
				leftovers.add(rlid, Leftover(x + w, y + h, wdelta, hdelta));
				rlid++;

				if (w >= MINLOSIZE)
				{
					leftovers.add(rlid, Leftover(x, y + h, w, hdelta));
					rlid++;
				}

				if (h >= MINLOSIZE)
				{
					leftovers.add(rlid, Leftover(x + w, y, wdelta, h));
					rlid++;
				}
			}
			else if (wdelta >= MINLOSIZE)
			{
				leftovers.add(rlid, Leftover(x + w, y, wdelta, h + hdelta));
				rlid++;
			}
			else if (hdelta >= MINLOSIZE)
			{
				leftovers.add(rlid, Leftover(x, y + h, w + wdelta, hdelta));
				rlid++;
			}
		}
		else
		{
			if (border.x() + w > ATLASW)
			{
				border.setx(0);
				border.shifty(yrange.second());
				if (border.y() + h > ATLASH)
				{
					clearinternal();
				}
				else
				{
					yrange = Range<GLshort>();
				}
			}
			x = border.x();
			y = border.y();

			border.shiftx(w);

			if (h > yrange.second())
			{
				if (x >= MINLOSIZE && h - yrange.second() >= MINLOSIZE)
				{
					leftovers.add(rlid, Leftover(0, yrange.first(), x, h - yrange.second()));
					rlid++;
				}

				wasted += x * (h - yrange.second());

				yrange.setfirst(y + h);
				yrange.setsecond(h);
			}
			else if (h < yrange.first() - y)
			{
				if (w >= MINLOSIZE && yrange.first() - y - h >= MINLOSIZE)
				{
					leftovers.add(rlid, Leftover(x, y + h, w, yrange.first() - y - h));
					rlid++;
				}

				wasted += w * (yrange.first() - y - h);
			}
		}

		/*size_t used = ATLASW * border.y() + border.x() * yrange.second();
		double usedpercent = static_cast<double>(used) / (ATLASW * ATLASH);
		double wastedpercent = static_cast<double>(wasted) / used;
		Console::get().print("Used: " + std::to_string(usedpercent) + ", wasted: " + std::to_string(wastedpercent));*/

		glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_BGRA, GL_UNSIGNED_BYTE, bmp.data());
		offsets[id] = Offset(x, y, w, h);
		return offsets[id];
	}

	void GraphicsGL::draw(const bitmap& bmp, int16_t x, int16_t y, int16_t w, int16_t h, float alpha,
		float xscale, float yscale, int16_t centerx, int16_t centery) {

		if (locked)
			return;

		GLshort left = centerx + static_cast<GLshort>(xscale * (x - centerx));
		GLshort right = centerx + static_cast<GLshort>(xscale * (x + w - centerx));
		GLshort top = centery + static_cast<GLshort>(yscale * (y - centery));
		GLshort bottom = centery + static_cast<GLshort>(yscale * (y + h - centery));

		GLshort tl = left < right ? left : right;
		GLshort tr = left > right ? left : right;
		GLshort tt = (top < bottom ? top : bottom) + Constants::VIEWYOFFSET;
		GLshort tb = (top > bottom ? top : bottom) + Constants::VIEWYOFFSET;
		if (tr > 0 && tl < Constants::VIEWWIDTH && tb > 0 && tt < Constants::VIEWHEIGHT)
		{
			const Offset& offset = getoffset(bmp);
			Quad quad = Quad(left, right, top, bottom, offset, 1.0f, 1.0f, 1.0f, alpha);
			quads.push_back(quad);
		}
	}

	Text::Layout GraphicsGL::createlayout(const string& text, Text::Font id, Text::Alignment alignment, int16_t maxwidth)
	{
		auto builder = LayoutBuilder(fonts[id], alignment, maxwidth);

		const char* p_text = text.c_str();
		size_t last = text.length();
		if (last == 0)
			return Text::Layout();

		size_t first = 0;
		size_t offset = 0;
		while (offset < last)
		{
			size_t space = text.find(' ', offset + 1);
			if (space == string::npos)
				space = last;

			first = builder.addword(p_text, first, offset, space);
			offset = space;
		}

		return builder.finish(first, offset, last);
	}


	GraphicsGL::LayoutBuilder::LayoutBuilder(const Font& f, Text::Alignment a, int16_t mw)
		: font(f), layout(a), maxwidth(mw) {

		advance = 0;
		ax = 0;
		ay = font.linespace();
	}

	size_t GraphicsGL::LayoutBuilder::addword(const char* text, size_t prev, size_t first, size_t last)
	{
		if (first == last)
			return prev;

		int16_t width = 0;
		for (size_t i = first; i < last; i++)
		{
			char c = text[i];
			width += font.chars[c].ax;

			if (width > maxwidth)
			{
				if (last - first == 1)
				{
					return last;
				}
				else
				{
					prev = addword(text, prev, first, i);
					return addword(text, prev, i, last);
				}
			}
		}
		
		if (ax + width > maxwidth)
		{
			layout.addline(ax, ay, prev, first);
			ax = 0;
			ay += font.linespace();
		}
		bool newline = ax == 0;

		for (size_t pos = first; pos < last; pos++)
		{
			char c = text[pos];
			const Font::Char& ch = font.chars[c];

			layout.advances[pos] = ax;

			if (newline && c == ' ')
				continue;

			ax += ch.ax;
			advance += ch.ax;
		}
		return newline ? first : prev;
	}

	Text::Layout GraphicsGL::LayoutBuilder::finish(size_t first, size_t last, size_t length)
	{
		layout.addline(ax, ay, first, last);
		layout.advances[length] = advance;
		layout.dimensions = Point<int16_t>(std::min(advance, maxwidth), layout.linecount * font.linespace());
		layout.endoffset = Point<int16_t>(ax % maxwidth, (layout.linecount - 1) * font.linespace());
		return layout;
	}


	void GraphicsGL::drawtext(const string& text, const Text::Layout& layout, Text::Font id, Text::Color colorid,
		Text::Background background, Point<int16_t> origin, float opacity) {

		if (locked)
			return;

		const Font& font = fonts[id];

		GLshort x = origin.x();
		GLshort y = origin.y();

		switch (background)
		{
		case Text::NAMETAG:
			for (auto& line : layout.lines)
			{
				GLshort left = x + line.position.x() - 2;
				GLshort right = left + layout.dimensions.x() + 3;
				GLshort top = y + line.position.y() - font.linespace() + 5;
				GLshort bottom = top + layout.dimensions.y() - 2;

				quads.push_back(Quad(left, right, top, bottom, nulloffset, 0.0f, 0.0f, 0.0f, 0.6f));
				quads.push_back(Quad(left - 1, left, top + 1, bottom - 1, nulloffset, 0.0f, 0.0f, 0.0f, 0.6f));
				quads.push_back(Quad(right, right + 1, top + 1, bottom - 1, nulloffset, 0.0f, 0.0f, 0.0f, 0.6f));
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
			{ 0.8f, 0.3f, 0.3f }, // DarkRed
			{ 0.5f, 0.25f, 0.0f }, // Brown
			{ 0.5f, 0.5f, 0.5f }, // Lightgrey
			{ 0.25f, 0.25f, 0.25f }, // Darkgrey
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

	void GraphicsGL::drawrectangle(int16_t x, int16_t y, int16_t w, int16_t h, float r, float g, float b, float a)
	{
		if (locked)
			return;

		Quad quad = Quad(x, x + w, y, y + h, nulloffset, r, g, b, a);
		quads.push_back(quad);
	}

	void GraphicsGL::drawscreenfill(float r, float g, float b, float a)
	{
		drawrectangle(0, -Constants::VIEWYOFFSET, Constants::VIEWWIDTH, Constants::VIEWHEIGHT, r, g, b, a);
	}

	void GraphicsGL::lock()
	{
		locked = true;
	}

	void GraphicsGL::unlock()
	{
		locked = false;
	}

	void GraphicsGL::flush(float opacity)
	{
		/*static bool showatlas = false;
		if (showatlas)
		{
			quads.clear();

			for (auto& oit : offsets)
			{
				const Offset& off = oit.second;
				Quad quad = Quad(off.l / 10, off.r / 10, off.t / 10, off.b / 10, off, 0.0f, 0.0f, 0.0f, 1.0f);
				quads.push_back(quad);
			}
		}*/

		bool coverscene = opacity != 1.0f;
		if (coverscene)
		{
			int16_t left = 0;
			int16_t right = left + Constants::VIEWWIDTH;
			int16_t top = -Constants::VIEWYOFFSET;
			int16_t bottom = top + Constants::VIEWHEIGHT;
			float complement = 1.0f - opacity;

			Quad quad = Quad(left, right, top, bottom, nulloffset, 0.0f, 0.0f, 0.0f, complement);
			quads.push_back(quad);
		}

		glClearColor(1.0, 1.0, 1.0, 1.0);
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

		if (coverscene)
		{
			quads.pop_back();
		}
	}

	void GraphicsGL::clearscene()
	{
		quads.clear();
	}
}
