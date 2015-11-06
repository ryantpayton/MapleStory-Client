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
#include "Texture.h"
#include "nx.hpp"

#include "Journey.h"
#ifdef JOURNEY_USE_OPENGL
#include "BitmapWrapperGL.h"
#else
#include "BitmapWrapperD2D.h"
#endif

namespace Graphics
{
	Texture::Texture(node src)
	{
		if (src.data_type() == node::type::bitmap)
		{
			node orgnode = src["origin"];
			if (orgnode.data_type() == node::type::vector)
			{
				origin = vector2d<int32_t>(orgnode.x(), orgnode.y());
			}
			else
			{
				origin = vector2d<int32_t>();
			}

			if (src["source"].data_type() == node::type::string)
			{
				using::std::string;
				string link = src["source"].get_string();
				string file = link.substr(0, link.find('/'));
				link = link.substr(link.find('/') + 1);
				src = nl::nx::base[file].resolve(link);
			}

			nl::bitmap bmp = src.get_bitmap();
#ifdef JOURNEY_USE_OPENGL
			source = new BitmapWrapperGL(bmp);
#else
			source = new BitmapWrapperD2D(bmp);
#endif
			dimensions = vector2d<int32_t>(bmp.width(), bmp.height());
			shift = vector2d<int32_t>();

			loaded = true;
		}
		else
		{
			loaded = false;
		}
	}

	Texture::~Texture() 
	{
		if (loaded)
		{
			delete source;
		}
	}

	void Texture::draw(const DrawArgument& args) const
	{
		if (loaded)
		{
			vector2d<int32_t> stretch = args.getstretch();
			int32_t w = stretch.x();
			if (w == 0)
				w = dimensions.x();
			int32_t h = stretch.y();
			if (h == 0)
				h = dimensions.y();

			vector2d<int32_t> absp = args.getpos() - origin + shift;
			if (absp.x() <= 816 && absp.y() <= 624 && absp.x() > -w && absp.y() > -h)
			{
				source->draw(rectangle2d<int32_t>(absp, absp + vector2d<int32_t>(w, h)), args.getxscale(), args.getyscale(), args.getcenter(), args.getalpha());
			}
		}
	}

	void Texture::setshift(vector2d<int32_t> shf)
	{
		shift = shf;
	}

	vector2d<int32_t> Texture::getorigin() const
	{
		return origin;
	}

	vector2d<int32_t> Texture::getdimensions() const
	{
		return dimensions;
	}
}
