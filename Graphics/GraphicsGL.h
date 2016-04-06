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
#include "Constants.h"
#include "Text.h"

#include "Util\QuadTree.h"
#include "Util\Singleton.h"
#include "Util\rectangle2d.h"

#include "nlnx\bitmap.hpp"

#include "GL\glew.h"

#include "ft2build.h"
#include FT_FREETYPE_H

#include <unordered_map>
#include <hash_set>
#include <vector>

namespace Graphics 
{
	using std::vector;
	using std::unordered_map;
	using nl::bitmap;

	// Graphics engine which uses OpenGL.
	class GraphicsGL : public Singleton<GraphicsGL>
	{
	public:
		GraphicsGL();

		// Initialise all resources.
		bool init();
		// Re-initialise after changing screen modes.
		void reinit();

		// Clear all bitmaps if most of the space is used up.
		void clear();

		// Add a bitmap to the available resources.
		void addbitmap(const bitmap& bmp);
		// Draw the bitmap with the given parameters.
		void draw(const bitmap& bmp, int16_t x, int16_t y, int16_t tx, int16_t ty,
			float a, float xs, float ys, int16_t cx, int16_t cy);

		// Create a layout for the text with the parameters specified.
		Text::Layout createlayout(const string& text, Text::Font font, Text::Alignment alignment, int16_t maxwidth);
		// Draw a text with the given parameters.
		void drawtext(const string& text, const Text::Layout& layout, Text::Font font, 
			Text::Color color, Text::Background back, Point<int16_t> origin, float opacity);

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
				t = y ;
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
		const Offset& getoffset(const bitmap& bmp);

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

				GLfloat r;
				GLfloat g;
				GLfloat b;
				GLfloat a;
			};

			static const size_t LENGTH = 4;
			Vertex vertices[LENGTH];

			Quad(GLshort l, GLshort r, GLshort t, GLshort b, const Offset& o, GLfloat cr, GLfloat cg, GLfloat cb, GLfloat ca)
			{
				vertices[0] = { l, t, o.l, o.t, cr, cg, cb, ca };
				vertices[1] = { l, b, o.l, o.b, cr, cg, cb, ca };
				vertices[2] = { r, b, o.r, o.b, cr, cg, cb, ca };
				vertices[3] = { r, t, o.r, o.t, cr, cg, cb, ca };
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
			LayoutBuilder(const Font& font, Text::Alignment alignment, int16_t maxwidth);

			size_t addword(const char* text, size_t prev, size_t first, size_t last);
			Text::Layout finish(size_t first, size_t last, size_t length);

		private:
			LayoutBuilder& operator = (const LayoutBuilder&) = delete;

			const Font& font;
			Text::Layout layout;
			int16_t maxwidth;
			int16_t advance;
			int16_t ax;
			int16_t ay;
		};

		static const GLshort ATLASW = 10000;
		static const GLshort ATLASH = 10000;
		static const GLshort MINLOSIZE = 32;

		bool locked;

		vector<Quad> quads;
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

		unordered_map<size_t, Offset> offsets;
		Offset nulloffset;

		QuadTree<size_t, Leftover> leftovers;
		size_t rlid;
		size_t wasted;
		Point<GLshort> border;
		Range<GLshort> yrange;

		FT_Library ftlibrary;
		Font fonts[Text::NUM_FONTS];
		Point<GLshort> fontborder;
		GLshort fontymax;
	};
}

