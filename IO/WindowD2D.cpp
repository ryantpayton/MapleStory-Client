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
#include "WindowD2D.h"
#include "UI.h"
#include "Program\Configuration.h"
#include "Program\Constants.h"
#include "Gameplay\Stage.h"
#include "Graphics\GraphicsD2D.h"

namespace IO
{
	WindowD2D::WindowD2D()
	{
		HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);
		CoInitialize(NULL); 

		moninfo = { sizeof(MONITORINFO) };
		placement = { sizeof(WINDOWPLACEMENT) };
	}

	WindowD2D::~WindowD2D()
	{
		Graphics::GraphicsD2D::clear();
		CoUninitialize();

		if (d2dfactory) 
			d2dfactory->Release();
		if (dwfactory) 
			dwfactory->Release();
	}

	vector2d<int16_t> paramtov2d(LPARAM lParam)
	{
		return vector2d<int16_t>(
			GET_X_LPARAM(lParam),
			GET_Y_LPARAM(lParam)
			);
	}

	LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		WindowD2D* app = reinterpret_cast<WindowD2D*>(
			::GetWindowLongPtrW(hwnd, GWLP_USERDATA)
			);

		if (message != WM_CREATE && app == nullptr)
			return DefWindowProc(hwnd, message, wParam, lParam);

		switch (message)
		{
		case WM_CREATE:
			::SetWindowLongPtrW(
				hwnd,
				GWLP_USERDATA,
				(LONG_PTR)(((LPCREATESTRUCT)lParam)->lpCreateParams)
				);
			return 1;
		case WM_DISPLAYCHANGE:
			InvalidateRect(hwnd, NULL, FALSE);
			return 1;
		case WM_SETCURSOR:
			SetCursor(NULL);
			return 1;
		case WM_MOUSEMOVE:
			UI::sendmouse(paramtov2d(lParam));
			return 0;
		case WM_LBUTTONDBLCLK:
			UI::doubleclick(paramtov2d(lParam));
			return 0;
		case WM_LBUTTONDOWN:
			switch (wParam)
			{
			case MK_LBUTTON:
				UI::sendmouse(true, paramtov2d(lParam));
				break;
			}
			return 0;
		case WM_LBUTTONUP:
			if (wParam != MK_LBUTTON)
				UI::sendmouse(false, paramtov2d(lParam));
			return 0;
		case WM_SYSCOMMAND:
			if (wParam == SC_KEYMENU)
				return 0;
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
			app->handlekey(wParam, true);
			return 0;
		case WM_KEYUP:
		case WM_SYSKEYUP:
			app->handlekey(wParam, false);
			return 0;
		}

		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	void WindowD2D::handlekey(WPARAM keycode, bool down)
	{
		if (keycode == VK_F11 && down)
			togglemode();
		else
			UI::sendkey(static_cast<int32_t>(keycode), down);
	}

	bool WindowD2D::init()
	{
		if (initfactories() != S_OK)
			return false;

		LPCSTR title = (LPCSTR)"Journey";

		WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
		wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = sizeof(LONG_PTR);
		wcex.hbrBackground = NULL;
		wcex.lpszMenuName = NULL;
		wcex.hInstance = NULL;
		wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
		wcex.lpszClassName = title;

		RegisterClassEx(&wcex);

		float dpiX;
		float dpiY;
		d2dfactory->GetDesktopDpi(&dpiX, &dpiY);

		GetMonitorInfo(
			MonitorFromWindow(wnd, MONITOR_DEFAULTTOPRIMARY), 
			&moninfo
			);

		wnd = CreateWindow(
			title,
			title,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			static_cast<int32_t>(ceil(816.f * dpiX / 96.f)),
			static_cast<int32_t>(ceil(628.f * dpiY / 96.f)),
			NULL,
			NULL,
			NULL,
			this
			);

		if (wnd == nullptr)
			return false;

		if (inittargets() != S_OK)
			return false;

		Graphics::GraphicsD2D::init(
			&imgfactory, 
			&bmptarget, 
			&dwfactory
			);

		screencd = 0;
		opacity = 1.0f;
		transition = false;
		fullscreen = false;

		if (Program::Configuration::getbool("Fullscreen"))
			togglemode();

		SetPriorityClass(wnd, REALTIME_PRIORITY_CLASS);
		SetFocus(wnd);
		ShowWindow(wnd, SW_SHOWNORMAL);
		UpdateWindow(wnd);

		return true;
	}

	HRESULT WindowD2D::initfactories()
	{
		HRESULT result = CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&imgfactory)
			);

		if (result != S_OK)
			return result;

		result = D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED, 
			&d2dfactory
			);

		if (result != S_OK)
			return result;

		result = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&dwfactory)
			);

		return result;
	}

	HRESULT WindowD2D::inittargets()
	{
		RECT rc;
		GetClientRect(wnd, &rc);
		D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

		D2D1_PIXEL_FORMAT pixelformat = D2D1::PixelFormat(
			DXGI_FORMAT_B8G8R8A8_UNORM, 
			D2D1_ALPHA_MODE_IGNORE
			);

		HRESULT result = d2dfactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, pixelformat),
			D2D1::HwndRenderTargetProperties(wnd, size),
			&d2dtarget
			);

		if (result != S_OK)
			return result;

		result = d2dtarget->CreateCompatibleRenderTarget(&bmptarget);

		if (result != S_OK)
			return result;

		d2dtarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
		bmptarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);

		return result;
	}

	void WindowD2D::gofullscreen()
	{
		if (!GetWindowPlacement(wnd, &placement))
			return;

		SetWindowLong(wnd, GWL_STYLE, WS_VISIBLE);
		SetWindowPos(wnd, HWND_TOP,
			moninfo.rcMonitor.left, moninfo.rcMonitor.top,
			moninfo.rcMonitor.right - moninfo.rcMonitor.left,
			moninfo.rcMonitor.bottom - moninfo.rcMonitor.top,
			SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}

	void WindowD2D::gowindowed()
	{
		SetWindowLong(wnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(wnd, &placement);
		SetWindowPos(wnd, NULL, 0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
			SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}

	void WindowD2D::togglemode()
	{
		if (screencd > 0)
			return;

		if (fullscreen)
			gowindowed();
		else
			gofullscreen();

		fullscreen = !fullscreen;
		screencd = 50;
	}

	void WindowD2D::fadeout()
	{
		opcstep = -0.025f;
		transition = true;
	}

	void WindowD2D::update()
	{
		MSG winmsg;
		if (PeekMessage(&winmsg, 0, WM_KEYFIRST, WM_MOUSELAST, PM_REMOVE))
		{
			TranslateMessage(&winmsg);
			DispatchMessage(&winmsg);
		}

		updateopc();
	}

	void WindowD2D::updateopc()
	{
		screencd -= 1;

		if (opcstep == 0.0f)
			return;

		opacity += opcstep;

		if (opacity >= 1.0f)
		{
			opacity = 1.0f;
		}
		else if (opacity <= 0.0f)
		{
			opacity = 0.0f;
			opcstep = 0.025f;
			transition = false;

			Gameplay::Stage::reload();
			UI::enable();
		}
	}

	void WindowD2D::begin() const
	{
		if (!transition)
		{
			bmptarget->BeginDraw();
			bmptarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
		}
	}

	void WindowD2D::end() const
	{
		if (!transition)
			bmptarget->EndDraw();

		static const D2D1_RECT_F drc = 
			D2D1::RectF(0.0f, 0.0f, 800.0f, 589.0f);

		ID2D1Bitmap* scene;
		bmptarget->GetBitmap(&scene);

		d2dtarget->BeginDraw();
		d2dtarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
		d2dtarget->DrawBitmap(scene, drc, opacity);
		d2dtarget->EndDraw();
	}
}