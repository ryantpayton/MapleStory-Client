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
#include "Bodytype.h"
#include "nlnx\nx.hpp"

namespace Character
{
	Bodytype::Bodytype(int8_t skin, const BodyDrawinfo& drawinfo)
	{
		string sk;
		if (skin < 10)
		{
			sk.append("0");
		}
		sk.append(std::to_string(skin));

		using nl::node;
		node bodynode = nl::nx::character["000020" + sk + ".img"];
		node headnode = nl::nx::character["000120" + sk + ".img"];

		for (node stancenode : bodynode)
		{
			string stance = stancenode.name();
			uint8_t frame = 0;
			node framenode = stancenode[std::to_string(frame)];
			while (framenode.size() > 0)
			{
				for (node partnode : framenode)
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

						stances[stance][z][frame] = Texture(partnode);

						vector2d<int16_t> shift = vector2d<int16_t>();
						if (partnode["map"]["navel"].data_type() == node::type::vector)
						{
							shift -= vector2d<int16_t>(partnode["map"]["navel"]);
						}
						shift += drawinfo.getbodypos(stance, frame);

						stances[stance][z][frame].shift(shift);
					}

					node headsfnode = headnode[stance][std::to_string(frame)]["head"];
					if (headsfnode.data_type() == node::type::bitmap)
					{
						stances[stance][CL_HEAD][frame] = Texture(headsfnode);
						stances[stance][CL_HEAD][frame].shift(drawinfo.getheadpos(stance, frame));
					}
				}

				frame++;
				framenode = stancenode[std::to_string(frame)];
			}
		}

		static const string skintypes[12] =
		{
			"Light", "Tan", "Dark", "Pale", "Blue", "Green", "", "", "", "Grey", "Pink", "Red"
		};

		name = (skin < 12) ? skintypes[skin] : "";
	}

	Bodytype::Bodytype() 
	{
		name = "";
	}

	Bodytype::~Bodytype() {}

	void Bodytype::draw(string stance, CharacterLayer layer, 
		uint8_t frame, const DrawArgument& args) const {

		if (!stances.count(stance))
			return;

		if (!stances.at(stance).count(layer))
			return;

		if (!stances.at(stance).at(layer).count(frame))
			return;

		stances.at(stance).at(layer).at(frame).draw(args);
	}

	string Bodytype::getname() const
	{
		return name;
	}
}
