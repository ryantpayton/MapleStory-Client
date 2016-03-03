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
	MapInfo::MapInfo(node src, Range<int16_t> walls, Range<int16_t> borders)
	{
		node info = src["info"];
		if (info["VRLeft"].data_type() == node::type::integer)
		{
			mapwalls = Range<int16_t>(info["VRLeft"], info["VRRight"]);
			mapborders = Range<int16_t>(info["VRTop"], info["VRBottom"]);
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

		for (node seat : src["seat"])
		{
			seats.push_back(seat);
		}

		for (node ladder : src["ladderRope"])
		{
			ladders.push_back(ladder);
		}
	}

	MapInfo::MapInfo(InPacket& recv)
	{
		mapwalls.setfirst(recv.readshort());
		mapwalls.setsecond(recv.readshort());
		mapborders.setfirst(recv.readshort());
		mapborders.setsecond(recv.readshort());
		bgm = recv.readascii();
		cloud = recv.readbool();
		fieldlimit = recv.readint();
		hideminimap = recv.readbool();
		mapmark = recv.readascii();
		swim = recv.readbool();
		town = recv.readbool();

		uint16_t numseats = recv.readshort();
		for (uint16_t i = 0; i < numseats; i++)
		{
			seats.push_back(recv);
		}

		uint16_t numladders = recv.readshort();
		for (uint16_t i = 0; i < numladders; i++)
		{
			ladders.push_back(recv);
		}
	}

	MapInfo::MapInfo() {}

	MapInfo::~MapInfo() {}

	bool MapInfo::isswimmap() const
	{
		return swim;
	}

	string MapInfo::getbgm() const
	{
		return bgm;
	}

	Range<int16_t> MapInfo::getwalls() const
	{
		return mapwalls;
	}

	Range<int16_t> MapInfo::getborders() const
	{
		return mapborders;
	}

	Optional<const Seat> MapInfo::findseat(Point<int16_t> position) const
	{
		return Optional<Seat>::findfirst(seats, &Seat::inrange, position);
	}

	Optional<const Ladder> MapInfo::findladder(Point<int16_t> position, bool upwards) const
	{
		return Optional<Ladder>::findfirst(ladders, &Ladder::inrange, position, upwards);
	}
}
