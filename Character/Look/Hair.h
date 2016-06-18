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

#include "..\..\Console.h"
#include "..\..\Graphics\Texture.h"
#include "..\..\Util\Enum.h"

namespace jrc
{
	class Hair
	{
	public:
		static const size_t NUM_LAYERS = 7;
		enum Layer
		{
			NONE, DEFAULT, BELOWBODY, OVERHEAD,
			SHADE, BACK, BELOWCAP
		};

		static EnumIterator<Layer> layerit()
		{
			return EnumIterator<Layer>(DEFAULT, BELOWCAP);
		}

		static Layer layerbystring(std::string name)
		{
			if (name == "backHairBelowCapWide" || name == "backHairBelowCapNarrow")
				return BELOWCAP;

			static std::string layernames[NUM_LAYERS] =
			{
				"", "hair", "hairBelowBody", "hairOverHead",
				"hairShade", "backHair", "backHairBelowCap"
			};

			for (auto it = layerit(); it.hasnext(); it.increment())
			{
				Layer layer = it.get();
				if (layernames[layer] == name)
					return layer;
			}

			Console::get().print("Unhandled hair layer: " + name);

			return NONE;
		}

		Hair(int32_t hairid, const BodyDrawinfo& drawinfo);
		Hair();

		void draw(Stance::Value stance, Layer layer, uint8_t frame, const DrawArgument& args) const;
		std::string get_name() const;
		std::string getcolor() const;

	private:
		std::unordered_map<Layer, std::unordered_map<uint8_t, Texture>> stances[Stance::LENGTH];
		std::string name;
		std::string color;
	};
}

