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
#include "Hairstyle.h"
#include "nlnx\nx.hpp"

namespace Character
{
	const string haircolors[8] = 
	{ 
		"Black", "Red", "Orange", "Blonde", "Green", "Blue", "Violet", "Brown" 
	};

	Hairstyle::Hairstyle(int32_t hairid, const BodyDrawinfo& drawinfo)
	{
		using nl::node;
		node hairnode = nl::nx::character["Hair"]["000" + std::to_string(hairid) + ".img"];
		for (node stancenode : hairnode)
		{
			string stance = stancenode.name();
			if (stance != "default" || stance != "backDefault" || stance != "info")
			{
				uint8_t frame = 0;
				node framenode = stancenode[std::to_string(frame)];
				while (framenode.size() > 0)
				{
					for (node partnode : framenode)
					{
						string part = partnode.name();
						CharacterLayer z;
						if (part == "hair")
							z = CL_HAIR;
						else if (part == "hairBelowBody")
							z = CL_HAIRBBODY;
						else if (part == "hairOverHead")
							z = CL_HAIROHEAD;
						else if (part == "hairShade")
							z = CL_HAIRSHADE;
						else if (part == "backHair")
							z = CL_BACKHAIR;
						else if (part == "backHairBelowCap" || "backHairBelowCapWide")
							z = CL_BACKHAIRBCAP;

						stances[stance][z][frame] = Texture(partnode);
						stances[stance][z][frame].shift(
							drawinfo.gethairpos(stance, frame) - vector2d<int16_t>(partnode["map"]["brow"])
							);
					}

					frame++;
					framenode = stancenode[std::to_string(frame)];
				}
			}
		}

		name = nl::nx::string["Eqp.img"]["Eqp"]["Hair"][std::to_string(hairid)]["name"];
		color = (hairid % 10 < 8) ? haircolors[hairid % 10] : "";
	}

	Hairstyle::Hairstyle() {}

	Hairstyle::~Hairstyle() {}

	void Hairstyle::draw(string stance, CharacterLayer layer, 
		uint8_t frame, const DrawArgument& args) const {

		if (!stances.count(stance))
			return;

		if (!stances.at(stance).count(layer))
			return;

		if (!stances.at(stance).at(layer).count(frame))
			return;

		stances.at(stance).at(layer).at(frame).draw(args);
	}

	const string& Hairstyle::getname() const
	{ 
		return name; 
	}

	const string& Hairstyle::getcolor() const
	{ 
		return color; 
	}
}
