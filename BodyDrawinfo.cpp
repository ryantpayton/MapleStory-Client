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
#include "BodyDrawinfo.h"
#include "CharacterData.h"
#include "nx.hpp"
#include "node.hpp"

namespace Data
{
	BodyDrawinfo::BodyDrawinfo() {}

	BodyDrawinfo::~BodyDrawinfo() {}

	void BodyDrawinfo::init()
	{
		using::nl::node;
		node bodynode = nl::nx::character["00002000.img"];
		node headnode = nl::nx::character["00012000.img"];

		for (node stancenode = bodynode.begin(); stancenode != bodynode.end(); ++stancenode)
		{
			string stance = stancenode.name();

			uint8_t frame = 0;
			node framenode = stancenode[std::to_string(frame)];
			while (framenode.size() > 0)
			{
				uint16_t delay = (framenode["delay"].data_type() == node::type::integer) ? framenode["delay"] : 50;

				node actionnode = framenode.resolve("action");
				if (actionnode.data_type() == node::type::string)
				{
					string acstance = actionnode.get_string();
					uint8_t acframe = framenode["frame"];
					uint16_t acdelay = (delay < 0) ? -delay : delay;
					bodyactions[stance][frame] = BodyAction(acstance, acframe, delay);
				}
				else
				{
					stancedelays[stance][frame] = delay;
					map<CharacterLayer, map<string, vector2d<int32_t>>> bodyshiftmap;

					for (node partnode = framenode.begin(); partnode != framenode.end(); ++partnode)
					{
						string part = partnode.name();
						if (part != "delay" && part != "face")
						{
							CharacterLayer z;
							string zstr = partnode["z"];
							if (zstr == "armOverHair")
								z = CL_ARMOHAIR;
							else if (zstr == "handOverHair")
								z = CL_RHAND;
							else if (zstr == "body" || zstr == "backBody")
								z = CL_BODY;
							else if (zstr == "handBelowWeapon")
								z = CL_LHAND;
							else if (zstr == "arm")
								z = CL_ARM;
							else
								continue;

							node bodymap = partnode["map"];
							for (node mapnode = bodymap.begin(); mapnode != bodymap.end(); mapnode++)
							{
								bodyshiftmap[z][mapnode.name()] = vector2d<int32_t>(mapnode.x(), mapnode.y());
							}
						}
					}

					node headmap = headnode[stance][std::to_string(frame)]["head"]["map"];
					for (node mapnode = headmap.begin(); mapnode != headmap.end(); mapnode++)
					{
						bodyshiftmap[CL_HEAD][mapnode.name()] = vector2d<int32_t>(mapnode.x(), mapnode.y());
					}

					bodyposmap[stance][frame] = bodyshiftmap[CL_BODY]["navel"];
					armposmap[stance][frame] = bodyshiftmap.count(CL_ARM) ? (bodyshiftmap[CL_ARM]["hand"] - bodyshiftmap[CL_ARM]["navel"]) : (bodyshiftmap[CL_ARMOHAIR]["hand"] - bodyshiftmap[CL_ARMOHAIR]["navel"]);
					handposmap[stance][frame] = bodyshiftmap[CL_LHAND]["handMove"];
					headposmap[stance][frame] = bodyshiftmap[CL_BODY]["neck"] - bodyshiftmap[CL_HEAD]["neck"];
					faceposmap[stance][frame] = bodyshiftmap[CL_BODY]["neck"] - bodyshiftmap[CL_HEAD]["neck"] + bodyshiftmap[CL_HEAD]["brow"];
					hairposmap[stance][frame] = bodyshiftmap[CL_HEAD]["brow"] - bodyshiftmap[CL_HEAD]["neck"] + bodyshiftmap[CL_BODY]["neck"];
				}

				frame++;
				framenode = stancenode[std::to_string(frame)];
			}
		}
	}

	vector2d<int32_t> BodyDrawinfo::getbodypos(string stance, uint8_t pos) const
	{
		if (bodyposmap.count(stance))
		{
			if (pos < bodyposmap.at(stance).size())
			{
				return bodyposmap.at(stance).at(pos);
			}
		}
		return vector2d<int32_t>();
	}

	vector2d<int32_t> BodyDrawinfo::getarmpos(string stance, uint8_t pos) const
	{
		if (armposmap.count(stance))
		{
			if (pos < armposmap.at(stance).size())
			{
				return armposmap.at(stance).at(pos);
			}
		}
		return vector2d<int32_t>();
	}

	vector2d<int32_t> BodyDrawinfo::gethandpos(string stance, uint8_t pos) const
	{
		if (handposmap.count(stance))
		{
			if (pos < handposmap.at(stance).size())
			{
				return handposmap.at(stance).at(pos);
			}
		}
		return vector2d<int32_t>();
	}

	vector2d<int32_t> BodyDrawinfo::getheadpos(string stance, uint8_t pos) const
	{
		if (headposmap.count(stance))
		{
			if (pos < headposmap.at(stance).size())
			{
				return headposmap.at(stance).at(pos);
			}
		}
		return vector2d<int32_t>();
	}

	vector2d<int32_t> BodyDrawinfo::gethairpos(string stance, uint8_t pos) const
	{
		if (hairposmap.count(stance))
		{
			if (pos < hairposmap.at(stance).size())
			{
				return hairposmap.at(stance).at(pos);
			}
		}
		return vector2d<int32_t>();
	}

	vector2d<int32_t> BodyDrawinfo::getfacepos(string stance, uint8_t pos) const
	{
		if (faceposmap.count(stance))
		{
			if (pos < faceposmap.at(stance).size())
			{
				return faceposmap.at(stance).at(pos);
			}
		}
		return vector2d<int32_t>();
	}

	uint8_t BodyDrawinfo::nextframe(string stance, uint8_t pos) const
	{
		if (stancedelays.count(stance))
		{
			if (pos < stancedelays.at(stance).size() - 1)
			{
				return pos + 1;
			}
		}
		return 0;
	}

	uint16_t BodyDrawinfo::getdelay(string stance, uint8_t pos) const
	{
		if (stancedelays.count(stance))
		{
			if (pos < stancedelays.at(stance).size())
			{
				return stancedelays.at(stance).at(pos);
			}
		}
		return 50;
	}

	uint8_t BodyDrawinfo::nextacframe(string action, uint8_t pos) const
	{
		if (bodyactions.count(action))
		{
			if (pos < bodyactions.at(action).size() - 1)
			{
				return pos + 1;
			}
		}
		return 0;
	}

	const BodyAction* BodyDrawinfo::getaction(string stance, uint8_t pos) const
	{
		if (bodyactions.count(stance))
		{
			if (pos < bodyactions.at(stance).size())
			{
				return &bodyactions.at(stance).at(pos);
			}
		}
		return nullptr;
	}
}