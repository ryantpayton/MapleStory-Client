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
#include "Equipslot.h"
#include "Console.h"

#include "Character\Equipstat.h"
#include "Character\Maplestat.h"
#include "Character\Inventory\ItemData.h"
#include "Graphics\Texture.h"
#include "Util\Enum.h"

#include <vector>

namespace Character
{
	using std::vector;
	using Graphics::Texture;
	using Graphics::DrawArgument;

	class Clothing : public ItemData
	{
	public:
		static const size_t NUM_LAYERS = 32;
		enum Layer
		{
			CAPE, SHOES, PANTS, TOP, MAIL, MAILARM,
			EARRINGS, FACEACC, EYEACC, PENDANT, BELT, MEDAL, RING, 
			CAP, CAPBBODY, CAPOHAIR,
			GLOVE, WRIST, GLOVEOHAIR, WRISTOHAIR, GLOVEOBODY, WRISTOBODY,
			SHIELD, BACKSHIELD, SHIELDBBODY, SHIELDOHAIR,
			WEAPON, BACKWEAPON, WEAPONBARM, WEAPONBBODY, WEAPONOHAND, WEAPONOBODY, WEAPONOGLOVE
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
				else if (name == "weaponOverBody")
					return Layer::WEAPONOBODY;
				else if (name == "weaponBelowArm")
					return Layer::WEAPONBARM;
				else if (name == "weaponBelowBody")
					return Layer::WEAPONBBODY;
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
			case GLOVE:
				if (name == "gloveWrist")
					return Layer::WRIST;
				else if (name == "gloveOverHair")
					return Layer::GLOVEOHAIR;
				else if (name == "gloveOverBody")
					return Layer::GLOVEOBODY;
				else if (name == "gloveWristOverHair")
					return Layer::WRISTOHAIR;
				else if (name == "gloveWristOverBody")
					return Layer::WRISTOBODY;
			case CAP:
				if (name == "capOverHair")
					return Layer::CAPOHAIR;
				else if (name == "capBelowBody")
					return Layer::CAPBBODY;
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
		string getvslot() const;
		Equipslot::Value geteqslot() const;

	private:
		struct Whole
		{
			vector<Texture> parts;

			void add(Texture part)
			{
				parts.push_back(part);
			}

			void draw(const DrawArgument& args) const
			{
				for (auto& part : parts)
				{
					part.draw(args);
				}
			}
		};
		unordered_map<Layer, unordered_map<uint8_t, Whole>> stances[Stance::LENGTH];
		string type;
		string vslot;
		Equipslot::Value eqslot;
		bool cash;
		bool tradeblock;
		bool transparent;
		int32_t price;
		uint8_t slots;
		map<Maplestat::Value, int16_t> reqstats;
		map<Equipstat::Value, int16_t> defstats;
	};
}

