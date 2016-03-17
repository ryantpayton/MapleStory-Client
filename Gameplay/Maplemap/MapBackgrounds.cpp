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
#include "Constants.h"
#include "Graphics\GraphicsGL.h"
#include "nlnx\nx.hpp"

namespace Gameplay
{
	Background::Background(node src)
	{
		node backsrc = nl::nx::map["Back"];
		animated = src["ani"].get_bool();
		animation = backsrc[src["bS"] + ".img"][animated ? "ani" : "back"][src["no"]];
		opacity = src["a"];
		flipped = src["f"].get_bool();
		cx = src["cx"];
		cy = src["cy"];
		rx = src["rx"];
		ry = src["ry"];
		moveobj.setx(src["x"]);
		moveobj.sety(src["y"]);

		Type type = typebyid(src["type"]);
		settype(type);
	}

	Background::Background(InPacket& recv)
	{
		node backsrc = nl::nx::map["Back"];
		animation = backsrc[recv.readascii()];
		opacity = recv.readshort();
		flipped = recv.readbool();
		cx = recv.readshort();
		cy = recv.readshort();
		rx = recv.readshort();
		ry = recv.readshort();
		moveobj.setx(recv.readshort());
		moveobj.sety(recv.readshort());

		Type type = typebyid(recv.readbyte());
		settype(type);
	}

	void Background::settype(Type type)
	{
		if (cx == 0)
			cx = animation.getdimensions().x();
		if (cy == 0)
			cy = animation.getdimensions().y();

		htile = 1;
		vtile = 1;
		switch (type)
		{
		case HTILED:
		case HMOVEA:
			htile = Constants::VIEWWIDTH / cx + 3;
			break;
		case VTILED:
		case VMOVEA:
			vtile = Constants::VIEWHEIGHT / cy + 3;
			break;
		case TILED:
		case HMOVEB:
		case VMOVEB:
			htile = Constants::VIEWWIDTH / cx + 3;
			vtile = Constants::VIEWHEIGHT / cy + 3;
			break;
		}

		switch (type)
		{
		case HMOVEA:
		case HMOVEB:
			moveobj.hspeed = rx / 16;
			break;
		case VMOVEA:
		case VMOVEB:
			moveobj.vspeed = ry / 16;
			break;
		}
	}

	void Background::draw(Point<int16_t> position, float inter) const
	{
		int16_t shiftx = static_cast<int16_t>(rx * (-position.x() + WOFFSET) / 100 + WOFFSET);
		int16_t shifty = static_cast<int16_t>(ry * (-position.y() + HOFFSET) / 100 + HOFFSET);
		int16_t x = moveobj.getx(inter);
		int16_t y = moveobj.gety(inter);
		
		if (moveobj.hspeed != 0.0)
		{
			x += position.x();
			y += shifty;
		}
		else if (moveobj.vspeed != 0.0)
		{
			x += shiftx;
			y += position.y();
		}
		else
		{
			x += shiftx;
			y += shifty;
		}

		if (htile > 1)
		{
			while (x > 0)
			{
				x -= cx;
			}
			while (x < -cx)
			{
				x += cx;
			}
		}

		if (vtile > 1)
		{
			while (y > 0)
			{
				y -= cy;
			}
			while (y < -cy)
			{
				y += cy;
			}
		}

		int16_t endx = x + cx * htile;
		int16_t endy = y + cy * vtile;
		for (int16_t tx = x; tx < endx; tx += cx)
		{
			for (int16_t ty = y; ty < endy; ty += cy)
			{
				using Graphics::DrawArgument;
				animation.draw(DrawArgument(Point<int16_t>(tx, ty), flipped, opacity / 255), inter);
			}
		}
	}

	void Background::update()
	{
		moveobj.move();
		animation.update();
	}


	MapBackgrounds::MapBackgrounds(node src)
	{
		int16_t no = 0;
		node back = src[std::to_string(no)];
		while (back.size() > 0)
		{
			bool front = back["front"].get_bool();
			if (front)
			{
				foregrounds.push_back(back);
			}
			else
			{
				backgrounds.push_back(back);
			}

			no++;
			back = src[std::to_string(no)];
		}

		black = src["0"]["bS"].get_string() == "";
	}

	MapBackgrounds::MapBackgrounds(InPacket& recv)
	{
		int16_t size = recv.readshort();
		for (int16_t i = 0; i < size; i++)
		{
			bool front = recv.readbool();
			if (front)
			{
				foregrounds.push_back(recv);
			}
			else
			{
				backgrounds.push_back(recv);
			}
		}

		black = recv.readbool();
	}

	MapBackgrounds::MapBackgrounds() {}

	void MapBackgrounds::drawbackgrounds(Point<int16_t> position, float inter) const
	{
		if (black)
		{
			using Graphics::GraphicsGL;
			GraphicsGL::get().drawscreenfill(0.0f, 0.0f, 0.0f, 1.0f);
		}

		for (auto& background : backgrounds)
		{
			background.draw(position, inter);
		}
	}

	void MapBackgrounds::drawforegrounds(Point<int16_t> position, float inter) const
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