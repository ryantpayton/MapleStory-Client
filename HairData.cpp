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
#include "HairData.h"
#include "nx.hpp"

namespace Data
{
	const string haircolors[8] = 
	{ 
		"Black", "Red", "Orange", "Blonde", "Green", "Blue", "Violet", "Brown" 
	};

	HairData::HairData(int hairid, BodyDrawinfo& drawinfo)
	{
		node hairnode = nx::character["Hair"]["000" + to_string(hairid) + ".img"];
		for (node stancenode = hairnode.begin(); stancenode != hairnode.end(); ++stancenode)
		{
			string stance = stancenode.name();
			if (stance != "default" || stance != "backDefault" || stance != "info")
			{
				uint8_t frame = 0;
				node framenode = stancenode[to_string(frame)];
				while (framenode.size() > 0)
				{
					for (node partnode = framenode.begin(); partnode != framenode.end(); ++partnode)
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

						stances[stance][z].add(frame, new Texture(partnode));

						vector2d<int> shift = vector2d<int>();
						if (partnode["map"]["brow"].data_type() == node::type::vector)
						{
							shift -= vector2d<int>(partnode["map"]["brow"].x(), partnode["map"]["brow"].y());
						}
						shift += drawinfo.gethairpos(stance, frame);

						stances[stance][z].get(frame)->setshift(shift);
					}

					frame++;
					framenode = stancenode[to_string(frame)];
				}
			}
		}

		name = nx::string["Eqp.img"]["Eqp"]["Hair"][to_string(hairid)]["name"];
		color = (hairid % 10 < 8) ? haircolors[hairid % 10] : "";
	}
}
