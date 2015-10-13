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
//#include "maplelook.h"
#include "CharLook.h"
#include "ptrmap.h"

using namespace Util;
using namespace Character;

namespace Data
{
	class Equipcache
	{
	public:
		Equipcache(){}
		~Equipcache(){}
		void init();
		void loadlook(CharLook*);
		BodyData* getbody(char);
		HairData* gethair(int);
		FaceData* getface(int);
		EquipData* getequip(int);
		EquipData* getnulleq() { return &nullequip; }
	private:
		EquipData nullequip;
		BodyDrawinfo drawinfo;
		ptrmap<char, BodyData*> bodytypes;
		ptrmap<int, HairData*> hairstyles;
		ptrmap<int, FaceData*> faces;
		ptrmap<int, EquipData*> equips;
	};
}

