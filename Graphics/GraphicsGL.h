//////////////////////////////////////////////////////////////////////////////////
//	This file is part of the continued Journey MMORPG client					//
//	Copyright (C) 2015-2019  Daniel Allendorf, Ryan Payton						//
//																				//
//	This program is free software: you can redistribute it and/or modify		//
//	it under the terms of the GNU Affero General Public License as published by	//
//	the Free Software Foundation, either version 3 of the License, or			//
//	(at your option) any later version.											//
//																				//
//	This program is distributed in the hope that it will be useful,				//
//	but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				//
//	GNU Affero General Public License for more details.							//
//																				//
//	You should have received a copy of the GNU Affero General Public License	//
//	along with this program.  If not, see <https://www.gnu.org/licenses/>.		//
//////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Text.h"

#include "../Constants.h"
#include "../Error.h"

#include "../Util/QuadTree.h"
#include "../Template/Singleton.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <nlnx/bitmap.hpp>

namespace ms
{
	// Graphics engine which uses OpenGL.
	class GraphicsGL : public Singleton<GraphicsGL>
	{
	public:
		GraphicsGL();

		// Initialise all resources.
		Error init();
		// Re-initialise after changing screen modes.
		void reinit();

		// Clear all bitmaps if most of the space is used up.
		void clear();

		// Add a bitmap to the available resources.
		void addbitmap(const nl::bitmap& bmp);
		// Draw the bitmap with the given parameters.
		void draw(const nl::bitmap& bmp, const Rectangle<int16_t>& rect, const Color& color, float angle);

		// Create a layout for the text with the parameters specified.
		Text::Layout createlayout(const std::string& text, Text::Font font, Text::Alignment alignment, int16_t maxwidth, bool formatted, int16_t line_adj);
		// Draw a text with the given parameters.
		void drawtext(const DrawArgument& args, const std::string& text, const Text::Layout& layout, Text::Font font, Color::Name color, Text::Background back);

		// Draw a rectangle filled with the specified color.
		void drawrectangle(int16_t x, int16_t y, int16_t w, int16_t h, float r, float g, float b, float a);
		// Fill the screen with the specified color.
		void drawscreenfill(float r, float g, float b, float a);

		// Lock the current scene.
		void lock();
		// Unlock the scene.
		void unlock();

		// Draw the buffer contents with the specified scene opacity.
		void flush(float opacity);
		// Clear the buffer contents.
		void clearscene();

	private:
		void clearinternal();
		bool addfont(const char* name, Text::Font id, FT_UInt width, FT_UInt height);

		struct Offset
		{
			GLshort l;
			GLshort r;
			GLshort t;
			GLshort b;

			Offset(GLshort x, GLshort y, GLshort w, GLshort h)
			{
				l = x;
				r = x + w;
				t = y;
				b = y + h;
			}

			Offset()
			{
				l = 0;
				r = 0;
				t = 0;
				b = 0;
			}
		};

		// Add a bitmap to the available resources.
		const Offset& getoffset(const nl::bitmap& bmp);

		struct Leftover
		{
			GLshort l;
			GLshort r;
			GLshort t;
			GLshort b;

			Leftover(GLshort x, GLshort y, GLshort w, GLshort h)
			{
				l = x;
				r = x + w;
				t = y;
				b = y + h;
			}

			Leftover()
			{
				l = 0;
				r = 0;
				t = 0;
				b = 0;
			}

			GLshort width() const
			{
				return r - l;
			}

			GLshort height() const
			{
				return b - t;
			}
		};

		struct Quad
		{
			struct Vertex
			{
				GLshort x;
				GLshort y;
				GLshort s;
				GLshort t;

				Color c;
			};

			static const size_t LENGTH = 4;
			Vertex vertices[LENGTH];

			Quad(GLshort l, GLshort r, GLshort t, GLshort b, const Offset& o, const Color& color, GLfloat rot)
			{
				vertices[0] = { l, t, o.l, o.t, color };
				vertices[1] = { l, b, o.l, o.b, color };
				vertices[2] = { r, b, o.r, o.b, color };
				vertices[3] = { r, t, o.r, o.t, color };

				if (rot != 0.0f)
				{
					float cos = std::cos(rot);
					float sin = std::sin(rot);
					GLshort cx = (l + r) / 2;
					GLshort cy = (t + b) / 2;

					for (int i = 0; i < 4; i++)
					{
						GLshort vx = vertices[i].x - cx;
						GLshort vy = vertices[i].y - cy;
						GLfloat rx = std::roundf(vx * cos - vy * sin);
						GLfloat ry = std::roundf(vx * sin + vy * cos);
						vertices[i].x = static_cast<GLshort>(rx + cx);
						vertices[i].y = static_cast<GLshort>(ry + cy);
					}
				}
			}
		};

		struct Font
		{
			struct Char
			{
				GLshort ax;
				GLshort ay;
				GLshort bw;
				GLshort bh;
				GLshort bl;
				GLshort bt;
				Offset offset;
			};

			GLshort width;
			GLshort height;
			Char chars[128];

			Font(GLshort w, GLshort h)
			{
				width = w;
				height = h;
			}

			Font()
			{
				width = 0;
				height = 0;
			}

			int16_t linespace() const
			{
				return static_cast<int16_t>(height * 1.35 + 1);
			}
		};

		class LayoutBuilder
		{
		public:
			LayoutBuilder(const Font& font, Text::Alignment alignment, int16_t maxwidth, bool formatted, int16_t line_adj);

			size_t add(const char* text, size_t prev, size_t first, size_t last);
			Text::Layout finish(size_t first, size_t last);

		private:
			void add_word(size_t first, size_t last, Text::Font font, Color::Name color);
			void add_line();

			const Font& font;

			Text::Alignment alignment;
			Text::Font fontid;
			Color::Name color;
			int16_t maxwidth;
			bool formatted;

			int16_t ax;
			int16_t ay;

			std::vector<Text::Layout::Line> lines;
			std::vector<Text::Layout::Word> words;
			std::vector<int16_t> advances;
			int16_t width;
			int16_t endy;
			int16_t line_adj;
		};

		int16_t VWIDTH;
		int16_t VHEIGHT;
		Rectangle<int16_t> SCREEN;

		static const GLshort ATLASW = 8192;
		static const GLshort ATLASH = 8192;
		static const GLshort MINLOSIZE = 32;

		bool locked;

		std::vector<Quad> quads;
		GLuint vbo;
		GLuint atlas;

		GLint program;
		GLint attribute_coord;
		GLint attribute_color;
		GLint uniform_texture;
		GLint uniform_atlassize;
		GLint uniform_screensize;
		GLint uniform_yoffset;
		GLint uniform_fontregion;

		std::unordered_map<size_t, Offset> offsets;
		Offset nulloffset;

		QuadTree<size_t, Leftover> leftovers;
		size_t rlid;
		size_t wasted;
		Point<GLshort> border;
		Range<GLshort> yrange;

		FT_Library ftlibrary;
		Font fonts[Text::Font::NUM_FONTS];
		Point<GLshort> fontborder;
		GLshort fontymax;
	};
}