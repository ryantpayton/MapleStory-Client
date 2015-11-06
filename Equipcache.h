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
#include "CharLook.h"
#include "LookEntry.h"
#include "Ptrmap.h"

namespace Data
{
	using::Util::Ptrmap;
	using::Net::LookEntry;
	using::Character::CharLook;

	class Equipcache
	{
	public:
		Equipcache(){}
		~Equipcache(){}
		void init();
		void loadlook(CharLook&, const LookEntry&);
		BodyData* getbody(uint8_t);
		HairData* gethair(int32_t);
		FaceData* getface(int32_t);
		const EquipData& getequip(int32_t);
	private:
		BodyDrawinfo drawinfo;
		Ptrmap<uint8_t, BodyData> bodytypes;
		Ptrmap<int32_t, HairData> hairstyles;
		Ptrmap<int32_t, FaceData> faces;
		Ptrmap<int32_t, EquipData> equips;
	};
}

