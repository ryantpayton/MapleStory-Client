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
#include "Program\TimeConstants.h"
#include "nlnx\nx.hpp"

namespace Gameplay
{
	Mapportals::Mapportals()
	{
		findportalcd = 0;
	}

	Mapportals::~Mapportals() {}

	void Mapportals::init()
	{
		node pnode = nl::nx::map["MapHelper.img"]["portal"]["game"];
		animations[Portal::HIDDEN] = Animation(pnode["ph"]["default"]["portalContinue"]);
		animations[Portal::REGULAR] = Animation(pnode["pv"]);
	}

	void Mapportals::load(node src, int32_t mapid)
	{
		int8_t pid = 0;
		node ptnode = src[std::to_string(pid)];
		while (ptnode.size() > 0)
		{
			Portal::PtType type = static_cast<Portal::PtType>(ptnode["pt"].get_integer());
			string name = ptnode["pn"];
			int32_t targetid = ptnode["tm"];
			string targetname = ptnode["tn"];
			vector2d<int32_t> pos = vector2d<int32_t>(ptnode["x"], ptnode["y"]);

			const Animation* animation;
			if (animations.count(type))
				animation = &animations[type];
			else
				animation = nullptr;

			portals[pid] = Portal(animation, type, name, targetid == mapid, pos, targetid, targetname);
			portalnames[name] = pid;

			pid++;
			ptnode = src[std::to_string(pid)];
		}
	}

	void Mapportals::clear()
	{
		portals.clear();
	}

	void Mapportals::update(rectangle2d<int32_t> player)
	{
		for (map<Portal::PtType, Animation>::iterator anit = animations.begin(); anit != animations.end(); ++anit)
		{
			anit->second.update(Constants::TIMESTEP / 2);
		}
		for (map<uint8_t, Portal>::iterator ptit = portals.begin(); ptit != portals.end(); ++ptit)
		{
			if (ptit->second.gettype() == Portal::HIDDEN)
			{
				ptit->second.settouch(ptit->second.bounds().overlaps(player));
			}
		}

		if (findportalcd > 0)
		{
			findportalcd--;
		}
	}

	void Mapportals::draw(vector2d<int32_t> viewpos, float inter) const
	{
		for (map<uint8_t, Portal>::const_iterator ptit = portals.begin(); ptit != portals.end(); ++ptit)
		{
			ptit->second.draw(viewpos, inter);
		}
	}

	vector2d<int32_t> Mapportals::getspawnpoint(uint8_t pid) const
	{
		return portals.count(pid) ? portals.at(pid).getposition() : vector2d<int32_t>();
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
			for (map<uint8_t, Portal>::iterator ptit = portals.begin(); ptit != portals.end(); ++ptit)
			{
				if (ptit->second.bounds().overlaps(rect))
				{
					findportalcd = 60;
					return ptit->second.getwarpinfo();
				}
			}
		}
		return nullptr;
	}
}