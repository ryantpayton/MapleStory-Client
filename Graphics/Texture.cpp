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
#include "Texture.h"
#include "GraphicsEngine.h"
#include "Constants.h"
#include "nlnx\nx.hpp"

namespace Graphics
{
	Texture::Texture(node src)
	{
		if (src.data_type() == node::type::bitmap)
		{
			using std::string;
			string link = src["source"];
			if (link != "")
			{
				node srcfile = src;
				while (srcfile != srcfile.root())
				{
					srcfile = srcfile.root();
				}
				src = srcfile.resolve(link.substr(link.find('/') + 1));
			}

			source = src.get_bitmap();
			origin = src["origin"];
			dimensions = Point<int16_t>(
				source.width(), 
				source.height()
				);

			GraphicsEngine::getengine().addbitmap(source);
		}
	}

	Texture::Texture() {}

	Texture::~Texture() {}

	void Texture::draw(const DrawArgument& args) const
	{
		size_t id = source.id();
		if (id == 0)
			return;

		Point<int16_t> stretch = args.getstretch();
		int16_t w = stretch.x();
		if (w == 0)
			w = dimensions.x();
		int16_t h = stretch.y();
		if (h == 0)
			h = dimensions.y();

		Point<int16_t> absp = args.getpos() - origin;
		int16_t x = absp.x();
		int16_t y = absp.y();

		int16_t sy = y + Constants::VIEWYOFFSET;
		if (x < Constants::VIEWWIDTH && sy < Constants::VIEWHEIGHT && x > -w && sy > -h)
		{
			auto& engine = GraphicsEngine::getengine();
			if (!engine.available(id))
			{
				engine.addbitmap(source);
			}
			engine.draw(id, absp.x(), absp.y(), w, h, args.getalpha(), args.getxscale(),
				args.getyscale(), args.getcenter().x(), args.getcenter().y());
		}
	}

	void Texture::shift(Point<int16_t> amount)
	{
		origin -= amount;
	}

	bool Texture::isloaded() const
	{
		return source.id() > 0;
	}

	int16_t Texture::width() const
	{
		return dimensions.x();
	}

	int16_t Texture::height() const
	{
		return dimensions.y();
	}

	Point<int16_t> Texture::getorigin() const
	{
		return origin;
	}

	Point<int16_t> Texture::getdimensions() const
	{
		return dimensions;
	}
}
