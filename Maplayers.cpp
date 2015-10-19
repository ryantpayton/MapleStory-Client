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
#include "Maplayers.h"

namespace Gameplay
{
	Maplayers::~Maplayers()
	{
		clear();
	}

	void Maplayers::load(node src)
	{
		for (char i = 0; i < 8; i++)
		{
			layers[i] = Layer(src[to_string(i)], cache);
		}
	}

	void Maplayers::clear()
	{
		cache.clear();
	}

	void Maplayers::draw(char layer, vector2d<int> viewpos)
	{
		layers[layer].draw(viewpos);
	}

	void Maplayers::update(short dpf)
	{
		cache.update(dpf);
	}
}
