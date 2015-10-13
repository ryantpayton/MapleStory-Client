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
#include "TextWrapperDW.h"

namespace Graphics
{
	TextWrapperDW::TextWrapperDW(Font f, Textcolor c, string s, short m)
	{
		font = Program::locator.getfonts()->getfont(f);
		color = c;
		settext(s, m);

		brush = 0;
		backbrush = 0;
		alpha = 1.0f;
		height = 0;
		width = 0;
		back = TXB_NONE;
	}

	TextWrapperDW::~TextWrapperDW()
	{
		if (brush)
		{
			brush->Release();
		}
		if (backbrush)
		{
			backbrush->Release();
		}
	}

	void TextWrapperDW::draw(vector2d<int> pos)
	{
		ID2D1RenderTarget* target = Program::locator.gettarget();

		if (target && font)
		{
			pos.shifty(5);

			if (pos.x() + width >= 0 && pos.x() - width <= 816 && pos.y() + height >= 0 && pos.y() - height <= 624 && text.size() > 0)
			{
				short lrwidth = width + 5;
				D2D1_RECT_F layrect;
				switch (font->GetTextAlignment())
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

				if (!brush)
				{
					switch (color)
					{
					case TXC_WHITE:
						target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brush);
						break;
					case TXC_BLACK:
						target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &brush);
						break;
					case TXC_YELLOW:
						target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &brush);
						break;;
					case TXC_RED:
						target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &brush);
						break;
					case TXC_BLUE:
						target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue), &brush);
						break;
					case TXC_BROWN:
						target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Brown), &brush);
						break;
					case TXC_GREY:
						target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gray), &brush);
						break;
					case TXC_ORANGE:
						target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Orange), &brush);
						break;
					case TXC_MBLUE:
						target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::SkyBlue), &brush);
						break;
					case TXC_VIOLET:
						target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Violet), &brush);
						break;
					}
				}

				if (!backbrush)
				{
					switch (back)
					{
					case TXB_NAMETAG:
						target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &backbrush);
						backbrush->SetOpacity(0.6f);
						break;
					case TXB_GMCHAT:
						target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::WhiteSmoke), &backbrush);
						backbrush->SetOpacity(0.8f);
						break;
					}
				}

				if (backbrush)
				{
					if (back == TXB_NAMETAG)
					{
						FLOAT hwidth = ((FLOAT)lrwidth) / 2;
						FLOAT space = font->GetFontSize() + 2.0f;

						D2D1_RECT_F bglayout = D2D1::RectF(
							(FLOAT)pos.x() - hwidth,
							(FLOAT)pos.y() + 2,
							(FLOAT)pos.x() + hwidth,
							(FLOAT)pos.y() + space
							);

						D2D1_ROUNDED_RECT rect = { bglayout, 1.5f, 1.5f };

						target->DrawRoundedRectangle(&rect, backbrush);
						target->FillRoundedRectangle(&rect, backbrush);
					}
				}

				if (brush && target)
				{
					brush->SetOpacity(alpha);

					target->DrawText(
						text.c_str(),
						(UINT32)text.size(),
						font,
						layrect,
						brush
						);
				}
			}
		}
	}

	void TextWrapperDW::settext(string t, short wmax)
	{
		if (font)
		{
			text = wstring(t.begin(), t.end());
			int totlen = createlayout(wmax);
			float space = font->GetFontSize() * 1.25f;
			if (totlen > wmax && wmax > 0)
			{
				width = wmax;
				height = static_cast<short>(space)* ((totlen / wmax) + 1);
				endpos = vector2d<short>(totlen % wmax, height - static_cast<short>(font->GetFontSize()));
			}
			else
			{
				width = totlen;
				height = static_cast<short>(space);
				endpos = vector2d<short>(totlen, 0);
			}
		}
	}

	int TextWrapperDW::createlayout(short wmax)
	{
		int total = 0;
		IDWriteFactory* dwfactory = Program::locator.getfonts()->getdwfactory();
		if (dwfactory && text.size() > 0)
		{
			if (wmax < 1)
			{
				wmax = 816;
			}

			IDWriteTextLayout* layout = 0;
			dwfactory->CreateTextLayout(
				text.c_str(),
				(UINT32)text.length(),
				font,
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
				total = static_cast<int>(tadv);
			}
		}
		return total;
	}

	short TextWrapperDW::getadvance(size_t pos)
	{
		if (advances.count(pos))
		{
			return static_cast<short>(advances[pos]);
		}
		else
		{
			return 0;
		}
	}
}
