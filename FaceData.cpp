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
#include "FaceData.h"
#include "nx.hpp"

namespace Data
{
	FaceData::FaceData(int faceid)
	{
		node facenode = nx::character["Face"]["000" + to_string(faceid) + ".img"];
		for (node expnode = facenode.begin(); expnode != facenode.end(); ++expnode)
		{
			string state = expnode.name();
			if (state == "default")
			{
				node shiftnode = expnode["face"]["map"]["brow"];
				stances[state][CL_FACE].add(0, new Texture(expnode["face"]));
				stances[state][CL_FACE].get(0)->setshift(vector2d<int>(-shiftnode.x(), -shiftnode.y()));
				delays[state][0] = 2500;
			}
			else if (state != "info")
			{
				uint8_t frame = 0;
				node framenode = expnode[to_string(frame)];
				while (framenode.size() > 0)
				{
					if (framenode["face"].data_type() == node::type::bitmap)
					{
						node shiftnode = framenode["face"]["map"]["brow"];
						stances[state][CL_FACE].add(frame, new Texture(framenode["face"]));
						stances[state][CL_FACE].get(frame)->setshift(vector2d<int>(-shiftnode.x(), -shiftnode.y()));
						delays[state][frame] = (framenode["delay"].data_type() == node::type::integer) ? framenode["delay"] : 2500;
					}

					frame++;
					framenode = expnode[to_string(frame)];
				}
			}
		}

		name = nx::string["Eqp.img"]["Eqp"]["Face"][to_string(faceid)]["name"];
	}

	uint8_t FaceData::nextframe(string exp, uint8_t frame)
	{
		return (frame < delays[exp].size() - 1) ? (frame + 1) : 0;
	}

	short FaceData::getdelay(string exp, uint8_t frame)
	{
		return delays[exp][frame];
	}
}
