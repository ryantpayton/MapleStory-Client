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
#include "Window.h"
#include "UI.h"
#include "BitmapWrapperD2D.h"
#include "FontsDW.h"
#include <windowsx.h>
#include <math.h>

using namespace IO;

namespace Program
{
	class WindowD2D : public Window
	{
	public:
		WindowD2D();
		~WindowD2D();
		bool init(UI*);
		void update();
		void draw();
		void begin();
		void end();
		void togglemode();
		void fadeout(Transition);
		float getdpix() { return dpiX; }
		float getdpiy() { return dpiY; }
		HWND getwindow() { return wnd; }
		UI* getui() { return ui; }
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
		ID2D1Bitmap* scene;
		FontsDW fonts;
		UI* ui;
		Transition trans;
		float dpiX;
		float dpiY;
		float scralpha;
		float alphastep;
		short screencd;
		bool fullscreen;
		bool draw_finished;
	};
}

template<class Interface>
inline void SafeRelease(Interface **ppInterfaceToRelease)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();

		(*ppInterfaceToRelease) = NULL;
	}
}

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif