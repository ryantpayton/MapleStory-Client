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
#include "Imagecache.h"
#include <d2d1.h>
#include <d2d1helper.h>
#include <wincodecsdk.h>
#include <map>
#include <mutex>

using namespace std;

namespace Program
{
	class ImagecacheD2D : public Imagecache
	{
	public:
		ImagecacheD2D() { target = 0; }
		~ImagecacheD2D();
		void init(IWICImagingFactory*);
		void setmode(Imagecontext);
		void unlock();
		void clearcache(Imagecontext);
		void draw(Imagecontext, size_t, rectangle2d<int32_t>, float_t, float_t, vector2d<int32_t>, float_t);
		Imagecontext createimage(bitmap);
		void settarget(ID2D1RenderTarget* trg) { target = trg; }
		ID2D1RenderTarget* gettarget() { return target; }
	private:
		IWICImagingFactory* imgfactory;
		ID2D1RenderTarget* target;
		map<Imagecontext, map<size_t, IWICBitmap*>> temp;
		map<Imagecontext, map<size_t, ID2D1Bitmap*>> cache;
		Imagecontext imgcon;
		mutex modelock;
	};
}

