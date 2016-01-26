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
#include "BodyDrawinfo.h"
#include "Character\Equipstat.h"
#include "Character\Maplestat.h"
#include "Character\Inventory\ItemData.h"

namespace Character
{
	using Graphics::DrawArgument;

	class Clothing : public ItemData
	{
	public:
		enum Slot : int16_t
		{
			NONE = 0,
			CAP = 1,
			FACEACC = 2,
			EYEACC = 3,
			EARRINGS = 4,
			TOP = 5,
			PANTS = 6,
			SHOES = 7,
			GLOVES = 8,
			CAPE = 9,
			SHIELD = 10,
			WEAPON = 11,
			RING = 12,
			RING2 = 13,
			RING3 = 15,
			RING4 = 16,
			PENDANT = 17,
			TAMEDMOB = 18,
			SADDLE = 19,
			MEDAL = 49,
			BELT = 50
		};

		static Slot slotbyid(int16_t id)
		{
			if (id < NONE || id > BELT || (id > SADDLE && id < MEDAL) || id == 14)
				return NONE;

			return static_cast<Slot>(id);
		}

		Clothing(int32_t, const BodyDrawinfo&);
		Clothing();

		void draw(string stance, CharacterLayer layer, 
			uint8_t frame, const DrawArgument& args) const;

		bool islayer(string stance, CharacterLayer layer) const;
		bool istransparent() const;
		bool isweapon() const;
		int16_t getreqstat(Maplestat::Value stat) const;
		int16_t getdefstat(Equipstat::Value stat) const;
		string getstatstr(Equipstat::Value stat) const;
		string gettype() const;
		Slot geteqslot() const;

	private:
		map<string, map<CharacterLayer, map<uint8_t, Texture>>> stances;
		string type;
		Slot eqslot;
		bool cash;
		bool tradeblock;
		bool transparent;
		int32_t price;
		uint8_t slots;
		map<Maplestat::Value, int16_t> reqstats;
		map<Equipstat::Value, int16_t> defstats;
	};
}

