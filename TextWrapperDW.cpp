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
#include "TextWrapperDW.h"
#include "LocatorD2D.h"

namespace IO
{
	TextWrapperDW::TextWrapperDW(Font f, Textcolor c, string s, uint16_t m)
	{
		font = f;
		color = c;
		settext(s, m);

		alpha = 1.0f;
		back = TXB_NONE;
	}

	TextWrapperDW::TextWrapperDW() {}

	void TextWrapperDW::settext(string t, uint16_t wmax)
	{
		IDWriteTextFormat* fnt = Graphics::locator.getfonts()->getfont(font);
		if (fnt)
		{
			str = t;
			text = wstring(t.begin(), t.end());
			int32_t totlen = createlayout(wmax);
			float space = fnt->GetFontSize() * 1.25f;
			if (totlen > wmax && wmax > 0)
			{
				width = wmax;
				height = static_cast<uint16_t>(space)* ((totlen / wmax) + 1);
				endpos = vector2d<uint16_t>(totlen % wmax, height - static_cast<uint16_t>(fnt->GetFontSize()));
			}
			else
			{
				width = totlen;
				height = static_cast<uint16_t>(space);
				endpos = vector2d<uint16_t>(totlen, 0);
			}
		}
	}

	int TextWrapperDW::createlayout(uint16_t wmax)
	{
		int32_t total = 0;
		IDWriteFactory* dwfactory = Graphics::locator.getfonts()->getdwfactory();
		IDWriteTextFormat* fnt = Graphics::locator.getfonts()->getfont(font);
		if (dwfactory && fnt && text.size() > 0)
		{
			if (wmax < 1)
			{
				wmax = 816;
			}

			IDWriteTextLayout* layout = 0;
			dwfactory->CreateTextLayout(
				text.c_str(),
				(UINT32)text.length(),
				fnt,
				wmax,
				624,
				&layout
				);

			if (layout)
			{
				DWRITE_CLUSTER_METRICS* metrics = new DWRITE_CLUSTER_METRICS[text.length()];
				UINT32 result = 0;

				layout->GetClusterMetrics(metrics, (UINT32)text.size(), &result);
				if (result != text.size())
				{
					layout->GetClusterMetrics(metrics, result, &result);
				}

				float tadv = 0.0f;
				advances.clear();
				for (UINT32 i = 0; i < result; i++)
				{
					advances[i] = tadv;
					tadv += metrics[i].width;
				}
				advances[result] = tadv;

				delete[] metrics;
				layout->Release();
				total = static_cast<int32_t>(tadv);
			}
		}
		return total;
	}

	void TextWrapperDW::setfont(Font f)
	{
		font = f;
	}

	void TextWrapperDW::setcolor(Textcolor c)
	{
		color = c;
	}

	void TextWrapperDW::setback(TextBackground b)
	{
		back = b;
	}

	void TextWrapperDW::setalpha(float a)
	{
		alpha = a;
	}

	void TextWrapperDW::draw(vector2d<int32_t> pos) const
	{
		ID2D1RenderTarget* target = Graphics::locator.gettarget();
		IDWriteTextFormat* fnt = Graphics::locator.getfonts()->getfont(font);
		if (target && fnt)
		{
			pos.shifty(5);

			if (pos.x() + width >= 0 && pos.x() - width <= 816 && pos.y() + height >= 0 && pos.y() - height <= 624 && text.size() > 0)
			{
				uint16_t lrwidth = width + 5;
				D2D1_RECT_F layrect;
				switch (fnt->GetTextAlignment())
				{
				case DWRITE_TEXT_ALIGNMENT_LEADING:
					layrect = D2D1::RectF(
						(FLOAT)(pos.x()),
						(FLOAT)(pos.y()),
						(FLOAT)(pos.x() + lrwidth),
						(FLOAT)(pos.y() + height)
						);
					break;
				case DWRITE_TEXT_ALIGNMENT_TRAILING:
					layrect = D2D1::RectF(
						(FLOAT)(pos.x() - lrwidth),
						(FLOAT)(pos.y()),
						(FLOAT)(pos.x()),
						(FLOAT)(pos.y() + height)
						);
					break;
				case DWRITE_TEXT_ALIGNMENT_CENTER:
					layrect = D2D1::RectF(
						(FLOAT)(pos.x() - lrwidth / 2),
						(FLOAT)(pos.y()),
						(FLOAT)(pos.x() + lrwidth / 2),
						(FLOAT)(pos.y() + height)
						);
					break;
				default:
					return;
				}

				if (back == TXB_NAMETAG)
				{
					ID2D1SolidColorBrush* backbrush = Graphics::locator.getfonts()->getbrush(TXC_BLACK);
					if (backbrush)
					{
						FLOAT hwidth = ((FLOAT)lrwidth) / 2;
						FLOAT space = fnt->GetFontSize() + 2.0f;

						D2D1_RECT_F bglayout = D2D1::RectF(
							(FLOAT)pos.x() - hwidth,
							(FLOAT)pos.y() + 2,
							(FLOAT)pos.x() + hwidth,
							(FLOAT)pos.y() + space
							);

						D2D1_ROUNDED_RECT rect = { bglayout, 1.5f, 1.5f };
						backbrush->SetOpacity(0.6f);
						target->DrawRoundedRectangle(&rect, backbrush);
						target->FillRoundedRectangle(&rect, backbrush);
						backbrush->SetOpacity(1.0f);
					}
				}

				ID2D1SolidColorBrush* brush = Graphics::locator.getfonts()->getbrush(color);
				if (brush)
				{
					brush->SetOpacity(alpha);
					target->DrawText(
						text.c_str(),
						(UINT32)text.size(),
						fnt,
						layrect,
						brush
						);
					brush->SetOpacity(1.0f);
				}
			}
		}
	}

	uint16_t TextWrapperDW::getadvance(size_t pos) const
	{
		if (advances.count(pos))
		{
			return static_cast<uint16_t>(advances.at(pos));
		}
		else
		{
			return 0;
		}
	}

	size_t TextWrapperDW::getlength() const
	{
		return str.size();
	}

	string TextWrapperDW::gettext() const
	{
		return str;
	}

	vector2d<uint16_t> TextWrapperDW::getdimensions() const
	{
		return vector2d<uint16_t>(width, height);
	}
}
#endif