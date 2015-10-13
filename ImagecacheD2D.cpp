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
#include "ImagecacheD2D.h"

namespace Program
{
	ImagecacheD2D::~ImagecacheD2D()
	{
		clearcache(ICT_SYSTEM);
		clearcache(ICT_LOGIN);
		clearcache(ICT_MAP);
	}

	void ImagecacheD2D::init(IWICImagingFactory* imf)
	{
		imgfactory = imf;
		imgcon = ICT_SYSTEM;
	}

	Imagecontext ImagecacheD2D::createimage(bitmap bmp)
	{
		size_t id = bmp.id();

		if (temp[imgcon][id] || cache[imgcon][id])
		{
			imgcon;
		}
		else
		{
			IWICBitmap* wic = 0;

			imgfactory->CreateBitmapFromMemory(
				bmp.width(), bmp.height(),
				GUID_WICPixelFormat32bppBGRA,
				4 * bmp.width(),
				bmp.length(),
				(BYTE*)bmp.data(),
				&wic);

			if (wic)
			{
				IWICFormatConverter* spConverter = 0;
				int result = imgfactory->CreateFormatConverter(&spConverter);
				if (result == 0)
				{
					spConverter->Initialize(wic,
						GUID_WICPixelFormat32bppPBGRA,
						WICBitmapDitherTypeNone, NULL, 0.f,
						WICBitmapPaletteTypeMedianCut);
					imgfactory->CreateBitmapFromSource(spConverter, WICBitmapNoCache, &temp[imgcon][id]);
					spConverter->Release();
				}
				wic->Release();
			}
		}

		return imgcon;
	}

	void ImagecacheD2D::draw(Imagecontext ict, size_t id, rectangle2d<int32_t> rect, float_t xscale, float_t yscale, vector2d<int32_t> center, float_t alpha)
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

		if (!cache[ict][id])
		{
			target->CreateBitmapFromWicBitmap(temp[ict][id], &cache[ict][id]);
			temp[ict][id]->Release();
			temp[ict].erase(id);

			if (cache[ict][id] == 0)
			{
				cache[ict].erase(id);
			}
		}

		if (cache[ict][id])
		{
			D2D_RECT_F r = D2D1::RectF((FLOAT)rect.l(), (FLOAT)rect.t(), (FLOAT)rect.r(), (FLOAT)rect.b());
			target->DrawBitmap(cache[ict][id], r, alpha);
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

	void ImagecacheD2D::setmode(Imagecontext ict)
	{
		modelock.lock();
		imgcon = ict;
	}

	void ImagecacheD2D::unlock()
	{
		modelock.unlock();
	}

	void ImagecacheD2D::clearcache(Imagecontext ict)
	{
		for (map<size_t, IWICBitmap*>::iterator cit = temp[ict].begin(); cit != temp[ict].end(); ++cit)
		{
			if (cit->second != 0)
			{
				cit->second->Release();
				cit->second = 0;
			}
		}
		temp[ict].clear();

		for (map<size_t, ID2D1Bitmap*>::iterator cit = cache[ict].begin(); cit != cache[ict].end(); ++cit)
		{
			if (cit->second != 0)
			{
				cit->second->Release();
				cit->second = 0;
			}
		}
		cache[ict].clear();
	}
}
