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
#include "VisiblePortal.h"
#include "nx.hpp"

namespace Gameplay
{
	Mapportals::Mapportals()
	{
		findportalcd = 0;
	}

	Mapportals::~Mapportals()
	{
	}

	void Mapportals::init()
	{
		node pnode = nl::nx::map["MapHelper.img"]["portal"]["game"];
		animations.add(PT_HIDDEN, new Animation(pnode["ph"]["default"]["portalContinue"]));
		animations.add(PT_REGULAR, new Animation(pnode["pv"]));
	}

	void Mapportals::load(node src, int32_t mapid)
	{
		int8_t pid = 0;
		node ptnode = src[std::to_string(pid)];
		while (ptnode.size() > 0)
		{
			PortalType type = static_cast<PortalType>(ptnode["pt"].get_integer());
			string name = ptnode["pn"];
			int32_t targetid = ptnode["tm"];
			string targetname = ptnode["tn"];
			vector2d<int32_t> pos = vector2d<int32_t>(ptnode["x"], ptnode["y"]);
			if (animations.contains(type))
			{
				portals.add(pid, new VisiblePortal(*animations.get(type), type, name, targetid, targetid == mapid, targetname, pos));
			}
			else
			{
				portals.add(pid, new Portal(type, name, targetid, targetid == mapid, targetname, pos));
			}
			portalnames[name] = pid;

			pid++;
			ptnode = src[std::to_string(pid)];
		}
	}

	void Mapportals::clear()
	{
		portals.clear();
	}

	void Mapportals::update(rectangle2d<int32_t> player, uint16_t dpf)
	{
		for (map<PortalType, Animation*>::iterator anit = animations.getbegin(); anit != animations.getend(); ++anit)
		{
			anit->second->update(dpf / 2);
		}

		if (findportalcd > 0)
		{
			findportalcd--;
		}
	}

	void Mapportals::draw(vector2d<int32_t> viewpos) const
	{
		for (map<uint8_t, Portal*>::const_iterator ptit = portals.getbegin(); ptit != portals.getend(); ++ptit)
		{
			ptit->second->draw(viewpos);
		}
	}

	vector2d<int32_t> Mapportals::getspawnpoint(uint8_t pid) const
	{
		Portal* portal = portals.get(pid);
		return portal ? portal->getposition() : vector2d<int32_t>();
	}

	vector2d<int32_t> Mapportals::getspawnpoint(string pname) const
	{
		uint8_t pid = portalnames.count(pname) ? portalnames.at(pname) : 0;
		return getspawnpoint(pid);
	}

	const pair<int32_t, string>* Mapportals::findportal(rectangle2d<int32_t> rect)
	{
		if (findportalcd == 0)
		{
			for (map<uint8_t, Portal*>::const_iterator ptit = portals.getbegin(); ptit != portals.getend(); ++ptit)
			{
				if (ptit->second->bounds().overlaps(rect))
				{
					findportalcd = 60;
					return ptit->second->getwarpinfo();
				}
			}
		}
		return nullptr;
	}
}