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
#include "FontsDW.h"
#include <d2d1.h>
#include <d2d1helper.h>
#include <wincodecsdk.h>

namespace Program
{
	class LocatorD2D
	{
	public:
		void setfactory(IWICImagingFactory** f) { p_factory = f; }
		void settarget(ID2D1BitmapRenderTarget** t) { p_target = t; }
		void setfonts(FontsDW* d) { p_fonts = d; }
		IWICImagingFactory* getfactory() { return *p_factory; }
		ID2D1BitmapRenderTarget* gettarget() { return *p_target; }
		FontsDW* getfonts() { return p_fonts; }
	private:
		IWICImagingFactory** p_factory;
		ID2D1BitmapRenderTarget** p_target;
		FontsDW* p_fonts;
	};

	extern LocatorD2D locator;
}