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
#include "Facetype.h"
#include "nlnx\nx.hpp"

namespace Character
{
	Facetype::Facetype(int32_t faceid)
	{
		using::nl::node;
		node facenode = nl::nx::character["Face"]["000" + std::to_string(faceid) + ".img"];
		for (node expnode : facenode)
		{
			string state = expnode.name();
			if (state == "default")
			{
				node shiftnode = expnode["face"]["map"]["brow"];
				stances[state][CL_FACE][0] = Texture(expnode["face"]);
				stances[state][CL_FACE][0].setshift(vector2d<int16_t>() - vector2d<int16_t>(shiftnode));
				delays[state][0] = 2500;
			}
			else if (state != "info")
			{
				uint8_t frame = 0;
				node framenode = expnode[std::to_string(frame)];
				while (framenode.size() > 0)
				{
					if (framenode["face"].data_type() == node::type::bitmap)
					{
						node shiftnode = framenode["face"]["map"]["brow"];
						stances[state][CL_FACE][frame] = Texture(framenode["face"]);
						stances[state][CL_FACE][frame].setshift(vector2d<int16_t>() - vector2d<int16_t>(shiftnode));

						if (framenode["delay"].data_type() == node::type::integer)
							delays[state][frame] = framenode["delay"];
						else
							delays[state][frame] = 2500;
					}

					frame++;
					framenode = expnode[std::to_string(frame)];
				}
			}
		}

		name = nl::nx::string["Eqp.img"]["Eqp"]["Face"][std::to_string(faceid)]["name"];
	}

	Facetype::Facetype() {}

	Facetype::~Facetype() {}

	void Facetype::draw(string stance, CharacterLayer layer, uint8_t frame, const DrawArgument& args) const
	{
		if (stances.count(stance))
		{
			if (stances.at(stance).count(layer))
			{
				if (stances.at(stance).at(layer).count(frame))
				{
					stances.at(stance).at(layer).at(frame).draw(args);
				}
			}
		}
	}

	uint8_t Facetype::nextframe(string exp, uint8_t frame) const
	{
		if (delays.count(exp))
		{
			if (frame < delays.at(exp).size() - 1)
			{
				return frame + 1;
			}
		}
		return 0;
	}

	short Facetype::getdelay(string exp, uint8_t frame) const
	{
		if (delays.count(exp))
		{
			if (delays.at(exp).count(frame))
			{
				return delays.at(exp).at(frame);
			}
		}
		return 50;
	}

	const string& Facetype::getname() const
	{
		return name;
	}
}
