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
#include "Character\Inventory\ItemData.h"
#include "Character\CharConstants.h"

namespace Character
{
	using Graphics::DrawArgument;

	class Clothing : public ItemData
	{
	public:
		Clothing(int32_t, const BodyDrawinfo&);
		Clothing();

		void draw(string stance, CharacterLayer layer, 
			uint8_t frame, const DrawArgument& args) const;
		bool islayer(string stance, CharacterLayer layer) const;
		bool istransparent() const;
		const string& gettype() const;
		Equipslot geteqslot() const;

	private:
		map<string, map<CharacterLayer, map<uint8_t, Texture>>> stances;
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

