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
#ifndef JOURNEY_USE_OPENGL
#include "Textlabel.h"
#include <dwrite.h>
#include <d2d1.h>
#include <map>

namespace IO
{
	using::std::map;

	class FontsDW
	{
	public:
		FontsDW(){}
		~FontsDW();
		void init(IDWriteFactory*, ID2D1RenderTarget*);
		IDWriteTextFormat* getfont(Font) const;
		ID2D1SolidColorBrush* getbrush(Textcolor) const;
		IDWriteFactory* getdwfactory() const;
	private:
		IDWriteFactory* dwfactory;
		map<Font, IDWriteTextFormat*> fonts;
		map<Textcolor, ID2D1SolidColorBrush*> brushes;
	};
}
#endif