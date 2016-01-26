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
#include "Graphics\GraphicsD2D.h"
#ifndef JOURNEY_USE_OPENGL
#include <unordered_map>

namespace Graphics
{
	GraphicsD2D::GraphicsD2D()
	{
		p_factory = nullptr;
		p_target = nullptr;
		p_dwfactory = nullptr;
	}

	GraphicsD2D::~GraphicsD2D()
	{
		free();
	}

	void GraphicsD2D::addfont(Text::Font id, const WCHAR* name, float size, DWRITE_FONT_WEIGHT weight)
	{

		if (!p_dwfactory)
			return;

		IDWriteFactory* dwfactory = (*p_dwfactory);

		if (!dwfactory)
			return;

		dwfactory->CreateTextFormat(
			name,
			NULL,
			weight,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			size,
			L"en-us",
			&fonts[id]
			);
	}

	void GraphicsD2D::init(IWICImagingFactory** pwf, ID2D1BitmapRenderTarget** pt, IDWriteFactory** dwf)
	{
		p_factory = pwf;
		p_target = pt;
		p_dwfactory = dwf;

		if (!p_dwfactory || !p_target)
			return;

		IDWriteFactory* dwfactory = (*p_dwfactory);
		ID2D1BitmapRenderTarget* target = (*p_target);

		if (!dwfactory || !target)
			return;

		addfont(Text::A11L, L"Arial Light", 11.0f, DWRITE_FONT_WEIGHT_LIGHT);
		addfont(Text::A11M, L"Arial", 11.0f, DWRITE_FONT_WEIGHT_NORMAL);
		addfont(Text::A11B, L"Arial", 11.0f, DWRITE_FONT_WEIGHT_BOLD);
		addfont(Text::A12M, L"Arial", 12.0f, DWRITE_FONT_WEIGHT_NORMAL);
		addfont(Text::A12B, L"Arial", 12.0f, DWRITE_FONT_WEIGHT_BOLD);
		addfont(Text::A13M, L"Arial", 13.0f, DWRITE_FONT_WEIGHT_NORMAL);
		addfont(Text::A13B, L"Arial", 13.0f, DWRITE_FONT_WEIGHT_BOLD);
		addfont(Text::A18M, L"Arial", 18.0f, DWRITE_FONT_WEIGHT_NORMAL);

		target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brushes[Text::WHITE]);
		target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &brushes[Text::BLACK]);
		target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &brushes[Text::YELLOW]);
		target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue), &brushes[Text::BLUE]);
		target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &brushes[Text::RED]);
		target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Brown), &brushes[Text::BROWN]);
		target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gray), &brushes[Text::GREY]);
		target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DimGray), &brushes[Text::DARKGREY]);
		target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Orange), &brushes[Text::ORANGE]);
		target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::SkyBlue), &brushes[Text::MEDIUMBLUE]);
		target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Violet), &brushes[Text::VIOLET]);
	}

	void GraphicsD2D::clear()
	{
		for (auto& bmp : bitmaps)
		{
			ID2D1Bitmap* bitmap = bmp.second;
			if (bitmap)
				bitmap->Release();
		}

		bitmaps.clear();
	}

	void GraphicsD2D::free()
	{
		clear();

		for (size_t i = 0; i < Text::NUM_FONTS; i++)
		{
			IDWriteTextFormat* format = fonts[i];
			if (format)
				format->Release();
		}

		for (size_t i = 0; i < Text::NUM_COLORS; i++)
		{
			ID2D1SolidColorBrush* brush = brushes[i];
			if (brush)
				brush->Release();
		}
	}

	bool GraphicsD2D::available(size_t id)
	{
		return bitmaps.count(id) > 0;
	}

	void GraphicsD2D::addbitmap(const bitmap& bmp)
	{
		if (!p_factory || !p_target || available(bmp.id()))
			return;

		IWICImagingFactory* factory = (*p_factory);
		ID2D1BitmapRenderTarget* target = (*p_target);

		if (!factory || !target)
			return;

		IWICBitmap* wic = nullptr;

		factory->CreateBitmapFromMemory(
			bmp.width(), bmp.height(),
			GUID_WICPixelFormat32bppBGRA,
			4 * bmp.width(),
			bmp.length(),
			(BYTE*)bmp.data(),
			&wic);

		if (!wic)
			return;

		IWICFormatConverter* converter = nullptr;
		IWICBitmap* temp = nullptr;

		HRESULT result = factory->CreateFormatConverter(&converter);
		if (result == S_OK)
		{
			converter->Initialize(wic,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone, 0, 0.f,
				WICBitmapPaletteTypeMedianCut);
			factory->CreateBitmapFromSource(converter, WICBitmapNoCache, &temp);
			converter->Release();
		}
		wic->Release();

		target->CreateBitmapFromWicBitmap(temp, &bitmaps[bmp.id()]);
		temp->Release();
		temp = nullptr;
	}

	void GraphicsD2D::draw(size_t id, int16_t x, int16_t y, int16_t w, int16_t h, float alpha,
		float xscale, float yscale, int16_t ox, int16_t oy) {

		if (!p_target || !bitmaps.count(id))
			return;

		ID2D1BitmapRenderTarget* target = (*p_target);
		ID2D1Bitmap* todraw = bitmaps[id];

		if (!target || !todraw)
			return;

		bool transform = (xscale != 1.0f) || (yscale != 1.0f);
		if (transform)
		{
			target->SetTransform(
				D2D1::Matrix3x2F::Scale(
				D2D1::Size(xscale, yscale),
				D2D1::Point2F((FLOAT)ox, (FLOAT)oy)
				));
		}

		D2D_RECT_F rect = D2D1::RectF((FLOAT)x, (FLOAT)y, (FLOAT)(x + w), (FLOAT)(y + h));
		target->DrawBitmap(todraw, rect, alpha);

		if (transform)
		{
			target->SetTransform(
				D2D1::Matrix3x2F::Scale(
				D2D1::Size(1.0f, 1.0f),
				D2D1::Point2F((FLOAT)ox, (FLOAT)oy)
				));
		}
	}

	Text::Layout GraphicsD2D::createlayout(const wstring& text, Text::Font font, float wmax)
	{
		if (!p_dwfactory)
			return Text::Layout();

		IDWriteFactory* dwfactory = (*p_dwfactory);
		IDWriteTextFormat* fnt = fonts[font];

		if (!dwfactory || !fnt || text.size() == 0)
			return Text::Layout();

		if (wmax < 1.0f)
			wmax = 800.0f;

		IDWriteTextLayout* layout = nullptr;
		HRESULT err = dwfactory->CreateTextLayout(
			text.c_str(),
			(UINT32)text.length(),
			fnt, wmax, 624, &layout
			);

		if (err != S_OK)
			return Text::Layout();

		DWRITE_CLUSTER_METRICS* metrics = new DWRITE_CLUSTER_METRICS[text.length()];
		UINT32 result = 0;

		layout->GetClusterMetrics(metrics, (UINT32)text.size(), &result);
		if (result != text.size())
		{
			layout->GetClusterMetrics(metrics, result, &result);
		}

		float tadv = 0.0f;
		map<size_t, float> advances;
		for (UINT32 i = 0; i < result; i++)
		{
			advances[i] = tadv;
			tadv += metrics[i].width;
		}
		advances[result] = tadv;

		delete[] metrics;

		DWRITE_TEXT_METRICS tmetrics;
		layout->GetMetrics(&tmetrics);
		layout->Release();

		float width = (tadv > wmax) ? wmax : tadv;
		float height = tmetrics.height;

		int16_t linecount = 1;
		float xend = tadv;
		while (xend > wmax)
		{
			xend -= wmax;
			linecount++;
		}

		Text::Layout textlayout;
		textlayout.dimensions = vector2d<float>(width, height);
		textlayout.endoffset = vector2d<int16_t>(
			static_cast<int16_t>(xend),
			static_cast<int16_t>(height)
			);
		textlayout.advances = advances;
		textlayout.linecount = linecount;
		return textlayout;
	}

	void GraphicsD2D::drawtext(const wstring& text, Text::Font font, Text::Alignment alignment, Text::Color color,
		Text::Background back, float alpha, vector2d<float> pos, vector2d<float> dimensions) {

		if (!p_target)
			return;

		ID2D1BitmapRenderTarget* target = (*p_target);
		IDWriteTextFormat* fnt = fonts[font];
		ID2D1SolidColorBrush* brush = brushes[color];

		if (!target || !fnt || !brush)
			return;

		FLOAT x = pos.x();
		FLOAT y = pos.y() + 5;
		FLOAT width = dimensions.x();
		FLOAT height = dimensions.y();

		if (width == 0.0f)
			width = 800.0f;
		if (height == 0.0f)
			height = fnt->GetFontSize();

		if (x + width < 0.0f || x - width > 800.0f || y + height < 0.0f || y - height > 624.0f || text.size() == 0)
			return;

		D2D1_RECT_F layrect;
		switch (alignment)
		{
		case Text::LEFT:
			layrect = D2D1::RectF(x, y, x + width, y + height);
			fnt->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
			break;
		case Text::RIGHT:
			layrect = D2D1::RectF(x - width, y, x, y + height);
			fnt->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
			break;
		case Text::CENTER:
			layrect = D2D1::RectF(x - width / 2, y, x + width / 2, y + height);
			fnt->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
			break;
		}

		if (back == Text::NAMETAG)
		{
			ID2D1SolidColorBrush* backbrush = brushes[Text::BLACK];
			if (backbrush)
			{
				FLOAT hwidth = (width / 2) + 2.5f;
				FLOAT space = fnt->GetFontSize() + 2.0f;

				D2D1_RECT_F bglayout = D2D1::RectF(x - hwidth, y, x + hwidth, y + space);
				D2D1_ROUNDED_RECT rect = { bglayout, 1.0f, 1.0f };
				backbrush->SetOpacity(0.6f);
				target->DrawRoundedRectangle(&rect, backbrush);
				target->FillRoundedRectangle(&rect, backbrush);
				backbrush->SetOpacity(1.0f);
			}
		}

		brush->SetOpacity(alpha);
		target->DrawText(text.c_str(), (UINT32)text.size(), fnt, layrect, brush);
		brush->SetOpacity(1.0f);
	}
}
#endif