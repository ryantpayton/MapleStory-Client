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
#include "Stance.h"
#include "Util\Point.h"
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace Character
{
	using std::uint8_t;
	using std::uint16_t;
	using std::string;
	using std::vector;

	// A frame of animation for a skill or similiar 'meta-stance'. 
	// This simply redirects to a different stance and frame to use.
	class BodyAction
	{
	public:
		BodyAction(node src)
		{
			stance = Stance::bystring(src["action"]);
			frame = src["frame"];
			move = src["move"];

			int16_t sgndelay = src["delay"];
			if (sgndelay == 0)
				sgndelay = 100;
			if (sgndelay > 0)
			{
				delay = sgndelay;
				attackframe = true;
			}
			else if (sgndelay < 0)
			{
				delay = -sgndelay;
				attackframe = false;
			}
		}

		BodyAction() {}

		bool isattackframe() const
		{
			return attackframe;
		}

		uint8_t getframe() const
		{
			return frame;
		}

		uint16_t getdelay() const
		{
			return delay;
		}

		Point<int16_t> getmove() const
		{
			return move;
		}

		Stance::Value getstance() const
		{
			return stance;
		}

	private:
		Stance::Value stance;
		uint8_t frame;
		uint16_t delay;
		Point<int16_t> move;
		bool attackframe;
	};

	using std::int16_t;
	using std::unordered_map;

	class BodyDrawinfo
	{
	public:
		BodyDrawinfo();
		~BodyDrawinfo();

		void init();

		Point<int16_t> getbodypos(Stance::Value stance, uint8_t frame) const;
		Point<int16_t> getarmpos(Stance::Value stance, uint8_t frame) const;
		Point<int16_t> gethandpos(Stance::Value stance, uint8_t frame) const;
		Point<int16_t> getheadpos(Stance::Value stance, uint8_t frame) const;
		Point<int16_t> gethairpos(Stance::Value stance, uint8_t frame) const;
		Point<int16_t> getfacepos(Stance::Value stance, uint8_t frame) const;
		uint8_t nextframe(Stance::Value stance, uint8_t frame) const;
		uint16_t getdelay(Stance::Value stance, uint8_t frame) const;

		uint16_t getattackdelay(string action, size_t no) const;
		uint8_t nextacframe(string action, uint8_t frame) const;
		const BodyAction* getaction(string action, uint8_t frame) const;

	private:
		unordered_map<uint8_t, Point<int16_t>> bodyposmap[Stance::LENGTH];
		unordered_map<uint8_t, Point<int16_t>> armposmap[Stance::LENGTH];
		unordered_map<uint8_t, Point<int16_t>> handposmap[Stance::LENGTH];
		unordered_map<uint8_t, Point<int16_t>> headposmap[Stance::LENGTH];
		unordered_map<uint8_t, Point<int16_t>> hairposmap[Stance::LENGTH];
		unordered_map<uint8_t, Point<int16_t>> faceposmap[Stance::LENGTH];
		unordered_map<uint8_t, uint16_t> stancedelays[Stance::LENGTH];

		unordered_map<string, unordered_map<uint8_t, BodyAction>> bodyactions;
		unordered_map<string, vector<uint16_t>> attackdelays;
	};
}