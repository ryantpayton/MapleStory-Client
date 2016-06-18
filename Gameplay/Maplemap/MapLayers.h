/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015-2016 Daniel Allendorf                                   //
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
#include "Tile.h"
#include "Obj.h"

#include <vector>
#include <map>

namespace jrc
{
	class Layer
	{
	public:
		Layer(nl::node src);
		Layer();

		void draw(Point<int16_t> viewpos, float alpha) const;
		void update();

	private:
		std::map<uint8_t, std::vector<Tile>> tiles;
		std::map<uint8_t, std::vector<Obj>> objs;
	};


	class MapLayers
	{
	public:
		static const uint8_t NUM_LAYERS = 8;

		MapLayers(nl::node src);
		MapLayers();

		void draw(uint8_t layer, Point<int16_t> viewpos, float alpha) const;
		void update();

	private:
		Layer layers[NUM_LAYERS];
	};
}

