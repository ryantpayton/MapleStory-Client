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
#include "Portal.h"
#include "Net\InPacket.h"
#include "Util\Optional.h"
#include "nlnx\node.hpp"
#include <unordered_map>

namespace Gameplay
{
	using std::unordered_map;
	using nl::node;
	using Net::InPacket;

	// Collecton of portals on a map. Draws and updates portals.
	// Also contains methods for using portals and obtaining spawn points.
	class MapPortals
	{
	public:
		static void init();

		MapPortals(node source, int32_t mapid);
		MapPortals(InPacket& recv);
		MapPortals();
		~MapPortals();

		void update(Point<int16_t> playerpos);
		void draw(Point<int16_t> viewpos, float inter) const;

		Portal::WarpInfo findportal(Point<int16_t> playerpos);

		uint8_t getportalbyname(string name) const;
		Point<int16_t> getspawnpoint(uint8_t id) const;
		Point<int16_t> getspawnpoint(string name) const;

	private:
		static unordered_map<Portal::Type, Animation> animations;

		unordered_map<uint8_t, Portal> portals;
		unordered_map<string, uint8_t> idsbyname;

		static const int16_t WARPCD = 48;
		int16_t cooldown;
	};
}

