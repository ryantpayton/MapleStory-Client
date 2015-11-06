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
#include "LayerData.h"
#include "Tile.h"
#include "Obj.h"
#include <map>

namespace Gameplay
{
	using::std::vector;
	using::std::map;

	class Layer
	{
	public:
		Layer(node, LayerData&);
		Layer();
		~Layer();
		void draw(vector2d<int32_t>) const;
	private:
		map<uint8_t, vector<Tile>> tiles;
		map<uint8_t, vector<Obj>> objs;
	};
}

