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
#include "Journey.h"
#ifdef JOURNEY_USE_OPENGL
#include "TextWrapper.h"
#include "GL\glew.h"
#include "glfw3.h"
#include <map>

namespace IO
{
	using::std::map;
	// Implementation of text drawing using OpenGL.
	class TextWrapperGL : public TextWrapper
	{
	public:
		TextWrapperGL(Font, Textcolor, string, uint16_t);
		TextWrapperGL();
		void settext(string, uint16_t);
		void setfont(Font);
		void setcolor(Textcolor);
		void setback(TextBackground);
		void setalpha(float);
		void draw(vector2d<int32_t>) const;
		uint16_t getadvance(size_t) const;
		size_t getlength() const;
		string gettext() const;
		vector2d<uint16_t> getdimensions() const;
	private:
		GLuint texture;
		Font font;
		Textcolor color;
		string str;
		TextBackground back;
		float alpha;
		uint16_t width;
		uint16_t height;
		vector2d<uint16_t> endpos;
		map<size_t, float> advances;
	};
}
#endif