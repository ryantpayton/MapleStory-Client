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

	const GLfloat ATLASW = 16000;
	const GLfloat ATLASH = 16000; 
	const GLfloat sx = 2.0f / static_cast<GLfloat>(Constants::VIEWWIDTH);
	const GLfloat sy = 2.0f / static_cast<GLfloat>(Constants::VIEWHEIGHT);

	class GraphicsGL : public Singleton<GraphicsGL>
	{
	public:
		// Free all resources.
		~GraphicsGL();

		// Initialise all resources.
		bool init();
		void reinit();
		// Clear all bitmaps.
		void clear();
		// Free all resources.
		void free();

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
			GLfloat l;
			GLfloat r;
			GLfloat t;
			GLfloat b;

			Offset(GLint x, GLint y, GLint w, GLint h)
			{
				l = x / ATLASW;
				r = (x + w) / ATLASW;
				t = y / ATLASH;
				b = (y + h) / ATLASH;
			}

			Offset() 
			{
				l = 0.0;
				r = 0.0;
				t = 0.0;
				b = 0.0;
			}
		};

		struct TexCoords 
		{
			struct Coordinates 
			{
				GLfloat x;
				GLfloat y;
				GLfloat s;
				GLfloat t;
			};

			Coordinates coordinates[4];

			TexCoords(GLfloat l, GLfloat r, GLfloat t, GLfloat b, Offset o)
			{
				l = -1.0 + l * sx;
				r = -1.0 + r * sx;
				t = 1.0 - t * sy;
				b = 1.0 - b * sy;

				coordinates[0] = { l, t, o.l, o.t };
				coordinates[1] = { l, b, o.l, o.b };
				coordinates[2] = { r, b, o.r, o.b };
				coordinates[3] = { r, t, o.r, o.t };
			}
		};

		vector<TexCoords> coordinates;
		unordered_map<size_t, Offset> offsets;
		vector2d<GLint> coord;
		GLint ymax;

		GLint program;
		GLint attribute_coord;
		GLint uniform_tex;

		GLuint vbo;
		GLuint atlas;
	};
}
#endif

