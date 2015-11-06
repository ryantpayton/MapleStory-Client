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
#include "BodyAction.h"
#include "vector2d.h"
#include <map>

namespace Data
{
	using::std::int32_t;
	using::std::map;
	using::Util::vector2d;

	class BodyDrawinfo
	{
	public:
		BodyDrawinfo();
		~BodyDrawinfo();
		void init();
		vector2d<int32_t> getbodypos(string, uint8_t) const;
		vector2d<int32_t> getarmpos(string, uint8_t) const;
		vector2d<int32_t> gethandpos(string, uint8_t) const;
		vector2d<int32_t> getheadpos(string, uint8_t) const;
		vector2d<int32_t> gethairpos(string, uint8_t) const;
		vector2d<int32_t> getfacepos(string, uint8_t) const;
		uint8_t nextframe(string, uint8_t) const;
		uint16_t getdelay(string, uint8_t) const;
		uint8_t nextacframe(string, uint8_t) const;
		const BodyAction* getaction(string, uint8_t) const;
	private:
		map<string, map<uint8_t, vector2d<int32_t>>> bodyposmap;
		map<string, map<uint8_t, vector2d<int32_t>>> armposmap;
		map<string, map<uint8_t, vector2d<int32_t>>> handposmap;
		map<string, map<uint8_t, vector2d<int32_t>>> headposmap;
		map<string, map<uint8_t, vector2d<int32_t>>> hairposmap;
		map<string, map<uint8_t, vector2d<int32_t>>> faceposmap;
		map<string, map<uint8_t, uint16_t>> stancedelays;
		map<string, map<uint8_t, BodyAction>> bodyactions;
	};
}