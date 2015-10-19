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
#include "Portal.h"
#include "Ptrmap.h"

namespace Gameplay
{
	class Mapportals
	{
	public:
		Mapportals();
		~Mapportals();
		void init();
		void load(node, int);
		void clear();
		void draw(vector2d<int>);
		void update(rectangle2d<int>, short);
		vector2d<int> getspawnpoint(char);
		vector2d<int> getspawnpoint(string);
		pair<int, string> findportal(rectangle2d<int>);
	private:
		Ptrmap<char, Portal> portals;
		map<string, char> portalnames;
		Animation* animhidden;
		Animation* animregular;
	};
}

