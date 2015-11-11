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
#include "Obj.h"
#include "nlnx\nx.hpp"

namespace Gameplay
{
	Obj::Obj(node src)
	{
		animation = Animation(nl::nx::map["Obj"][src["oS"] + ".img"][src["l0"]][src["l1"]][src["l2"]]);
		pos = vector2d<int32_t>(src["x"], src["y"]);
		flip = src["f"].get_bool();
		z = src["z"];
	}

	Obj::~Obj() {}

	void Obj::update()
	{
		animation.update();
	}

	void Obj::draw(vector2d<int32_t> viewpos, float inter) const
	{
		using::Graphics::DrawArgument;
		animation.draw(DrawArgument(pos + viewpos, flip), inter);
	}

	uint8_t Obj::getz() const
	{
		return z;
	}
}
