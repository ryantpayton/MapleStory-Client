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
#include "BodyData.h"
#include "nx.hpp"

namespace Data
{
	const string skintypes[12] =
	{
		"Light", "Tan", "Dark", "Pale", "Blue", "Green", "", "", "", "Grey", "Pink", "Red"
	};

	BodyData::BodyData(int8_t skin, const BodyDrawinfo& drawinfo)
	{
		string sk;
		if (skin < 10)
		{
			sk.append("0");
		}
		sk.append(std::to_string(skin));

		using::nl::node;
		node bodynode = nl::nx::character["000020" + sk + ".img"];
		node headnode = nl::nx::character["000120" + sk + ".img"];

		for (node stancenode = bodynode.begin(); stancenode != bodynode.end(); ++stancenode)
		{
			string stance = stancenode.name();
			uint8_t frame = 0;
			node framenode = stancenode[std::to_string(frame)];
			while (framenode.size() > 0)
			{
				for (node partnode = framenode.begin(); partnode != framenode.end(); ++partnode)
				{
					string part = partnode.name();
					if (part != "delay" && part != "face")
					{
						CharacterLayer z;
						string zs = partnode["z"];
						if (zs == "armOverHair")
							z = CL_ARMOHAIR;
						else if (zs == "handOverHair")
							z = CL_RHAND;
						else if (zs == "body" || zs == "backBody")
							z = CL_BODY;
						else if (zs == "handBelowWeapon")
							z = CL_LHAND;
						else if (zs == "arm")
							z = CL_ARM;
						else
							continue;

						stances[stance][z].add(frame, new Texture(partnode));

						vector2d<int32_t> shift = vector2d<int32_t>();
						if (partnode["map"]["navel"].data_type() == node::type::vector)
						{
							shift -= vector2d<int32_t>(partnode["map"]["navel"].x(), partnode["map"]["navel"].y());
						}
						shift += drawinfo.getbodypos(stance, frame);

						stances[stance][z].get(frame)->setshift(shift);
					}

					node headsfnode = headnode[stance][std::to_string(frame)]["head"];
					if (headsfnode.data_type() == node::type::bitmap)
					{
						stances[stance][CL_HEAD].add(frame, new Texture(headsfnode));
						stances[stance][CL_HEAD].get(frame)->setshift(drawinfo.getheadpos(stance, frame));
					}
				}

				frame++;
				framenode = stancenode[std::to_string(frame)];
			}
		}

		name = (skin < 12) ? skintypes[skin] : "";
	}

	const string& BodyData::getname() const
	{
		return name;
	}
}
