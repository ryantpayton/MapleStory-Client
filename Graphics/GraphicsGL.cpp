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
#include "glm.hpp"

namespace Graphics
{
	GraphicsGL::~GraphicsGL()
	{
		free();
	}

	bool GraphicsGL::addfont(const char* name, Text::Font id, FT_UInt width, FT_UInt height)
	{
		if (FT_New_Face(ftlibrary, name, 0, &fonts[id]))
			return false;

		if (FT_Set_Pixel_Sizes(fonts[id], width, height))
			return false;

		/*FT_Face face = fonts[id];
		FT_GlyphSlot g = face->glyph;
		int w = 0;
		int h = 0;

		for (int i = 32; i < 128; i++)
		{
		if (FT_Load_Char(face, i, FT_LOAD_RENDER))
		continue;

		w += g->bitmap.width;
		if (g->bitmap.rows > h)
		h = g->bitmap.rows;
		}

		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &fatlass[id].tex);
		glBindTexture(GL_TEXTURE_2D, fatlass[id].tex);
		glUniform1i(uniform_tex, 0);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		fatlass[id].width = w;
		fatlass[id].height = h;

		int x = 0;
		for (uint8_t i = 32; i < 128; i++)
		{
		if (FT_Load_Char(face, i, FT_LOAD_RENDER))
		continue;

		glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, g->bitmap.width,
		g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

		FontAtlas::CharInfo* c = &fatlass[id].chars[i];
		c->ax = g->advance.x >> 6;
		c->ay = g->advance.y >> 6;
		c->bw = g->bitmap.width;
		c->bh = g->bitmap.rows;
		c->bl = g->bitmap_left;
		c->bt = g->bitmap_top;
		c->tx = (float)x / w;

		x += g->bitmap.width;
		}*/

		return true;
	}

	bool GraphicsGL::init()
	{
		if (glewInit())
			return false;

		/*if (FT_Init_FreeType(&ftlibrary))
			return false;

		addfont("arial.ttf", Text::A11L, 0, 11);
		addfont("arial.ttf", Text::A11M, 0, 11);
		addfont("arialbd.ttf", Text::A11B, 0, 11);
		addfont("arial.ttf", Text::A12M, 0, 12);
		addfont("arialbd.ttf", Text::A12B, 0, 12);
		addfont("arial.ttf", Text::A13M, 0, 13);
		addfont("arialbd.ttf", Text::A13B, 0, 13);
		addfont("arial.ttf", Text::A18M, 0, 18);*/

		GLint result = GL_FALSE;

		GLuint vs = glCreateShader(GL_VERTEX_SHADER);
		const char *vs_source =
			"#version 120\n"
			"attribute vec4 coord;"
			"varying vec2 texpos;"
			"void main(void) {"
			"   gl_Position = vec4(coord.x, coord.y - 0.04, 0.0, 1.0);"
			"	texpos = coord.zw;"
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
			"uniform sampler2D tex;"
			"void main(void) {"
			"	gl_FragColor = texture2D(tex, texpos);"
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
		uniform_tex = glGetUniformLocation(program, "tex");
		if (attribute_coord == -1 || uniform_tex == -1)
			return false;

		glGenBuffers(1, &vbo);
		glGenTextures(1, &atlas);

		return true;
	}

	void GraphicsGL::reinit()
	{
		glBindTexture(GL_TEXTURE_2D, atlas);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		clear();
	}

