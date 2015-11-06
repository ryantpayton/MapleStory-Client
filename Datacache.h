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
#include "Equipcache.h"
#include "Itemcache.h"
//#include "Mobcache.h"
#include "Npccache.h"
//#include "Skillcache.h"
//#include "Effectcache.h"
//#include "Soundcache.h"

namespace Data
{
	class Datacache
	{
	public:
		Datacache();
		~Datacache();
		void init();
		const ItemData& getitem(int32_t);
		Equipcache& getequips() { return equips; }
		Itemcache& getitems() { return items; }
		//Mobcache* getmobs() { return &mobs; }
		Npccache& getnpcs() { return npcs; }
		/*Skillcache* getskills() { return &skills; }
		Effectcache* geteffects() { return &effects; }
		Soundcache* getsounds() { return &sounds; }*/
	private:
		//Mapdata mdata;
		Equipcache equips;
		Itemcache items;
		//Mobcache mobs;
		Npccache npcs;
		/*Skillcache skills;
		Effectcache effects;
		Soundcache sounds;*/
	};
}

