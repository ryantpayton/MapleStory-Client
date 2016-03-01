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
#include "Body.h"
#include "nlnx\nx.hpp"

namespace Character
{
	Body::Body(int32_t skin, const BodyDrawinfo& drawinfo)
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

		for (auto it = Stance::getit(); it.hasnext(); it.increment())
		{
			Stance::Value stance = it.get();
			string stancename = Stance::nameof(stance);
			node stancenode = bodynode[stancename];
			if (stancenode.size() == 0)
				continue;

			uint8_t frame = 0;
			node framenode = stancenode[std::to_string(frame)];
			while (framenode.size() > 0)
			{
				for (node partnode : framenode)
				{
					string part = partnode.name();
					if (part != "delay" && part != "face")
					{
						string z = partnode["z"];
						Layer layer = layerbystring(z);

						stances[stance][layer][frame] = partnode;
						stances[stance][layer][frame].shift(drawinfo.getbodypos(stance, frame));
						stances[stance][layer][frame].shift(-Point<int16_t>(partnode["map"]["navel"]));
					}

					node headsfnode = headnode[stancename][std::to_string(frame)]["head"];
					if (headsfnode.size() > 0)
					{
						stances[stance][Layer::HEAD][frame] = headsfnode;
						stances[stance][Layer::HEAD][frame].shift(drawinfo.getheadpos(stance, frame));
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

	Body::Body() 
	{
		name = "";
	}

	void Body::draw(Stance::Value stance, Layer layer, uint8_t frame, const DrawArgument& args) const
	{
		if (!stances[stance].count(layer))
			return;

		if (!stances[stance].at(layer).count(frame))
			return;

		stances[stance].at(layer).at(frame).draw(args);
	}

	string Body::getname() const
	{
		return name;
	}
}
