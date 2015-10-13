/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015 SYJourney                                               //
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
#include "vector2d.h"

using namespace Util;

namespace Graphics
{
	enum Font
	{
		DWF_TEXTFIELD,
		DWF_12ML,
		DWF_12LL,
		DWF_12MC,
		DWF_12BC,
		DWF_12MR,
		DWF_14ML,
		DWF_14MC,
		DWF_14BC,
		DWF_14MR,
		DWF_20MC
	};

	enum Textcolor
	{
		TXC_BLACK,
		TXC_WHITE,
		TXC_YELLOW,
		TXC_BLUE,
		TXC_RED,
		TXC_BROWN,
		TXC_GREY,
		TXC_ORANGE,
		TXC_MBLUE,
		TXC_VIOLET
	};

	enum TextBackground
	{
		TXB_NONE,
		TXB_NAMETAG,
		TXB_GMCHAT
	};

	class TextWrapper
	{
	public:
		virtual ~TextWrapper() {}
		virtual void draw(vector2d<int>) = 0;
		virtual void settext(string, short) = 0;
		virtual void setalpha(float) = 0;
		virtual void setback(TextBackground) = 0;
		virtual short getadvance(size_t) = 0;
		virtual vector2d<short> getdimensions() = 0;
	};
}