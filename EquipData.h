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
#include "ItemData.h"
#include "BodyDrawinfo.h"
#include "CharacterData.h"
#include "CharConstants.h"
#include "EquipConstants.h"

namespace Data
{
	using::Character::Equipslot;
	using::Character::Equipstat;
	using::Character::Maplestat;

	class EquipData : public ItemData, public CharacterData
	{
	public:
		EquipData(int32_t, const BodyDrawinfo&);
		EquipData();
		bool istransparent() const;
		const string& gettype() const;
		Equipslot geteqslot() const;
	private:
		string type;
		Equipslot eqslot;
		bool cash;
		bool tradeblock;
		bool transparent;
		int32_t price;
		uint8_t slots;
		map<Maplestat, int16_t> reqstats;
		map<Equipstat, int16_t> defstats;
	};
}

