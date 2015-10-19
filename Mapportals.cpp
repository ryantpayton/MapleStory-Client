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
#include "Mapportals.h"
#include "VisiblePortal.h"
#include "nx.hpp"

namespace Gameplay
{
	Mapportals::Mapportals()
	{
		animhidden = 0;
		animregular = 0;
	}

	Mapportals::~Mapportals()
	{
		if (animhidden != 0)
		{
			delete animhidden;
		}
		if (animregular != 0)
		{
			delete animregular;
		}
	}

	void Mapportals::init()
	{
		node pnode = nx::map["MapHelper.img"]["portal"]["game"];
		animhidden = new Animation(pnode["ph"]["default"]["portalContinue"]);
		animregular = new Animation(pnode["pv"]);
	}

	void Mapportals::load(node src, int mapid)
	{
		char pid = 0;
		node ptnode = src[to_string(pid)];
		while (ptnode.size() > 0)
		{
			PortalType type = static_cast<PortalType>(ptnode["pt"].get_integer());
			string name = ptnode["pn"];
			int targetid = ptnode["tm"];
			string targetname = ptnode["tn"];
			vector2d<int> pos = vector2d<int>(ptnode["x"], ptnode["y"]);
			switch (type)
			{
			case PT_REGULAR:
				portals.add(pid, new VisiblePortal(*animregular, type, name, targetid, targetid == mapid, targetname, pos));
				break;
			case PT_HIDDEN:
				portals.add(pid, new VisiblePortal(*animhidden, type, name, targetid, targetid == mapid, targetname, pos));
				break;
			default:
				portals.add(pid, new Portal(type, name, targetid, targetid == mapid, targetname, pos));
			}
			portalnames[name] = pid;

			pid++;
			ptnode = src[to_string(pid)];
		}
	}

	void Mapportals::clear()
	{
		portals.clear();
	}

	void Mapportals::draw(vector2d<int> viewpos)
	{
		for (map<char, Portal*>::iterator ptit = portals.getbegin(); ptit != portals.getend(); ++ptit)
		{
			ptit->second->draw(viewpos);
		}
	}

	void Mapportals::update(rectangle2d<int> player, short dpf)
	{
		animhidden->update(dpf / 2);
		animregular->update(dpf / 2);
	}

	vector2d<int> Mapportals::getspawnpoint(char pid)
	{
		Portal* portal = portals.get(pid);
		if (portal != 0)
		{
			return portal->getposition();
		}
		else
		{
			return vector2d<int>();
		}
	}

	vector2d<int> Mapportals::getspawnpoint(string pname)
	{
		char pid = portalnames.count(pname) ? portalnames[pname] : 0;
		Portal* portal = portals.get(pid);
		if (portal != 0)
		{
			return portal->getposition();
		}
		else
		{
			return vector2d<int>();
		}
	}

	pair<int, string> Mapportals::findportal(rectangle2d<int> rect)
	{
		for (map<char, Portal*>::iterator ptit = portals.getbegin(); ptit != portals.getend(); ++ptit)
		{
			if (ptit->second->bounds().overlaps(rect))
			{
				return ptit->second->getwarpinfo();
			}
		}
		return make_pair(-1, "");
	}
}