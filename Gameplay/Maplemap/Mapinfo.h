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
#include "Util\vector2d.h"
#include "nlnx\node.hpp"
#include <vector>

namespace Gameplay
{
	using::std::int32_t;
	using::std::vector;
	using::std::string;
	using::nl::node;
	using::Util::vector2d;

	class Mapinfo
	{
	public:
		Mapinfo();
		~Mapinfo() {}
		void loadinfo(node, vector2d<int32_t>, vector2d<int32_t>);
		bool hasnewbgm() const;
		const string& getbgm() const;
		vector2d<int32_t> getwalls() const;
		vector2d<int32_t> getborders() const;
		const vector2d<int32_t>* getseat(vector2d<int>) const;
	private:
		bool newbgm;
		int32_t fieldlimit;
		bool cloud;
		string bgm;
		string mapdesc;
		string mapname;
		string streetname;
		string mapmark;
		bool swim;
		bool town;
		bool hideminimap;
		vector2d<int32_t> mapwalls;
		vector2d<int32_t> mapborders;
		vector<vector2d<int32_t>> seats;
	};
}