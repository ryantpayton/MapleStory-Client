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
#include "Mapportals.h"
#include "Constants.h"
#include "nlnx\nx.hpp"

namespace Gameplay
{
	MapPortals::MapPortals(node src, int32_t mapid)
	{
		for (node sub : src)
		{
			uint8_t pid = 0;
			try 
			{
				string idstr = sub.name();
				pid = static_cast<uint8_t>(std::stoi(idstr));
			}
			catch (const std::exception&)
			{
				continue;
			}

			Portal::Type type = Portal::typebyid(sub["pt"]);
			string name = sub["pn"];
			int32_t targetid = sub["tm"];
			string targetname = sub["tn"];
			Point<int16_t> pos = Point<int16_t>(sub["x"], sub["y"]);

			portals[pid] = Portal(&animations[type], type, name, targetid == mapid, pos, targetid, targetname);
			idsbyname[name] = pid;
		}

		cooldown = WARPCD;
	}

	MapPortals::MapPortals()
	{
		cooldown = WARPCD;
	}

	MapPortals::~MapPortals() {}

	MapPortals::MapPortals(InPacket& recv)
	{
		uint8_t numportals = recv.readbyte();
		for (uint8_t pid = 0; pid < numportals; pid++)
		{
			Portal::Type type = Portal::typebyid(recv.readint());
			string name = recv.readascii();
			int32_t targetid = recv.readint();
			string targetname = recv.readascii();
			Point<int16_t> pos = recv.readpoint();

			portals[pid] = Portal(&animations[type], type, name, targetid == 0, pos, targetid, targetname);
			idsbyname[name] = pid;
		}

		cooldown = WARPCD;
	}

	void MapPortals::update(Point<int16_t> playerpos)
	{
		animations[Portal::REGULAR].update(Constants::TIMESTEP);
		animations[Portal::HIDDEN].update(Constants::TIMESTEP);

		for (auto& ptit : portals)
		{
			if (ptit.second.gettype() == Portal::HIDDEN)
			{
				ptit.second.settouch(ptit.second.bounds().contains(playerpos));
			}
		}

		if (cooldown > 0)
		{
			cooldown--;
		}
	}

	void MapPortals::draw(Point<int16_t> viewpos, float inter) const
	{
		for (auto& ptit : portals)
		{
			ptit.second.draw(viewpos, inter);
		}
	}

	Point<int16_t> MapPortals::getspawnpoint(uint8_t pid) const
	{
		if (portals.count(pid))
			return portals.at(pid).getposition() - Point<int16_t>(0, 30);
		else
			return Point<int16_t>();
	}

	Point<int16_t> MapPortals::getspawnpoint(string pname) const
	{
		uint8_t pid = idsbyname.count(pname) ? idsbyname.at(pname) : 0;
		return getspawnpoint(pid);
	}

	Portal::WarpInfo MapPortals::findportal(Point<int16_t> playerpos)
	{
		if (cooldown == 0)
		{
			cooldown = WARPCD;

			for (auto& ptit : portals)
			{
				const Portal& portal = ptit.second;
				if (portal.bounds().contains(playerpos))
				{
					return portal.getwarpinfo();
				}
			}
		}
		return Portal::WarpInfo();
	}

	void MapPortals::init()
	{
		node src = nl::nx::map["MapHelper.img"]["portal"]["game"];

		animations[Portal::HIDDEN] = src["ph"]["default"]["portalContinue"];
		animations[Portal::REGULAR] = src["pv"];
	}
	unordered_map<Portal::Type, Animation> MapPortals::animations;
}