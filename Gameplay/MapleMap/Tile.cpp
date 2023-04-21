//////////////////////////////////////////////////////////////////////////////////
//	This file is part of the continued Journey MMORPG client					//
//	Copyright (C) 2015-2019  Daniel Allendorf, Ryan Payton						//
//																				//
//	This program is free software: you can redistribute it and/or modify		//
//	it under the terms of the GNU Affero General Public License as published by	//
//	the Free Software Foundation, either version 3 of the License, or			//
//	(at your option) any later version.											//
//																				//
//	This program is distributed in the hope that it will be useful,				//
//	but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				//
//	GNU Affero General Public License for more details.							//
//																				//
//	You should have received a copy of the GNU Affero General Public License	//
//	along with this program.  If not, see <https://www.gnu.org/licenses/>.		//
//////////////////////////////////////////////////////////////////////////////////
#include "Tile.h"

#ifdef USE_NX
#include <nlnx/nx.hpp>
#endif

#include <iostream>

namespace ms
{
	Tile::Tile(nl::node src, const std::string& ts)
	{
		nl::node dsrc = nl::nx::Map["Tile"][ts][src["u"]][src["no"]];
		nl::node _outlink = dsrc["_outlink"];

		// TODO: Should this be done during the NX conversion?
		if (_outlink)
		{
			size_t pos = 0;
			std::string path = _outlink;
			std::string delimiter = "/";

			std::string file;

			if ((pos = path.find(delimiter)) != std::string::npos)
			{
				file = path.substr(0, pos);
				path.erase(0, pos + delimiter.length());
			}

			if (file == "Map")
				dsrc = nl::nx::Map.resolve(path);
			else
				LOG(LOG_WARN, "Tile::Tile file not handled: " << file);
		}

		texture = Texture(dsrc);
		pos = Point<int16_t>(src["x"], src["y"]);
		z = dsrc["z"];

		if (z == 0)
			z = dsrc["zM"];
	}

	void Tile::draw(Point<int16_t> viewpos) const
	{
		texture.draw(pos + viewpos);
	}

	uint8_t Tile::getz() const
	{
		return z;
	}
}