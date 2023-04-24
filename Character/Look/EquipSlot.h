//////////////////////////////////////////////////////////////////////////////////
//	This file is part of the continued Journey MMORPG client					//
//	Copyright (C) 2015-2019  Daniel Allendorf, Ryan Payton						//
//																				//
//	This program is free software: you can redistribute it and/or modify		//
//	it under the terms of the GNU Affero General Public License as published by	//
//	the Free Software Foundation, either version 3 of the License, or			//
//	(at your option) any later version.											//
//																				//
//	This program is distributed in the hope that it will be useful,				//
//	but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				//
//	GNU Affero General Public License for more details.							//
//																				//
//	You should have received a copy of the GNU Affero General Public License	//
//	along with this program.  If not, see <https://www.gnu.org/licenses/>.		//
//////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "../../Template/Enumeration.h"
#include "../../Template/EnumMap.h"

namespace ms
{
	namespace EquipSlot
	{
		enum Id : int16_t
		{
			NONE,
			HAT,
			FACE,
			EYEACC,
			EARACC,
			TOP,
			BOTTOM,
			SHOES,
			GLOVES,
			CAPE,
			SHIELD,		// TODO: Where is this now?
			WEAPON,
			RING1,
			RING2,
			RING3 = 15,
			RING4,
			PENDANT1,
			TAMEDMOB,	// TODO: Where is this now?
			SADDLE,		// TODO: Where is this now?
			MEDAL = 49,
			BELT = 50,
			POCKET,		// TODO: What is the proper value for this?
			BOOK,		// TODO: What is the proper value for this?
			PENDANT2,	// TODO: What is the proper value for this?
			SHOULDER,	// TODO: What is the proper value for this?
			ANDROID,	// TODO: What is the proper value for this?
			EMBLEM,		// TODO: What is the proper value for this?
			BADGE,		// TODO: What is the proper value for this?
			SUBWEAPON,	// TODO: What is the proper value for this?
			HEART,		// TODO: What is the proper value for this?
			TOP_DEFAULT,
			BOTTOM_DEFAULT,
			LENGTH
		};

		Id by_id(size_t id);

		constexpr Enumeration<Id> values;
	};
}
