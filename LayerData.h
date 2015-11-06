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
#include "Tileset.h"
#include "Animation.h"
#include "Ptrmap.h"

namespace Gameplay
{
	using::Graphics::Animation;
	// Tileset and obj data belonging to a map.
	class LayerData
	{
	public:
		LayerData();
		~LayerData();
		void clear();
		void update(uint16_t);
		const Tileset& gettileset(string);
		const Animation& getobj(node);
	private:
		Ptrmap<string, Tileset> tilesets;
		Ptrmap<size_t, Animation> objs;
		Tileset nulltileset;
		Animation nullobj;
	};
}

