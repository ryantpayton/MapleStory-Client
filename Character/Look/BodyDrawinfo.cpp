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
#include "BodyDrawinfo.h"
#include "Body.h"
#include "nlnx\nx.hpp"
#include "nlnx\node.hpp"

namespace Character
{
	BodyDrawinfo::BodyDrawinfo() {}

	BodyDrawinfo::~BodyDrawinfo() {}

	void BodyDrawinfo::init()
	{
		using nl::node;
		node bodynode = nl::nx::character["00002000.img"];
		node headnode = nl::nx::character["00012000.img"];

		for (node stancenode : bodynode)
		{
			string ststr = stancenode.name();

			uint8_t frame = 0;
			node framenode = stancenode[std::to_string(frame)];
			while (framenode.size() > 0)
			{
				uint16_t delay;
				if (framenode["delay"].data_type() == node::type::integer)
				{
					int16_t sdelay = framenode["delay"];
					delay = (sdelay < 0) ? -sdelay : sdelay;
				}
				else
					delay = 50;

				node actionnode = framenode.resolve("action");
				if (actionnode.data_type() == node::type::string)
				{
					string acstr = actionnode.get_string();
					uint8_t acframe = framenode["frame"];
					Stance::Value acstance = Stance::bystring(acstr);
					bodyactions[ststr][frame] = BodyAction(acstance, acframe, delay);
				}
				else
				{
					Stance::Value stance = Stance::bystring(ststr);
					stancedelays[stance][frame] = delay;

					map<Body::Layer, map<string, vector2d<int16_t>>> bodyshiftmap;
					for (node partnode : framenode)
					{
						string part = partnode.name();
						if (part != "delay" && part != "face")
						{
							string zstr = partnode["z"];
							Body::Layer z = Body::layerbystring(zstr);

							for (node mapnode : partnode["map"])
							{
								bodyshiftmap[z][mapnode.name()] = vector2d<int16_t>(mapnode);
							}
						}
					}

					node headmap = headnode[ststr][std::to_string(frame)]["head"]["map"];
					for (node mapnode : headmap)
					{
						bodyshiftmap[Body::HEAD][mapnode.name()] = vector2d<int16_t>(mapnode);
					}

					bodyposmap[stance][frame] = bodyshiftmap[Body::BODY]["navel"];
					armposmap[stance][frame] = bodyshiftmap.count(Body::ARM) ?
						(bodyshiftmap[Body::ARM]["hand"] - bodyshiftmap[Body::ARM]["navel"]) :
						(bodyshiftmap[Body::ARMOVERHAIR]["hand"] - bodyshiftmap[Body::ARMOVERHAIR]["navel"]);
					handposmap[stance][frame] = bodyshiftmap[Body::LEFTHAND]["handMove"];
					headposmap[stance][frame] = bodyshiftmap[Body::BODY]["neck"] - bodyshiftmap[Body::HEAD]["neck"];
					faceposmap[stance][frame] = bodyshiftmap[Body::BODY]["neck"] - bodyshiftmap[Body::HEAD]["neck"] + bodyshiftmap[Body::HEAD]["brow"];
					hairposmap[stance][frame] = bodyshiftmap[Body::HEAD]["brow"] - bodyshiftmap[Body::HEAD]["neck"] + bodyshiftmap[Body::BODY]["neck"];
				}

				frame++;
				framenode = stancenode[std::to_string(frame)];
			}
		}
	}

	vector2d<int16_t> BodyDrawinfo::getbodypos(Stance::Value stance, uint8_t pos) const
	{
		if (bodyposmap.count(stance))
		{
			if (pos < bodyposmap.at(stance).size())
			{
				return bodyposmap.at(stance).at(pos);
			}
		}
		return vector2d<int16_t>();
	}

	vector2d<int16_t> BodyDrawinfo::getarmpos(Stance::Value stance, uint8_t pos) const
	{
		if (armposmap.count(stance))
		{
			if (pos < armposmap.at(stance).size())
			{
				return armposmap.at(stance).at(pos);
			}
		}
		return vector2d<int16_t>();
	}

	vector2d<int16_t> BodyDrawinfo::gethandpos(Stance::Value stance, uint8_t pos) const
	{
		if (handposmap.count(stance))
		{
			if (pos < handposmap.at(stance).size())
			{
				return handposmap.at(stance).at(pos);
			}
		}
		return vector2d<int16_t>();
	}

	vector2d<int16_t> BodyDrawinfo::getheadpos(Stance::Value stance, uint8_t pos) const
	{
		if (headposmap.count(stance))
		{
			if (pos < headposmap.at(stance).size())
			{
				return headposmap.at(stance).at(pos);
			}
		}
		return vector2d<int16_t>();
	}

	vector2d<int16_t> BodyDrawinfo::gethairpos(Stance::Value stance, uint8_t pos) const
	{
		if (hairposmap.count(stance))
		{
			if (pos < hairposmap.at(stance).size())
			{
				return hairposmap.at(stance).at(pos);
			}
		}
		return vector2d<int16_t>();
	}

	vector2d<int16_t> BodyDrawinfo::getfacepos(Stance::Value stance, uint8_t pos) const
	{
		if (faceposmap.count(stance))
		{
			if (pos < faceposmap.at(stance).size())
			{
				return faceposmap.at(stance).at(pos);
			}
		}
		return vector2d<int16_t>();
	}

	uint8_t BodyDrawinfo::nextframe(Stance::Value stance, uint8_t pos) const
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

	uint16_t BodyDrawinfo::getdelay(Stance::Value stance, uint8_t pos) const
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