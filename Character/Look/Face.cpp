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

namespace jrc
{
	Face::Face(int32_t faceid)
	{
		nl::node facenode = nl::nx::character["Face"]["000" + std::to_string(faceid) + ".img"];

		for (auto iter : expressions)
		{
			Expression::Value exp = iter.first;
			if (exp == Expression::DEFAULT)
			{
				iter.second.emplace(0, facenode["default"]);
			}
			else
			{
				std::string expname = Expression::names[exp];
				nl::node expnode = facenode[expname];

				uint8_t frame = 0;
				nl::node framenode = expnode[std::to_string(frame)];
				while (framenode.size() > 0)
				{
					iter.second.emplace(frame, framenode);

					frame++;
					framenode = expnode[std::to_string(frame)];
				}
			}
		}

		name = nl::nx::string["Eqp.img"]["Eqp"]["Face"][std::to_string(faceid)]["name"];
	}

	Face::Face() {}

	void Face::draw(Expression::Value expression, uint8_t frame, const DrawArgument& args) const
	{
		auto frameit = expressions[expression].find(frame);
		if (frameit != expressions[expression].end())
			frameit->second.texture.draw(args);
	}

	uint8_t Face::nextframe(Expression::Value exp, uint8_t frame) const
	{
		return expressions[exp].count(frame + 1) ? frame + 1 : 0;
	}

	int16_t Face::getdelay(Expression::Value exp, uint8_t frame) const
	{
		auto delayit = expressions[exp].find(frame);
		return delayit != expressions[exp].end() ? delayit->second.delay : 100;
	}

	std::string Face::getname() const
	{
		return name;
	}
}
