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

		for (node sub : src["seat"])
		{
			seats.push_back({ sub });
		}

		for (node lrnode : src["ladderRope"])
		{
			Ladder ladder;
			ladder.x = lrnode["x"];
			ladder.y1 = lrnode["y1"];
			ladder.y2 = lrnode["y2"];
			ladder.ladder = lrnode["l"].get_bool();
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
			Seat seat;
			seat.pos = recv.readpoint();
			seats.push_back(seat);
		}

		uint16_t numladders = recv.readshort();
		for (uint16_t i = 0; i < numladders; i++)
		{
			Ladder ladder;
			ladder.x = recv.readshort();
			ladder.y1 = recv.readshort();
			ladder.y2 = recv.readshort();
			ladder.ladder = recv.readbool();
			ladders.push_back(ladder);
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

	const Seat* MapInfo::findseat(Point<int16_t> pos) const
	{
		Range<int16_t> hor = Range<int16_t>(pos.x() - 10, pos.x() + 10);
		Range<int16_t> ver = Range<int16_t>(pos.y() - 10, pos.y() + 10);
		for (auto& stit : seats)
		{
			if (hor.contains(stit.pos.x()) && ver.contains(stit.pos.y()))
			{
				return &stit;
			}
		}
		return nullptr;
	}

	const Ladder* MapInfo::findladder(Point<int16_t> pos, bool up) const
	{
		Range<int16_t> hor = Range<int16_t>(pos.x() - 25, pos.x() + 25);
		for (auto& lrit : ladders)
		{
			Range<int16_t> lrver;
			if (up)
				lrver = Range<int16_t>(lrit.y1, lrit.y2 + 15);
			else
				lrver = Range<int16_t>(lrit.y1 - 5, lrit.y2);

			if (hor.contains(lrit.x) && lrver.contains(pos.y()))
			{
				return &lrit;
			}
		}
		return nullptr;
	}
}
