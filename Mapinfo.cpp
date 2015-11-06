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
#include "Mapinfo.h"

namespace Gameplay
{
	Mapinfo::Mapinfo()
	{
		bgm = "";
		newbgm = true;
	}

	void Mapinfo::loadinfo(node src, vector2d<int32_t> walls, vector2d<int32_t> borders)
	{
		string oldbgm = bgm;

		node info = src["info"];
		if (info["VRLeft"].data_type() == node::type::integer)
		{
			mapwalls = vector2d<int32_t>(info["VRLeft"], info["VRRight"]);
			mapborders = vector2d<int32_t>(info["VRTop"], info["VRBottom"]);
		}
		else
		{
			mapwalls = walls;
			mapborders = borders;
		}

		string bgmpath = info["bgm"];
		size_t split = bgmpath.find('/');
		bgm = bgmpath.substr(0, split) + ".img/" + bgmpath.substr(split + 1);
		if (bgm != oldbgm)
		{
			newbgm = true;
		}
		else
		{
			newbgm = false;
		}

		cloud = info["cloud"].get_bool();
		fieldlimit = info["fieldLimit"];
		hideminimap = info["hideMinimap"].get_bool();
		mapmark = info["mapMark"];
		swim = info["swim"].get_bool();
		town = info["town"].get_bool();

		node stsrc = src["seat"];
		for (node sub = stsrc.begin(); sub != stsrc.end(); ++sub)
		{
			seats.push_back(vector2d<int32_t>(sub.x(), sub.y()));
		}
	}

	bool Mapinfo::hasnewbgm() const
	{
		return newbgm;
	}

	const string& Mapinfo::getbgm() const
	{
		return bgm;
	}

	vector2d<int32_t> Mapinfo::getwalls() const
	{
		return mapwalls;
	}

	vector2d<int32_t> Mapinfo::getborders() const
	{
		return mapborders;
	}

	const vector2d<int32_t>* Mapinfo::getseat(vector2d<int32_t> pos) const
	{
		vector2d<int32_t> hor = vector2d<int32_t>(pos.x() - 10, pos.x() + 10);
		vector2d<int32_t> ver = vector2d<int32_t>(pos.y() - 10, pos.y() + 10);
		for (size_t i = 0; i < seats.size(); i++)
		{
			vector2d<int32_t> stit = seats.at(i);
			if (hor.contains(stit.x()) && ver.contains(stit.y()))
			{
				return &seats.at(i);
			}
		}
		return 0;
	}
}
