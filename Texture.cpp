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
#include "Texture.h"
#include "nx.hpp"
#include "BitmapWrapperD2D.h"

namespace Graphics
{
	Texture::Texture(node src)
	{
		if (src.data_type() == node::type::bitmap)
		{
			node orgnode = src["origin"];
			if (orgnode.data_type() == node::type::vector)
			{
				origin = vector2d<int>(orgnode.x(), orgnode.y());
			}
			else
			{
				origin = vector2d<int>();
			}

			if (src["source"].data_type() == node::type::string)
			{
				string link = src["source"].get_string();
				string file = link.substr(0, link.find('/'));
				link = link.substr(link.find('/') + 1);
				src = nx::base[file].resolve(link);
			}

			bitmap bmp = src.get_bitmap();
			source = new BitmapWrapperD2D(bmp);
			dimension = vector2d<int>(bmp.width(), bmp.height());
			shift = vector2d<int>();

			loaded = true;
		}
		else
		{
			loaded = false;
		}
	}

	void Texture::draw(DrawArgument& args)
	{
		if (loaded)
		{
			vector2d<int> stretch = args.getstretch();
			int w = stretch.x();
			if (w == 0)
				w = dimension.x();
			int h = stretch.y();
			if (h == 0)
				h = dimension.y();

			vector2d<int> absp = args.getpos() - origin + shift;
			if (absp.x() <= 816 && absp.y() <= 624 && absp.x() > -w && absp.y() > -h)
			{
				source->draw(rectangle2d<int>(absp, absp + vector2d<int>(w, h)), args.getxscale(), args.getyscale(), args.getcenter(), args.getalpha());
			}
		}
	}
}
