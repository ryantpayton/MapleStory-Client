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
#include "Maplayers.h"

namespace Gameplay
{
	Maplayers::Maplayers() {}

	Maplayers::~Maplayers()
	{
		clear();
	}

	void Maplayers::load(node src)
	{
		for (uint8_t i = 0; i < NUM_LAYERS; i++)
		{
			layers[i] = Layer(src[std::to_string(i)], layerdata);
		}
	}

	void Maplayers::clear()
	{
		layerdata.clear();
	}

	void Maplayers::draw(uint8_t layer, vector2d<int32_t> viewpos) const
	{
		if (layers.count(layer)) layers.at(layer).draw(viewpos);
	}

	void Maplayers::update(uint16_t dpf)
	{
		layerdata.update(dpf);
	}
}
