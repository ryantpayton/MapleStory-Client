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
#include "Tileset.h"

namespace Gameplay
{
	Tileset::Tileset(node src)
	{
		for (node sub = src.begin(); sub != src.end(); ++sub)
		{
			if (sub.size() > 0)
			{
				string u = sub.name();
				int no = 0;
				node tilen = sub[to_string(no)];
				while (tilen.size() > 0)
				{
					tiles[u].add(no, new TileData(tilen));
					no++;
					tilen = sub[to_string(no)];
				}
			}
		}
	}

	TileData* Tileset::gettile(string u, int no)
	{
		if (tiles.count(u))
		{
			if (tiles[u].contains(no))
			{
				return tiles[u].get(no);
			}
		}
		return 0;
	}
}
