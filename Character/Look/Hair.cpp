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
#include "Hair.h"
#include "nlnx\nx.hpp"

namespace jrc
{
	Hair::Hair(int32_t hairid, const BodyDrawinfo& drawinfo)
	{
		nl::node hairnode = nl::nx::character["Hair"]["000" + std::to_string(hairid) + ".img"];

		for (auto it = Stance::getit(); it.hasnext(); it.increment())
		{
			Stance::Value stance = it.get();
			std::string stancename = Stance::nameof(stance);
			nl::node stancenode = hairnode[stancename];
			if (stancenode.size() == 0)
				continue;

			uint8_t frame = 0;
			nl::node framenode = stancenode[std::to_string(frame)];
			while (framenode.size() > 0)
			{
				for (nl::node partnode : framenode)
				{
					std::string part = partnode.name();
					Layer layer = layerbystring(part);

					stances[stance][layer][frame] = partnode;
					stances[stance][layer][frame].shift(drawinfo.gethairpos(stance, frame));
					stances[stance][layer][frame].shift(-Point<int16_t>(partnode["map"]["brow"]));
				}

				frame++;
				framenode = stancenode[std::to_string(frame)];
			}
		}

		name = nl::nx::string["Eqp.img"]["Eqp"]["Hair"][std::to_string(hairid)]["name"];

		static constexpr char* haircolors[8] =
		{
			"Black", "Red", "Orange", "Blonde", "Green", "Blue", "Violet", "Brown"
		};
		color = (hairid % 10 < 8) ? haircolors[hairid % 10] : "";
	}

	Hair::Hair() 
	{
		name = "";
		color = "";
	}

	void Hair::draw(Stance::Value stance, Layer layer, uint8_t frame, const DrawArgument& args) const
	{
		auto layerit = stances[stance].find(layer);
		if (layerit == stances[stance].end())
			return;

		auto frameit = layerit->second.find(frame);
		if (frameit == layerit->second.end())
			return;

		frameit->second.draw(args);
	}

	std::string Hair::get_name() const
	{ 
		return name; 
	}

	std::string Hair::getcolor() const
	{ 
		return color; 
	}
}
