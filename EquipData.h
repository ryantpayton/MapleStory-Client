/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015 SYJourney                                               //
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

using namespace Constants;

namespace Data
{
	class EquipData : public ItemData, public CharacterData
	{
	public:
		EquipData(int, BodyDrawinfo&);
		EquipData();
		bool istransparent() const;
		string gettype() const;
		Equipslot geteqslot() const;
	private:
		string type;
		Equipslot eqslot;
		bool cash;
		bool tradeblock;
		bool transparent;
		int price;
		uint8_t slots;
		map<Maplestat, short> reqstats;
		map<Equipstat, short> defstats;
	};
}

