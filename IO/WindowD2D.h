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
#include "Graphics\GraphicsD2D.h"
#include <windowsx.h>
#include <math.h>

namespace IO
{
	using::Graphics::GraphicsD2D;

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
		float getdpix() { return dpiX; }
		float getdpiy() { return dpiY; }

	private:
		void endtransition();
		HRESULT initfactories();
		HRESULT inittargets();
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		HWND wnd;
		IWICImagingFactory* imgfactory;
		ID2D1Factory* d2d_factory;
		IDWriteFactory* dwfactory;
		ID2D1HwndRenderTarget* d2d_rtarget;
		ID2D1BitmapRenderTarget* bitmaptarget;
		GraphicsD2D graphicsd2d;
		float dpiX;
		float dpiY;
		float scralpha;
		float alphastep;
		short screencd;
		bool fullscreen;
		bool draw_finished;
	};
}
#endif