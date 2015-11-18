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
#include "Graphics\Texture.h"
#include "nlnx\nx.hpp"
#include "nlnx\bitmap.hpp"

#include "Journey.h"
#ifdef JOURNEY_USE_OPENGL
#include "Graphics\GraphicsGL.h"
#else
#include "Graphics\GraphicsD2D.h"
#endif

namespace Graphics
{
	Texture::Texture(node src)
	{
		if (src.data_type() == node::type::bitmap)
		{
			if (src["source"].data_type() == node::type::string)
			{
				using::std::string;
				string link = src["source"].get_string();
				string file = link.substr(0, link.find('/'));
				link = link.substr(link.find('/') + 1);
				src = nl::nx::base[file].resolve(link);
			}
			source = src;
			origin = vector2d<int16_t>(source["origin"]);
			dimensions = vector2d<int16_t>(source.get_bitmap().width(), source.get_bitmap().height());

#ifdef JOURNEY_USE_OPENGL
			GraphicsGL::addbitmap(source.get_bitmap());
#else
			GraphicsD2D::addbitmap(source.get_bitmap());
#endif

		}
	}

	Texture::Texture() {}

	Texture::~Texture() {}

	void Texture::draw(const DrawArgument& args) const
	{
		size_t id = source.get_bitmap().id();
		if (id > 0)
		{
			vector2d<int16_t> stretch = args.getstretch();
			int16_t w = stretch.x();
			if (w == 0)
				w = dimensions.x();
			int16_t h = stretch.y();
			if (h == 0)
				h = dimensions.y();

			vector2d<int16_t> absp = args.getpos() - origin + shift;

			if (absp.x() <= 816 && absp.y() <= 624 && absp.x() > -w && absp.y() > -h)
			{

#ifdef JOURNEY_USE_OPENGL
				if (!GraphicsGL::available(id))
				{
					GraphicsGL::addbitmap(source.get_bitmap());
				}
				GraphicsGL::draw(id, rectangle2d<int16_t>(absp.x(), absp.x() + w, absp.y(), absp.y() + h),
					args.getxscale(), args.getyscale(), args.getcenter(), args.getalpha());
#else
				if (!GraphicsD2D::available(id))
				{
					GraphicsD2D::addbitmap(source.get_bitmap());
				}
				GraphicsD2D::draw(id, absp.x(), absp.y(), w, h, args.getalpha(), args.getxscale(),
					args.getyscale(), args.getcenter().x(), args.getcenter().y());
#endif

			}
		}
	}

	void Texture::setshift(vector2d<int16_t> shf)
	{
		shift = shf;
	}

	vector2d<int16_t> Texture::getorigin() const
	{
		return origin;
	}

	vector2d<int16_t> Texture::getdimensions() const
	{
		return dimensions;
	}
}
