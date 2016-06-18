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
#include "BodyDrawinfo.h"
#include "Equipslot.h"

#include "..\Equipstat.h"
#include "..\Maplestat.h"
#include "..\Inventory\ItemData.h"

#include "..\..\Console.h"
#include "..\..\Graphics\Texture.h"
#include "..\..\Util\Enum.h"
#include "..\..\Util\EnumMap.h"

#include <vector>
#include <map>

namespace jrc
{
	class Clothing : public ItemData
	{
	public:
		enum Layer
		{
			CAPE, SHOES, PANTS, TOP, MAIL, MAILARM,
			EARRINGS, FACEACC, EYEACC, PENDANT, BELT, MEDAL, RING, 
			CAP, CAPBBODY, CAPOHAIR,
			GLOVE, WRIST, GLOVEOHAIR, WRISTOHAIR, GLOVEOBODY, WRISTOBODY,
			SHIELD, BACKSHIELD, SHIELDBBODY, SHIELDOHAIR,
			WEAPON, BACKWEAPON, WEAPONBARM, WEAPONBBODY, WEAPONOHAND, WEAPONOBODY, WEAPONOGLOVE,
			NUM_LAYERS
		};

		Clothing(int32_t equipid, const BodyDrawinfo& drawinfo);
		Clothing();

		void draw(Stance::Value stance, Layer layer, uint8_t frame, const DrawArgument& args) const;

		bool contains_layer(Stance::Value stance, Layer layer) const;
		bool istransparent() const;
		bool isweapon() const;
		int16_t getreqstat(Maplestat::Value stat) const;
		int16_t getdefstat(Equipstat::Value stat) const;
		std::string gettype() const;
		std::string getvslot() const;
		Equipslot::Value geteqslot() const;

	private:
		static EnumIterator<Layer> layerit();
		static Layer sublayer(Layer base, const std::string& name);

		struct Whole
		{
			std::vector<Texture> parts;

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

		EnumMap<Stance::Value, EnumMap<Layer, std::unordered_map<uint8_t, Whole>, NUM_LAYERS>> stances;
		EnumMap<Maplestat::Value, int16_t> reqstats;
		EnumMap<Equipstat::Value, int16_t> defstats;
		std::string type;
		std::string vslot;
		int32_t price;
		Equipslot::Value eqslot;
		uint8_t slots;
		bool cash;
		bool tradeblock;
		bool transparent;
	};
}

