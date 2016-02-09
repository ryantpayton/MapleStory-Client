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
#include "Graphics\Texture.h"
#include "Util\Enum.h"
#include "Console.h"

namespace Character
{
	using Graphics::Texture;
	using Graphics::DrawArgument;

	class Body
	{
	public:
		static const size_t NUM_LAYERS = 8;
		enum Layer
		{
			NONE, BODY, ARM, ARMOVERHAIR,
			LEFTHAND, RIGHTHAND, HANDOVER, HEAD
		};

		static EnumIterator<Layer> layerit(Layer s = BODY, Layer l = HEAD)
		{
			return EnumIterator<Layer>(s, l);
		}

		static Layer layerbystring(string name)
		{
			if (name == "backBody")
				return BODY;

			static string layernames[NUM_LAYERS] =
			{
				"", "body", "arm", "armOverHair",
				"handBelowWeapon", "handOverHair", "handOverWeapon", "head"
			};

			for (auto it = layerit(); it.hasnext(); it.increment())
			{
				Layer layer = it.get();
				if (layernames[layer] == name)
					return layer;
			}

			Console::get().print("Unhandled body layer: " + name);

			return NONE;
		}

		Body(int32_t skin, const BodyDrawinfo& drawinfo);
		Body();

		void draw(Stance::Value stance, Layer layer, uint8_t frame, const DrawArgument& args) const;
		string getname() const;

	private:
		unordered_map<Layer, unordered_map<uint8_t, Texture>> stances[Stance::LENGTH];
		string name;
	};
}

