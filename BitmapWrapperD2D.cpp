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
#include "BitmapWrapperD2D.h"

namespace Graphics
{
	BitmapWrapperD2D::BitmapWrapperD2D(bitmap bmp)
	{
		IWICBitmap* wic = 0;

		Program::locator.getfactory()->CreateBitmapFromMemory(
			bmp.width(), bmp.height(),
			GUID_WICPixelFormat32bppBGRA,
			4 * bmp.width(),
			bmp.length(),
			(BYTE*)bmp.data(),
			&wic);

		if (wic != 0)
		{
			IWICFormatConverter* converter = 0;
			int result = Program::locator.getfactory()->CreateFormatConverter(&converter);
			if (result == 0)
			{
				converter->Initialize(wic,
					GUID_WICPixelFormat32bppPBGRA,
					WICBitmapDitherTypeNone, 0, 0.f,
					WICBitmapPaletteTypeMedianCut);
				Program::locator.getfactory()->CreateBitmapFromSource(converter, WICBitmapNoCache, &temp);
				converter->Release();
			}
			wic->Release();
		}
		else
		{
			temp = 0;
		}

		source = 0;
	}

	void BitmapWrapperD2D::draw(rectangle2d<int32_t> rect, float_t xscale, float_t yscale, vector2d<int32_t> center, float_t alpha)
	{
		ID2D1BitmapRenderTarget* target = Program::locator.gettarget();
		if (target)
		{
			bool transform = (xscale != 1.0f) || (yscale != 1.0f);

			if (transform)
			{
				target->SetTransform(
					D2D1::Matrix3x2F::Scale(
					D2D1::Size(xscale, yscale),
					D2D1::Point2F(
					static_cast<FLOAT>(center.x()),
					static_cast<FLOAT>(center.y())
					)));
			}

			if (source == 0 && temp != 0)
			{
				target->CreateBitmapFromWicBitmap(temp, &source);
				temp->Release();
				temp = 0;

				if (source == 0)
				{
					source->Release();
					source = 0;
				}
			}

			if (source != 0)
			{
				D2D_RECT_F r = D2D1::RectF((FLOAT)rect.l(), (FLOAT)rect.t(), (FLOAT)rect.r(), (FLOAT)rect.b());
				target->DrawBitmap(source, r, alpha);
			}

			if (transform)
			{
				target->SetTransform(
					D2D1::Matrix3x2F::Scale(
					D2D1::Size(1.0f, 1.0f),
					D2D1::Point2F(
					static_cast<FLOAT>(center.x()),
					static_cast<FLOAT>(center.y())
					)));
			}
		}
	}

	BitmapWrapperD2D::~BitmapWrapperD2D()
	{
		if (temp != 0)
		{
			temp->Release();
			temp = 0;
		}

		if (source != 0)
		{
			source->Release();
			source = 0;
		}
	}
}