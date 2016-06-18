/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015-2016 Daniel Allendorf                                   //
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

#include "..\..\Util\Point.h"

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace jrc
{
	// A frame of animation for a skill or similiar 'meta-stance'. 
	// This simply redirects to a different stance and frame to use.
	class BodyAction
	{
	public:
		BodyAction(nl::node src)
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

		Point<int16_t> get_move() const
		{
			return move;
		}

		Stance::Value get_stance() const
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


	class BodyDrawinfo
	{
	public:
		void init();

		Point<int16_t> getbodypos(Stance::Value stance, uint8_t frame) const;
		Point<int16_t> getarmpos(Stance::Value stance, uint8_t frame) const;
		Point<int16_t> gethandpos(Stance::Value stance, uint8_t frame) const;
		Point<int16_t> get_headpos(Stance::Value stance, uint8_t frame) const;
		Point<int16_t> gethairpos(Stance::Value stance, uint8_t frame) const;
		Point<int16_t> getfacepos(Stance::Value stance, uint8_t frame) const;
		uint8_t nextframe(Stance::Value stance, uint8_t frame) const;
		uint16_t getdelay(Stance::Value stance, uint8_t frame) const;

		uint16_t get_attackdelay(std::string action, size_t no) const;
		uint8_t nextacframe(std::string action, uint8_t frame) const;
		const BodyAction* getaction(std::string action, uint8_t frame) const;

	private:
		std::unordered_map<uint8_t, Point<int16_t>> bodyposmap[Stance::LENGTH];
		std::unordered_map<uint8_t, Point<int16_t>> armposmap[Stance::LENGTH];
		std::unordered_map<uint8_t, Point<int16_t>> handposmap[Stance::LENGTH];
		std::unordered_map<uint8_t, Point<int16_t>> headposmap[Stance::LENGTH];
		std::unordered_map<uint8_t, Point<int16_t>> hairposmap[Stance::LENGTH];
		std::unordered_map<uint8_t, Point<int16_t>> faceposmap[Stance::LENGTH];
		std::unordered_map<uint8_t, uint16_t> stancedelays[Stance::LENGTH];

		std::unordered_map<std::string, std::unordered_map<uint8_t, BodyAction>> bodyactions;
		std::unordered_map<std::string, std::vector<uint16_t>> attackdelays;
	};
}