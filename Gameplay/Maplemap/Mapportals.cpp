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
#include "Mapportals.h"
#include "Program\Constants.h"
#include "nlnx\nx.hpp"

namespace Gameplay
{
	Animation* getanimation(Portal::PtType type)
	{
		static std::map<Portal::PtType, Animation> animations;
		if (!animations.count(type))
		{
			node pnode = nl::nx::map["MapHelper.img"]["portal"]["game"];
			switch (type)
			{
			case Portal::HIDDEN:
				animations[Portal::HIDDEN] = Animation(pnode["ph"]["default"]["portalContinue"]);
				break;
			case Portal::REGULAR:
				animations[Portal::REGULAR] = Animation(pnode["pv"]);
				break;
			default:
				return nullptr;
			}
		}
		return &animations[type];
	}

	MapPortals::MapPortals()
	{
		findportalcd = 0;
	}

	MapPortals::~MapPortals() {}

	void MapPortals::load(node src, int32_t mapid)
	{
		int8_t pid = 0;
		node ptnode = src[std::to_string(pid)];
		while (ptnode.size() > 0)
		{
			Portal::PtType type = static_cast<Portal::PtType>(ptnode["pt"].get_integer());
			string name = ptnode["pn"];
			int32_t targetid = ptnode["tm"];
			string targetname = ptnode["tn"];
			vector2d<int16_t> pos = vector2d<int16_t>(ptnode["x"], ptnode["y"]);

			portals[pid] = Portal(getanimation(type), type, name, targetid == mapid, pos, targetid, targetname);
			portalnames[name] = pid;

			pid++;
			ptnode = src[std::to_string(pid)];
		}
	}

	void MapPortals::parseportals(InPacket& recv, int32_t mapid)
	{
		uint8_t numportals = recv.readbyte();
		for (uint8_t pid = 0; pid < numportals; pid++)
		{
			Portal::PtType type = static_cast<Portal::PtType>(recv.readint());
			string name = recv.readascii();
			int32_t targetid = recv.readint();
			string targetname = recv.readascii();
			vector2d<int16_t> pos = recv.readpoint();

			portals[pid] = Portal(getanimation(type), type, name, targetid == mapid, pos, targetid, targetname);
			portalnames[name] = pid;
		}
	}

	void MapPortals::clear()
	{
		portals.clear();
	}

	void MapPortals::update(rectangle2d<int16_t> player)
	{
		getanimation(Portal::REGULAR)->update(Constants::TIMESTEP / 2);
		getanimation(Portal::HIDDEN)->update(Constants::TIMESTEP / 2);

		for (auto& ptit : portals)
		{
			if (ptit.second.gettype() == Portal::HIDDEN)
			{
				ptit.second.settouch(ptit.second.bounds().overlaps(player));
			}
		}

		if (findportalcd > 0)
		{
			findportalcd--;
		}
	}

	void MapPortals::draw(vector2d<int16_t> viewpos, float inter) const
	{
		for (auto& ptit : portals)
		{
			ptit.second.draw(viewpos, inter);
		}
	}

	vector2d<int16_t> MapPortals::getspawnpoint(uint8_t pid) const
	{
		if (portals.count(pid))
			return portals.at(pid).getposition() - vector2d<int16_t>(0, 40);
		else
			return vector2d<int16_t>();
	}

	vector2d<int16_t> MapPortals::getspawnpoint(string pname) const
	{
		uint8_t pid = portalnames.count(pname) ? portalnames.at(pname) : 0;
		return getspawnpoint(pid);
	}

	const WarpInfo* MapPortals::findportal(rectangle2d<int16_t> rect)
	{
		if (findportalcd)
			return nullptr;

		for (auto& ptit : portals)
		{
			if (ptit.second.bounds().overlaps(rect))
			{
				findportalcd = 60;
				return &ptit.second.getwarpinfo();
			}
		}
		return nullptr;
	}
}