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
#include "MapBackgrounds.h"

#include "nlnx\nx.hpp"

namespace Gameplay
{
	Background::Background(node src, vector2d<int16_t> wl, vector2d<int16_t> br)
	{
		node backsrc = nl::nx::map["Back"];
		animated = src["ani"].get_bool();
		animation = Animation(backsrc[src["bS"] + ".img"][animated ? "ani" : "back"][src["no"]]);
		rpos = vector2d<int16_t>(src["rx"], src["ry"]);
		cpos = vector2d<int16_t>(src["cx"], src["cy"]);
		type = typebyid(src["type"]);
		opacity = src["a"];
		fx = src["x"];
		fy = src["y"];
		flipped = src["f"].get_bool();

		if (cpos.x() == 0)
			cpos.setx(animation.getdimensions().x());
		if (cpos.y() == 0)
			cpos.sety(animation.getdimensions().y());

		walls = wl;
		borders = br;
	}

	void Background::draw(vector2d<int16_t> position, float inter) const
	{
		int16_t htile = 1;
		int16_t vtile = 1;

		int16_t xcom = -position.x() - walls.x() + 400;
		int16_t ycom = -position.y() + borders.y() - 300;
		position.setx(rpos.x() * xcom / 100 + 400);
		position.sety(rpos.y() * ycom / 100 + 300);

		switch (type)
		{
		case HMOVEA:
		case HTILED:
			htile += walls.length() / cpos.x();
			break;
		case VMOVEA:
		case VTILED:
			vtile += borders.length() / cpos.y();
			break;
		case TILED:
			htile += walls.length() / cpos.x();
			vtile += borders.length() / cpos.y();
			break;
		}

		int16_t starty = position.y();
		for (int16_t i = 0; i < htile; i++)
		{
			for (int16_t j = 0; j < vtile; j++)
			{
				using Graphics::DrawArgument;
				animation.draw(DrawArgument(position, opacity / 255), inter);

				position.shifty(cpos.y());
			}

			position.sety(starty);
			position.shiftx(cpos.x());
		}
	}

	void Background::update()
	{
		animation.update();
	}


	MapBackgrounds::MapBackgrounds(node src, vector2d<int16_t> walls, vector2d<int16_t> borders)
	{
		for (auto backnode : src)
		{
			if (backnode["bS"].get_string() == "")
				continue;

			Background background = Background(backnode, walls, borders);
			bool front = backnode["front"].get_bool();
			if (front)
			{
				foregrounds.push_back(background);
			}
			else
			{
				backgrounds.push_back(background);
			}
		}
	}

	MapBackgrounds::MapBackgrounds() {}

	void MapBackgrounds::drawbackgrounds(vector2d<int16_t> position, float inter) const
	{
		for (auto& background : backgrounds)
		{
			background.draw(position, inter);
		}
	}

	void MapBackgrounds::drawforegrounds(vector2d<int16_t> position, float inter) const
	{
		for (auto& foreground : foregrounds)
		{
			foreground.draw(position, inter);
		}
	}

	void MapBackgrounds::update()
	{
		for (auto& background : backgrounds)
		{
			background.update();
		}

		for (auto& foreground : foregrounds)
		{
			foreground.update();
		}
	}
}