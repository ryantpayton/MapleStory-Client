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
#include "Mob.h"

namespace Gameplay
{
	Mob::Mob(int32_t oi, int32_t i, bool c, int8_t st, uint16_t fh, int8_t eff, bool fd, int8_t tm, int32_t x, int32_t y)
	{
		oid = oi;
		id = i;
		control = c;
		stance = st;
		team = tm;

		phobj.fhid = fh;
		setposition(x, y);
	}

	Mob::~Mob() {}

	int8_t Mob::update(const Physics& physics)
	{
		return phobj.fhlayer;
	}

	void Mob::draw(vector2d<int32_t> pos, float inter) const
	{

	}

	void Mob::setposition(int32_t x, int32_t y)
	{
		phobj.fx = static_cast<float>(x);
		phobj.fy = static_cast<float>(y);
	}

	int8_t Mob::getlayer() const
	{
		return phobj.fhlayer;
	}

	int32_t Mob::getoid() const
	{
		return oid;
	}

	vector2d<int32_t> Mob::getposition() const
	{
		return vector2d<int32_t>(static_cast<int32_t>(phobj.fx), static_cast<int32_t>(phobj.fy));
	}
}
