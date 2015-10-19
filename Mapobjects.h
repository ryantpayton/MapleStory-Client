//////////////////////////////////////////////////////////////////////////////
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
#include "Mapobject.h"
#include "Ptrmap.h"
#include <vector>

namespace Gameplay
{
	enum MapobjectType
	{
		MOT_CHAR,
		MOT_NPC,
		MOT_REACTOR,
		MOT_MOB,
		MOT_DROP
	};

	class Mapobjects
	{
	public:
		virtual ~Mapobjects(){}
		virtual void draw(char, vector2d<int>);
		virtual void update(short);
		virtual void clear();
	protected:
		void add(Mapobject*);
		void remove(int);
		Mapobject& get(int);
		Ptrmap<int, Mapobject> objects;
		map<char, map<int, int>> layers;
	};
}

