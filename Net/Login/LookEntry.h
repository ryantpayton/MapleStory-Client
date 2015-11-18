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
#include "Net\InPacket.h"
#include "Character\EquipConstants.h"
#include <vector>
#include <map>

namespace Net
{
	using std::vector;
	using std::map;
	using Character::Equipslot;

	class LookEntry
	{
	public:
		LookEntry(InPacket&);
		LookEntry(bool, uint8_t, int32_t, int32_t, map<int8_t, int32_t>);
		LookEntry() {}
		~LookEntry() {}

		bool isfemale() const;
		uint8_t getskin() const;
		int32_t getface() const;
		int32_t gethair() const;
		int32_t getequip(Equipslot) const;
		int32_t getpet(size_t) const;

	private:
		bool female;
		uint8_t skin;
		int32_t faceid;
		int32_t hairid;
		map<int8_t, int32_t> equips;
		map<int8_t, int32_t> maskedequips;
		vector<int32_t> petids;
	};
}

