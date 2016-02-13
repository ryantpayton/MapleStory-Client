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
#include "Constants.h"
#include "Text.h"

#include "Util\Singleton.h"
#include "Util\rectangle2d.h"

#include "nlnx\bitmap.hpp"

#include "GL\glew.h"

#include "ft2build.h"
#include FT_FREETYPE_H

#include <unordered_map>
#include <vector>

namespace Graphics 
{
	using std::vector;
	using std::unordered_map;
	using nl::bitmap;

	class GraphicsGL : public Singleton<GraphicsGL>
	{
	public:
		// Initialise all resources.
		bool init();
		// Re-initialise, for example after changing screen modes.
		void reinit();
		// Clear all bitmaps.
		void clear();

		// Add a bitmap to the available resources.
		void addbitmap(const bitmap& toadd);
		// Return wether the bitmap with the given id is in the resource pool.
		bool available(size_t id);
		// Draw the bitmap with the given id and paramters.
		void draw(size_t id, int16_t x, int16_t y, int16_t w, 
			int16_t h, float a, float xs, float ys, int16_t cx, int16_t cy);
		void flush();

		// Create a layout for the text with the parameters specified.
		Text::Layout createlayout(const char* text, Text::Font font, float maxwidth);
		// Draw a text with the given parameters.
		void drawtext(const char* text, Text::Font font, Text::Alignment alignment, Text::Color color,
			Text::Background back, float opacity, vector2d<float> origin, vector2d<float> layout);

	private:
		bool addfont(const char* name, Text::Font id, FT_UInt width, FT_UInt height);

		struct FontAtlas
		{
			struct CharInfo
			{
				float ax;
				float ay;
				float bw;
				float bh;
				float bl;
				float bt;
				float tx;
			};

			GLuint tex;
			GLfloat width;
			GLfloat height;
			CharInfo chars[128];
		};

		FT_Library ftlibrary;
		FT_Face fonts[Text::NUM_FONTS];
		FontAtlas fatlass[Text::NUM_FONTS];

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

		struct TexCoords 
		{
			struct Coordinates 
			{
				GLshort x;
				GLshort y;
				GLshort s;
				GLshort t;

				GLfloat r;
				GLfloat a;
				GLfloat b;
				GLfloat g;
			};

			static const size_t LENGTH = 4;
			Coordinates coordinates[LENGTH];

			TexCoords(GLshort l, GLshort r, GLshort t, GLshort b, Offset o, GLfloat cr, GLfloat cg, GLfloat cb, GLfloat ca)
			{
				coordinates[0] = { l, t, o.l, o.t, cr, ca, cb, cg };
				coordinates[1] = { l, b, o.l, o.b, cr, ca, cb, cg };
				coordinates[2] = { r, b, o.r, o.b, cr, ca, cb, cg };
				coordinates[3] = { r, t, o.r, o.t, cr, ca, cb, cg };
			}
		};

		static const GLshort ATLASW = 10000;
		static const GLshort ATLASH = 10000;

		vector<TexCoords> coordinates;
		unordered_map<size_t, Offset> offsets;
		vector2d<GLshort> coord;
		GLshort ymax;

		GLint program;
		GLint attribute_coord;
		GLint attribute_color;
		GLint uniform_texture;
		GLint uniform_atlassize;
		GLint uniform_screensize;

		GLuint vbo;
		GLuint atlas;
	};
}
#endif

