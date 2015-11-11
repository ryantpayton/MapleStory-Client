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
#include "Foothold.h"

namespace Gameplay
{
	Foothold::Foothold(uint16_t i, int8_t l, node src)
	{
		id = i;
		layer = l;
		prev = src["prev"];
		next = src["next"];
		horizontal = vector2d<int32_t>(src["x1"], src["x2"]);
		vertical = vector2d<int32_t>(src["y1"], src["y2"]);
	}

	Foothold::Foothold()
	{
		id = 0;
		layer = -1;
		prev = 0;
		next = 0;
	}

	uint16_t Foothold::getid() const
	{
		return id;
	}

	int8_t Foothold::getlayer() const
	{
		return layer;
	}

	uint16_t Foothold::getprev() const
	{
		return prev;
	}

	uint16_t Foothold::getnext() const
	{
		return next;
	}

	vector2d<int32_t> Foothold::gethor() const
	{
		return horizontal;
	}

	vector2d<int32_t> Foothold::getver() const
	{
		return vertical;
	}

	int32_t Foothold::getl() const
	{
		return horizontal.smaller();
	}

	int32_t Foothold::getr() const
	{
		return horizontal.greater();
	}

	int32_t Foothold::gett() const
	{
		return vertical.smaller();
	}

	int32_t Foothold::getb() const
	{
		return vertical.greater();
	}

	bool Foothold::iswall() const
	{ 
		return horizontal.x() == horizontal.y(); 
	}

	bool Foothold::isfloor() const
	{ 
		return vertical.x() == vertical.y(); 
	}

	bool Foothold::hcontains(int32_t x) const
	{ 
		return horizontal.contains(x); 
	}

	bool Foothold::vcontains(int32_t y) const
	{ 
		return vertical.contains(y); 
	}

	int32_t Foothold::gethdelta() const
	{ 
		return horizontal.y() - horizontal.x(); 
	}

	int32_t Foothold::getvdelta() const
	{ 
		return vertical.y() - vertical.x(); 
	}

	float Foothold::getslope() const
	{ 
		return iswall() ? 0 : static_cast<float>(getvdelta()) / gethdelta(); 
	}

	float Foothold::resolvex(float x) const
	{ 
		return isfloor() ? vertical.x() : getslope() * (x - horizontal.x()) + vertical.x(); 
	}

	vector2d<int32_t> Foothold::getledge() const
	{ 
		return vector2d<int32_t>(horizontal.x() - 1, vertical.x());
	}

	vector2d<int32_t> Foothold::getredge() const
	{ 
		return vector2d<int32_t>(horizontal.y() + 1, vertical.y());
	}
}
