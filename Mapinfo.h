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
#include "vector2d.h"
#include "node.hpp"
#include <vector>

using namespace std;
using namespace nl;
using namespace Util;

namespace Gameplay
{
	class Mapinfo
	{
	public:
		Mapinfo(node, vector2d<int>, vector2d<int>);
		Mapinfo() {}
		~Mapinfo() {}
		string getbgm() const;
		vector2d<int> getwalls() const;
		vector2d<int> getborders() const;
		const vector2d<int>* getseat(vector2d<int>) const;
	private:
		int fieldlimit;
		bool cloud;
		string bgm;
		string mapdesc;
		string mapname;
		string streetname;
		string mapmark;
		bool swim;
		bool town;
		bool hideminimap;
		vector2d<int> mapwalls;
		vector2d<int> mapborders;
		vector<vector2d<int>> seats;
	};
}