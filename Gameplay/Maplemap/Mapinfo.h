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
#pragma once
#include "Util\Point.h"
#include "Util\Range.h"
#include "Util\Optional.h"
#include "Net\InPacket.h"
#include "nlnx\node.hpp"
#include <vector>

namespace Gameplay
{
	using std::vector;
	using std::string;
	using nl::node;
	using Net::InPacket;

	class Seat
	{
	public:
		Seat(node source);
		Seat(InPacket& recv);

		bool inrange(Point<int16_t> position) const;
		Point<int16_t> getpos() const;

	private:
		Point<int16_t> pos;
	};

	class Ladder
	{
	public:
		Ladder(node source);
		Ladder(InPacket& recv);

		bool isladder() const;
		bool inrange(Point<int16_t> position, bool upwards) const;
		bool felloff(int16_t y, bool downwards) const;
		int16_t getx() const;

	private:
		int16_t x;
		int16_t y1;
		int16_t y2;
		bool ladder;
	};

	class MapInfo
	{
	public:
		MapInfo(node src, Range<int16_t> walls, Range<int16_t> borders);
		MapInfo(InPacket& recv);
		MapInfo();
		~MapInfo();

		bool isswimmap() const;
		string getbgm() const;
		Range<int16_t> getwalls() const;
		Range<int16_t> getborders() const;

		// Find a setat the player's position.
		Optional<const Seat> findseat(Point<int16_t> position) const;
		// Find a ladder at the player's position. upwards = false implies downwards.
		Optional<const Ladder> findladder(Point<int16_t> position, bool upwards) const;

	private:
		int32_t fieldlimit;
		bool cloud;
		string bgm;
		string mapdesc;
		string mapname;
		string streetname;
		string mapmark;
		bool swim;
		bool town;
		bool hideminimap;
		Range<int16_t> mapwalls;
		Range<int16_t> mapborders;
		vector<Seat> seats;
		vector<Ladder> ladders;
	};
}