	void GraphicsGL::clear()
	{
		ymax = 0.0;
		coord = vector2d<GLint>();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA, ATLASW, ATLASH, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, nullptr);
		offsets.clear();
	}

	void GraphicsGL::free()
	{
		clear();

		glDeleteProgram(program);
		glDeleteBuffers(1, &vbo);
		glDeleteTextures(1, &atlas);
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

		// TODO
		if (coord.x() + bmp.width() > ATLASW)
		{
			coord.setx(0);
			coord.shifty(ymax);
			ymax = 0;
			if (coord.y() + bmp.height() > ATLASH)
			{
				clear();
			}
		}

		glTexSubImage2D(GL_TEXTURE_2D, 0, coord.x(), coord.y(), 
			bmp.width(), bmp.height(), GL_BGRA, GL_UNSIGNED_BYTE, bmp.data());

		offsets[id] = Offset(coord.x(), coord.y(), bmp.width(), bmp.height());
		coord.shiftx(bmp.width());
		if (bmp.height() > ymax)
			ymax = bmp.height();
	}

	void GraphicsGL::draw(size_t id, int16_t x, int16_t y, int16_t w, int16_t h, float alpha,
		float xscale, float yscale, int16_t centerx, int16_t centery) {

		if (!available(id))
			return;

		GLfloat left = centerx + xscale * (x - centerx);
		GLfloat right = centerx + xscale * (x + w - centerx);
		GLfloat top = centery + yscale * (y - centery);
		GLfloat bottom = centery + yscale * (y + h - centery);
		Offset offset = offsets[id];
		coordinates.push_back(TexCoords(left, right, top, bottom, offset));
	}

	void GraphicsGL::flush()
	{
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);

		GLsizei csize = static_cast<GLsizei>(coordinates.size() * sizeof(TexCoords)); 
		GLsizei fsize = static_cast<GLsizei>(coordinates.size()) * sizeof(GLfloat);
		glUseProgram(program);
		glEnableVertexAttribArray(attribute_coord);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, csize, coordinates.data(), GL_STREAM_DRAW);
		glVertexAttribPointer(attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glDrawArrays(GL_QUADS, 0, fsize);

		glDisableVertexAttribArray(attribute_coord);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		coordinates.clear();
	}

	Text::Layout GraphicsGL::createlayout(const char* text, Text::Font id, float wmax)
	{
		return Text::Layout();

		/*FT_Face face = fonts[id];
		if (face == nullptr)
			return Text::Layout();

		if (wmax < 1.0f)
			wmax = 800.0f;

		FT_GlyphSlot g = face->glyph;
		size_t i = 0;
		float advance = 0.0f;
		float x = 0.0f;
		float height = face->size->metrics.y_ppem;
		Text::Layout layout;

		for (const char* p = text; *p; p++)
		{
			if (FT_Load_Char(face, *p, FT_LOAD_COMPUTE_METRICS))
				continue;

			i++;
			float adv = (g->advance.x >> 6);
			advance += adv;
			x += adv;
			if (x > wmax)
			{
				height += face->size->metrics.y_ppem;
				x = 0.0f;
			}
			layout.advances[i] = advance;
		}

		if (advance > wmax)
			x = wmax;

		float xend = advance;
		while (xend > wmax)
		{
			xend -= wmax;
		}

		layout.dimensions = vector2d<float>(x, height);
		layout.endoffset = vector2d<int16_t>(
			static_cast<int16_t>(xend),
			static_cast<int16_t>(height - face->size->metrics.y_ppem)
			);

		return layout;*/
	}

	void GraphicsGL::drawtext(const char* text, Text::Font id, Text::Alignment alignment, Text::Color colorid,
		Text::Background, float opacity, vector2d<float> origin, vector2d<float> layout) {

		return;

		/*FT_Face face = fonts[id];
		if (face == nullptr)
			return;

		float x = origin.x();
		float y = origin.y();
		switch (alignment)
		{
		case Text::CENTER:
			x -= std::ceilf(layout.x() / 2);
			break;
		case Text::RIGHT:
			x -= layout.x();
			break;
		}
		x = -1.0f + x * sx;
		y = 1.0f - (y + 14.0f + face->size->metrics.y_ppem) * sy;

		if (opacity < 0.0f)
			opacity = 0.0f;
		static const float colors[Text::NUM_COLORS][3] =
		{
			{ 0.0f, 0.0f, 0.0f }, // Black
			{ 1.0f, 1.0f, 1.0f } // White
		};
		const float* ccol = colors[colorid];
		GLfloat color[4] = { ccol[0], ccol[1], ccol[2], 1.0f };
		//glUniform4fv(uniform_color, 1, color);

		struct point
		{
			GLfloat x;
			GLfloat y;
			GLfloat s;
			GLfloat t;
		};*/

		// 1. Method
		/*GLuint tex;
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glUniform1i(uniform_tex, 0);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glEnableVertexAttribArray(attribute_coord);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glUseProgram(program);

		float startx = x;
		float maxw = layout.x();
		if (maxw == 0.0f)
			maxw = 800.0f;

		FT_GlyphSlot g = face->glyph;
		for (const char* p = text; *p; p++) {
			if (FT_Load_Char(face, *p, FT_LOAD_RENDER))
				continue;

			glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, g->bitmap.width, g->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);

			float x2 = x + g->bitmap_left * sx;
			float y2 = -y - g->bitmap_top * sy;
			float w = g->bitmap.width * sx;
			float h = g->bitmap.rows * sy;

			point box[4] = {
				{ x2, -y2, 0, 0 },
				{ x2 + w, -y2, 1, 0 },
				{ x2, -y2 - h, 0, 1 },
				{ x2 + w, -y2 - h, 1, 1 },
			};

			glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			x += (g->advance.x >> 6) * sx;
			y += (g->advance.y >> 6) * sy;
			if (x > startx + maxw * sx)
			{
				x = startx;
				y -= face->size->metrics.y_ppem * sy;
			}
		}

		glDisableVertexAttribArray(attribute_coord);
		glDeleteTextures(1, &tex);*/

		// 2. Method
		
		/*FontAtlas& font = fatlass[id];

		glBindTexture(GL_TEXTURE_2D, font.tex);
		glUniform1i(uniform_tex, 0);

		glEnableVertexAttribArray(attribute_coord);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glUseProgram(program);

		point* coords = new point[6 * strlen(text)];
		int n = 0;
		for (const uint8_t* p = (const uint8_t*)text; *p; p++)
		{
			FontAtlas::CharInfo& c = font.chars[*p];
			float x2 = x + c.bl * sx;
			float y2 = -y - c.bt * sy;
			float w = c.bw * sx;
			float h = c.bh * sy;
			if (!w || !h)
				continue;

			x += c.ax * sx;
			y += c.ay * sy;

			coords[n++] = point{ x2, -y2, c.tx, 0 };
			coords[n++] = point{ x2, -y2 - h, c.tx, c.bt };
			coords[n++] = point{ x2 + w, -y2 - h, c.tx + c.bw / font.width, c.bt };
			coords[n++] = point{ x2 + w, -y2, c.tx + c.bw / font.width, 0 };
		}

		glBufferData(GL_ARRAY_BUFFER, sizeof (coords), coords, GL_STREAM_DRAW);
		glDrawArrays(GL_TRIANGLES, 0, n);

		glDisableVertexAttribArray(attribute_coord);

		delete[] coords;*/
	}
}
#endif
