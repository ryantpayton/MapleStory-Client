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
#include "Gameplay\Maplemap\Portal.h"
#include "Graphics\Animation.h"
#include "Net\InPacket.h"
#include "nlnx\node.hpp"

namespace Gameplay
{
	using std::map;
	using nl::node;
	using Graphics::Animation;
	using Net::InPacket;

	// Collecton of portals on a map. Draws and updates portals.
	// Also contains methods for using portals and obtaining spawn points.
	class MapPortals
	{
	public:
		MapPortals();
		~MapPortals();

		void load(node source, int32_t mapid);
		void parseportals(InPacket& recv, int32_t mapid);

		void clear();
		void update(rectangle2d<int16_t> playerbounds);
		void draw(vector2d<int16_t> viewpos, float inter) const;

		uint8_t getportalbyname(string name) const;
		vector2d<int16_t> getspawnpoint(uint8_t id) const;
		vector2d<int16_t> getspawnpoint(string name) const;
		const WarpInfo* findportal(rectangle2d<int16_t> playerbounds);

	private:
		map<uint8_t, Portal> portals;
		map<string, uint8_t> portalnames;
		uint16_t findportalcd;
	};
}

