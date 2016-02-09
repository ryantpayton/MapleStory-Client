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
#include "Util\vector2d.h"
#include <cstdint>
#include <string>
#include <unordered_map>

namespace Character
{
	using std::uint8_t;
	using std::uint16_t;
	using std::string;

	// A frame of animation for a skill or similiar 'meta-stance'. 
	// This simply redirects to a different stance and frame to use.
	class BodyAction
	{
	public:
		BodyAction(Stance::Value s, uint8_t f, uint16_t d)
		{
			stance = s;
			frame = f;
			delay = d;
		}

		BodyAction() {}

		Stance::Value getstance() const
		{
			return stance;
		}

		uint8_t getframe() const
		{
			return frame;
		}

		uint16_t getdelay() const
		{
			return delay;
		}

	private:
		Stance::Value stance;
		uint8_t frame;
		uint16_t delay;
	};

	using std::int16_t;
	using std::unordered_map;

	class BodyDrawinfo
	{
	public:
		BodyDrawinfo();
		~BodyDrawinfo();

		void init();

		vector2d<int16_t> getbodypos(Stance::Value stance, uint8_t frame) const;
		vector2d<int16_t> getarmpos(Stance::Value stance, uint8_t frame) const;
		vector2d<int16_t> gethandpos(Stance::Value stance, uint8_t frame) const;
		vector2d<int16_t> getheadpos(Stance::Value stance, uint8_t frame) const;
		vector2d<int16_t> gethairpos(Stance::Value stance, uint8_t frame) const;
		vector2d<int16_t> getfacepos(Stance::Value stance, uint8_t frame) const;
		uint8_t nextframe(Stance::Value stance, uint8_t frame) const;
		uint16_t getdelay(Stance::Value stance, uint8_t frame) const;
		uint8_t nextacframe(string action, uint8_t frame) const;
		const BodyAction* getaction(string action, uint8_t frame) const;

	private:
		unordered_map<uint8_t, vector2d<int16_t>> bodyposmap[Stance::LENGTH];
		unordered_map<uint8_t, vector2d<int16_t>> armposmap[Stance::LENGTH];
		unordered_map<uint8_t, vector2d<int16_t>> handposmap[Stance::LENGTH];
		unordered_map<uint8_t, vector2d<int16_t>> headposmap[Stance::LENGTH];
		unordered_map<uint8_t, vector2d<int16_t>> hairposmap[Stance::LENGTH];
		unordered_map<uint8_t, vector2d<int16_t>> faceposmap[Stance::LENGTH];
		unordered_map<uint8_t, uint16_t> stancedelays[Stance::LENGTH];
		unordered_map<string, unordered_map<uint8_t, BodyAction>> bodyactions;
	};
}