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
#include "WindowD2D.h"
#include "UI.h"
#include "Program\Constants.h"
#include "Gameplay\Stage.h"

namespace IO
{
	WindowD2D::WindowD2D() : graphicsd2d(&imgfactory, &bitmaptarget, &dwfactory)
	{
		HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);
		CoInitialize(NULL);
	}

	WindowD2D::~WindowD2D()
	{
		CoUninitialize();
		if (d2d_factory) d2d_factory->Release();
		if (dwfactory) dwfactory->Release();
	}

	bool WindowD2D::init()
	{
		HRESULT result = initfactories();
		if (result == S_OK)
		{
			LPCSTR title = (LPCSTR)"Journey";

			WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
			wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
			wcex.lpfnWndProc = WindowD2D::WndProc;
			wcex.cbClsExtra = 0;
			wcex.cbWndExtra = sizeof(LONG_PTR);
			wcex.hbrBackground = NULL;
			wcex.lpszMenuName = NULL;
			wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
			wcex.lpszClassName = title;

			RegisterClassEx(&wcex);

			d2d_factory->GetDesktopDpi(&dpiX, &dpiY);

			wnd = CreateWindow(
				title,
				title,
				WS_OVERLAPPED,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				static_cast<int>(ceil(816.f * dpiX / 96.f)),
				static_cast<int>(ceil(628.f * dpiY / 96.f)),
				NULL,
				NULL,
				NULL,
				this
				);

			if (wnd)
			{
				result = inittargets();
				if (result == S_OK)
				{
					graphicsd2d.init();

					screencd = 0;
					scralpha = 1.0f;
					draw_finished = true;
					transition = false;

					SetPriorityClass(wnd, REALTIME_PRIORITY_CLASS);
					SetFocus(wnd);
					ShowWindow(wnd, SW_SHOWNORMAL);
					UpdateWindow(wnd);
				}
			}
		}
		return wnd != 0 && result == S_OK;
	}

	HRESULT WindowD2D::initfactories()
	{
		HRESULT result = CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&imgfactory)
			);

		if (result == S_OK)
		{
			result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2d_factory);
			if (result == S_OK)
			{
				result = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&dwfactory));
			}
		}
		return result;
	}

	HRESULT WindowD2D::inittargets()
	{
		RECT rc;
		GetClientRect(wnd, &rc);
		D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

		HRESULT result = d2d_factory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE)),
			D2D1::HwndRenderTargetProperties(wnd, size),
			&d2d_rtarget
			);

		if (result == S_OK)
		{
			result = d2d_rtarget->CreateCompatibleRenderTarget(&bitmaptarget);
			if (result == S_OK)
			{
				d2d_rtarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
				bitmaptarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
			}
		}
		return result;
	}

	LRESULT CALLBACK WindowD2D::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		LRESULT result = 0;
		UINT width, height;

		if (message == WM_CREATE)
		{
			LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
			WindowD2D* app = (WindowD2D*)pcs->lpCreateParams;

			::SetWindowLongPtrW(
				hwnd,
				GWLP_USERDATA,
				(long long)app
				);

			result = 1;
		}
		else
		{
			WindowD2D* app = reinterpret_cast<WindowD2D*>(::GetWindowLongPtrW(hwnd, GWLP_USERDATA));

			bool wasHandled = false;

			if (app)
			{
				switch (message)
				{
				case WM_SIZE:
					width = LOWORD(lParam);
					height = HIWORD(lParam);
					result = 0;
					wasHandled = true;
					break;
				case WM_DISPLAYCHANGE:
					InvalidateRect(hwnd, NULL, FALSE);
					result = 0;
					wasHandled = true;
					break;
				case WM_DESTROY:
				case WM_QUIT:
					result = 1;
					wasHandled = true;
					break;
				case WM_MOUSEMOVE:
					UI::sendmouse(vector2d<int16_t>(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
					result = 0;
					wasHandled = true;
					break;
				case WM_SETCURSOR:
					SetCursor(NULL);
					result = 1;
					wasHandled = true;
					break;
				case WM_LBUTTONDOWN:
					switch (wParam)
					{
					case MK_LBUTTON:
						UI::sendmouse(Cursor::MST_CLICKING, vector2d<int16_t>(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
						break;
					}
					result = 0;
					wasHandled = true;
					break;
				case WM_LBUTTONDBLCLK:
					//app->getui()->doubleclick(vector2d(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
					result = 0;
					wasHandled = true;
					break;
				case WM_LBUTTONUP:
					if (wParam != MK_LBUTTON)
					{
						UI::sendmouse(Cursor::MST_IDLE, vector2d<int16_t>(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
					}
					result = 0;
					wasHandled = true;
					break;
				case WM_KEYDOWN:
					if (wParam == VK_TAB)
						app->togglemode();
					else
						UI::sendkey(static_cast<uint8_t>(wParam), true);
					result = 0;
					wasHandled = true;
					break;
				case WM_KEYUP:
					UI::sendkey(static_cast<uint8_t>(wParam), false);
					result = 0;
					wasHandled = true;
					break;
				}
			}

			if (!wasHandled)
			{
				result = DefWindowProc(hwnd, message, wParam, lParam);
			}
		}

		return result;
	}

	WINDOWPLACEMENT g_wpPrev = { sizeof(g_wpPrev) };

	void WindowD2D::togglemode()
	{
		if (screencd <= 0)
		{
			DWORD dwStyle = GetWindowLong(wnd, GWL_STYLE);
			if (dwStyle & WS_OVERLAPPEDWINDOW) {
				MONITORINFO mi = { sizeof(mi) };
				if (GetWindowPlacement(wnd, &g_wpPrev) &&
					GetMonitorInfo(MonitorFromWindow(wnd,
					MONITOR_DEFAULTTOPRIMARY), &mi)) {
					SetWindowLong(wnd, GWL_STYLE,
						dwStyle & ~WS_OVERLAPPEDWINDOW);
					SetWindowPos(wnd, HWND_TOP,
						mi.rcMonitor.left, mi.rcMonitor.top,
						mi.rcMonitor.right - mi.rcMonitor.left,
						mi.rcMonitor.bottom - mi.rcMonitor.top,
						SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
				}
			}
			else {
				SetWindowLong(wnd, GWL_STYLE,
					dwStyle | WS_OVERLAPPEDWINDOW);
				SetWindowPlacement(wnd, &g_wpPrev);
				SetWindowPos(wnd, NULL, 0, 0, 0, 0,
					SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
					SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
			}
			screencd = 16;
		}
	}

	void WindowD2D::fadeout()
	{
		alphastep = -0.025f;
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

		screencd -= 1;
		if (alphastep != 0.0f)
		{
			scralpha += alphastep;

			if (scralpha >= 1.0f)
			{
				scralpha = 1.0f;
			}
			else if (scralpha <= 0.0f)
			{
				scralpha = 0.0f;
				alphastep = 0.025f;
				transition = false;

				Gameplay::Stage::reload();
				UI::enable();
				UI::enablegamekeys(true);
			}
		}
	}

	void WindowD2D::begin() const
	{
		if (!transition)
		{
			bitmaptarget->BeginDraw();
			bitmaptarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
		}
	}

	void WindowD2D::end() const
	{
		if (!transition)
			bitmaptarget->EndDraw();

		ID2D1Bitmap* scene;
		bitmaptarget->GetBitmap(&scene);

		D2D1_RECT_F drc = D2D1::RectF(0.0f, 0.0f, 800.0f, 589.0f);

		d2d_rtarget->BeginDraw();
		d2d_rtarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
		d2d_rtarget->DrawBitmap(scene, drc, scralpha);
		d2d_rtarget->EndDraw();
	}
}
#endif