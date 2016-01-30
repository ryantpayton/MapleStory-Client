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
#include "Face.h"
#include "nlnx\nx.hpp"

namespace Character
{
	Face::Face(int32_t faceid)
	{
		using nl::node;
		node facenode = nl::nx::character["Face"]["000" + std::to_string(faceid) + ".img"];

		node defnode = facenode["default"];
		expressions[Expression::DEFAULT][0] = defnode["face"];
		expressions[Expression::DEFAULT][0].shift(-vector2d<int16_t>(defnode["face"]["map"]["brow"]));
		delays[Expression::DEFAULT][0] = 2500;

		for (auto it = Expression::getit(Expression::BLINK); it.hasnext(); it.increment())
		{
			Expression::Value exp = it.get();
			string expname = Expression::nameof(exp);
			node expnode = facenode[expname];
			if (expnode.size() == 0)
				continue;

			uint8_t frame = 0;
			node framenode = expnode[std::to_string(frame)];
			while (framenode.size() > 0)
			{
				if (framenode["face"].data_type() == node::type::bitmap)
				{
					expressions[exp][frame] = framenode["face"];
					expressions[exp][frame].shift(-vector2d<int16_t>(framenode["face"]["map"]["brow"]));

					if (framenode["delay"].data_type() == node::type::integer)
						delays[exp][frame] = framenode["delay"];
					else
						delays[exp][frame] = 2500;
				}

				frame++;
				framenode = expnode[std::to_string(frame)];
			}
		}

		name = nl::nx::string["Eqp.img"]["Eqp"]["Face"][std::to_string(faceid)]["name"];
	}

	Face::Face() {}

	void Face::draw(Expression::Value expression, uint8_t frame, const DrawArgument& args) const
	{
		if (!expressions[expression].count(frame))
			return;

		expressions[expression].at(frame).draw(args);
	}

	uint8_t Face::nextframe(Expression::Value exp, uint8_t frame) const
	{
		if (frame < delays[exp].size() - 1)
		{
			return frame + 1;
		}
		return 0;
	}

	int16_t Face::getdelay(Expression::Value exp, uint8_t frame) const
	{
		if (delays[exp].count(frame))
		{
			return delays[exp].at(frame);
		}
		return 50;
	}

	string Face::getname() const
	{
		return name;
	}
}
