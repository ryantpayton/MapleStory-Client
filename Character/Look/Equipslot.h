/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015-2016 Daniel Allendorf                                   //
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
#include "../../Template/Enumeration.h"
#include "../../Template/EnumMap.h"

#include <cstdint>

namespace jrc
{
	namespace Equipslot
	{
		enum Id : int16_t
		{
			NONE = 0, 
			CAP = 1, 
			FACEACC = 2, 
			EYEACC = 3, 
			EARRINGS = 4, 
			TOP = 5, 
			PANTS = 6,
			SHOES = 7, 
			GLOVES = 8,
			CAPE = 9, 
			SHIELD = 10, 
			WEAPON = 11, 
			RING = 12, 
			RING2 = 13,
			RING3 = 15, 
			RING4 = 16, 
			PENDANT = 17, 
			TAMEDMOB = 18, 
			SADDLE = 19, 
			MEDAL = 49, 
			BELT = 50,
			LENGTH
		};

		Id by_id(size_t id);

		constexpr Enumeration<Id> values;
	};
}