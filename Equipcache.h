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
#include "CharLook.h"
#include "LookEntry.h"
#include "Ptrmap.h"

using namespace Util;
using namespace Character;
using namespace Net;

namespace Data
{
	class Equipcache
	{
	public:
		Equipcache(){}
		~Equipcache(){}
		void init();
		void loadlook(CharLook&, const LookEntry&);
		BodyData* getbody(char);
		HairData* gethair(int);
		FaceData* getface(int);
		const EquipData& getequip(int);
	private:
		BodyDrawinfo drawinfo;
		Ptrmap<char, BodyData> bodytypes;
		Ptrmap<int, HairData> hairstyles;
		Ptrmap<int, FaceData> faces;
		Ptrmap<int, EquipData> equips;
	};
}

