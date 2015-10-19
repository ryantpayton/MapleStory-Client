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
#include "Mapinfo.h"

namespace Gameplay
{
	Mapinfo::Mapinfo(node src, vector2d<int> walls, vector2d<int> borders)
	{
		node info = src["info"];
		if (info["VRLeft"].data_type() == node::type::integer)
		{
			mapwalls = vector2d<int>(info["VRLeft"], info["VRRight"]);
			mapborders = vector2d<int>(info["VRTop"], info["VRBottom"]);
		}
		else
		{
			mapwalls = walls;
			mapborders = borders;
		}

		string bgmpath = info["bgm"];
		size_t split = bgmpath.find('/');
		bgm = bgmpath.substr(0, split) + ".img/" + bgmpath.substr(split + 1);

		cloud = info["cloud"].get_bool();
		fieldlimit = info["fieldLimit"];
		hideminimap = info["hideMinimap"].get_bool();
		mapmark = info["mapMark"];
		swim = info["swim"].get_bool();
		town = info["town"].get_bool();

		node stsrc = src["seat"];
		for (node sub = stsrc.begin(); sub != stsrc.end(); ++sub)
		{
			seats.push_back(vector2d<int>(sub.x(), sub.y()));
		}
	}

	string Mapinfo::getbgm() const
	{
		return bgm;
	}

	vector2d<int> Mapinfo::getwalls() const
	{
		return mapwalls;
	}

	vector2d<int> Mapinfo::getborders() const
	{
		return mapborders;
	}

	const vector2d<int>* Mapinfo::getseat(vector2d<int> pos) const
	{
		vector2d<int> hor = vector2d<int>(pos.x() - 10, pos.x() + 10);
		vector2d<int> ver = vector2d<int>(pos.y() - 10, pos.y() + 10);
		for (size_t i = 0; i < seats.size(); i++)
		{
			vector2d<int> stit = seats.at(i);
			if (hor.contains(stit.x()) && ver.contains(stit.y()))
			{
				return &seats.at(i);
			}
		}
		return 0;
	}
}
