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
#include "BodyDrawinfo.h"
#include "CharacterData.h"
#include "nx.hpp"
#include "node.hpp"

using namespace nl;

namespace Data
{
	void BodyDrawinfo::init()
	{
		node bodynode = nx::character["00002000.img"];
		node headnode = nx::character["00012000.img"];

		for (node stancenode = bodynode.begin(); stancenode != bodynode.end(); ++stancenode)
		{
			string stance = stancenode.name();

			uint8_t frame = 0;
			node framenode = stancenode[to_string(frame)];
			while (framenode.size() > 0)
			{
				short delay = (framenode["delay"].data_type() == node::type::integer) ? framenode["delay"] : 50;

				node actionnode = framenode.resolve("action");
				if (actionnode.data_type() == node::type::string)
				{
					string acstance = actionnode.get_string();
					uint8_t acframe = static_cast<uint8_t>(framenode["frame"]);
					short acdelay = (delay < 0) ? -delay : delay;
					bodyactions[stance][frame] = BodyAction(acstance, acframe, delay);
				}
				else
				{
					stancedelays[stance][frame] = delay;
					map<CharacterLayer, map<string, vector2d<int>>> bodyshiftmap;

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
								bodyshiftmap[z][mapnode.name()] = vector2d<int>(mapnode.x(), mapnode.y());
							}
						}
					}

					node headmap = headnode[stance][to_string(frame)]["head"]["map"];
					for (node mapnode = headmap.begin(); mapnode != headmap.end(); mapnode++)
					{
						bodyshiftmap[CL_HEAD][mapnode.name()] = vector2d<int>(mapnode.x(), mapnode.y());
					}

					bodyposmap[stance][frame] = bodyshiftmap[CL_BODY]["navel"];
					armposmap[stance][frame] = bodyshiftmap.count(CL_ARM) ? (bodyshiftmap[CL_ARM]["hand"] - bodyshiftmap[CL_ARM]["navel"]) : (bodyshiftmap[CL_ARMOHAIR]["hand"] - bodyshiftmap[CL_ARMOHAIR]["navel"]);
					handposmap[stance][frame] = bodyshiftmap[CL_LHAND]["handMove"];
					headposmap[stance][frame] = bodyshiftmap[CL_BODY]["neck"] - bodyshiftmap[CL_HEAD]["neck"];
					faceposmap[stance][frame] = bodyshiftmap[CL_BODY]["neck"] - bodyshiftmap[CL_HEAD]["neck"] + bodyshiftmap[CL_HEAD]["brow"];
					hairposmap[stance][frame] = bodyshiftmap[CL_HEAD]["brow"] - bodyshiftmap[CL_HEAD]["neck"] + bodyshiftmap[CL_BODY]["neck"];
				}

				frame++;
				framenode = stancenode[to_string(frame)];
			}
		}
	}

	vector2d<int> BodyDrawinfo::getbodypos(string stance, uint8_t pos)
	{
		if (pos < bodyposmap[stance].size())
		{
			return bodyposmap[stance][pos];
		}
		else
		{
			return vector2d<int>();
		}
	}

	vector2d<int> BodyDrawinfo::getarmpos(string stance, uint8_t pos)
	{
		if (pos < armposmap[stance].size())
		{
			return armposmap[stance][pos];
		}
		else
		{
			return vector2d<int>();
		}
	}

	vector2d<int> BodyDrawinfo::gethandpos(string stance, uint8_t pos)
	{
		if (pos < handposmap[stance].size())
		{
			return handposmap[stance][pos];
		}
		else
		{
			return vector2d<int>();
		}
	}

	vector2d<int> BodyDrawinfo::getheadpos(string stance, uint8_t pos)
	{
		if (pos < headposmap[stance].size())
		{
			return headposmap[stance][pos];
		}
		else
		{
			return vector2d<int>();
		}
	}

	vector2d<int> BodyDrawinfo::gethairpos(string stance, uint8_t pos)
	{
		if (pos < hairposmap[stance].size())
		{
			return hairposmap[stance][pos];
		}
		else
		{
			return vector2d<int>();
		}
	}

	vector2d<int> BodyDrawinfo::getfacepos(string stance, uint8_t pos)
	{
		if (pos < faceposmap[stance].size())
		{
			return faceposmap[stance][pos];
		}
		else
		{
			return vector2d<int>();
		}
	}

	uint8_t BodyDrawinfo::nextframe(string stance, uint8_t pos)
	{
		return (pos < stancedelays[stance].size() - 1) ? (pos + 1) : 0;
	}

	short BodyDrawinfo::getdelay(string stance, uint8_t pos)
	{
		if (pos < stancedelays[stance].size())
		{
			return stancedelays[stance][pos];
		}
		else
		{
			return 50;
		}
	}

	uint8_t BodyDrawinfo::nextacframe(string action, uint8_t pos)
	{
		return (pos < bodyactions[action].size() - 1) ? (pos + 1) : 0;
	}

	BodyAction* BodyDrawinfo::getaction(string stance, uint8_t pos)
	{
		if (pos < bodyactions[stance].size())
		{
			return &bodyactions[stance][pos];
		}
		else
		{
			return 0;
		}
	}
}