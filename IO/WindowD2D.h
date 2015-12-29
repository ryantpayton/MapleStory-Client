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
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodecsdk.h>
#include <windowsx.h>

namespace IO
{
	class WindowD2D
	{
	public:
		WindowD2D();
		~WindowD2D();

		bool init();
		void update();
		void begin() const;
		void end() const;
		void togglemode();
		void fadeout();

	private:
		HRESULT initfactories();
		HRESULT inittargets();
		void updateopc();

		HWND wnd;
		WINDOWPLACEMENT placement;

		IWICImagingFactory* imgfactory;
		ID2D1Factory* d2dfactory;
		IDWriteFactory* dwfactory;
		ID2D1HwndRenderTarget* d2dtarget;
		ID2D1BitmapRenderTarget* bmptarget;

		float opacity;
		float opcstep;
		short screencd;
		bool transition;
	};
}
#endif