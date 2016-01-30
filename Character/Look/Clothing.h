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
#include "Graphics\Texture.h"
#include "Util\Enum.h"
#include "Util\Console.h"

namespace Character
{
	class Slot
	{
	public:
		static const int16_t LENGTH = 51;
		enum Value : int16_t
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

		static Value byid(int16_t id)
		{
			if (id < NONE || id > BELT || (id > SADDLE && id < MEDAL) || id == 14)
				return NONE;

			return static_cast<Value>(id);
		}
	};

	using Graphics::Texture;
	using Graphics::DrawArgument;

	class Clothing : public ItemData
	{
	public:
		static const size_t NUM_LAYERS = 25;
		enum Layer
		{
			CAPE, SHIELDBBODY, SHOES, PANTS, TOP, MAIL, 
			GLOVE, SHIELDOHAIR, EARRINGS, FACEACC, EYEACC, 
			PENDANT, BELT, MEDAL, RING, SHIELD, BACKSHIELD, 
			WEAPON, BACKWEAPON, MAILARM, HATOVERHAIR, HAT, 
			WEAPONOHAND, RGLOVE, WEAPONOGLOVE
		};

		static EnumIterator<Layer> layerit()
		{
			return EnumIterator<Layer>(CAPE, WEAPONOGLOVE);
		}

		static Layer sublayer(Layer base, string name)
		{
			switch (base)
			{
			case WEAPON:
				if (name == "weaponOverHand")
					return Layer::WEAPONOHAND;
				else if (name == "weaponOverGlove")
					return Layer::WEAPONOGLOVE;
				else if (name == "backWeaponOverShield")
					return Layer::BACKWEAPON;
				break;
			case SHIELD:
				if (name == "shieldOverHair")
					return Layer::SHIELDOHAIR;
				else if (name == "shieldBelowBody")
					return Layer::SHIELDBBODY;
				else if (name == "backShield")
					return Layer::BACKSHIELD;
				break;
			case HAT:
				if (name == "capOverHair")
					return Layer::HATOVERHAIR;
				break;
			}
			return base;
		}

		Clothing(int32_t equipid, const BodyDrawinfo& drawinfo);
		Clothing();

		void draw(Stance::Value stance, Layer layer, uint8_t frame, const DrawArgument& args) const;
		bool islayer(Stance::Value stance, Layer layer) const;
		bool istransparent() const;
		bool isweapon() const;
		int16_t getreqstat(Maplestat::Value stat) const;
		int16_t getdefstat(Equipstat::Value stat) const;
		string gettype() const;
		Slot::Value geteqslot() const;

	private:
		unordered_map<Layer, unordered_map<uint8_t, Texture>> stances[Stance::LENGTH];
		string type;
		Slot::Value eqslot;
		bool cash;
		bool tradeblock;
		bool transparent;
		int32_t price;
		uint8_t slots;
		map<Maplestat::Value, int16_t> reqstats;
		map<Equipstat::Value, int16_t> defstats;
	};
}

