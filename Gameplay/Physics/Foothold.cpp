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
	Foothold::Foothold(node src, uint8_t ly)
	{
		id = static_cast<uint16_t>(stoi(src.name()));
		prev = src["prev"];
		next = src["next"];
		layer = ly;

		horizontal = Range<int16_t>(src["x1"], src["x2"]);
		vertical = Range<int16_t>(src["y1"], src["y2"]);
	}

	Foothold::Foothold(InPacket& recv, uint8_t ly)
	{
		id = recv.readshort();
		prev = recv.readshort();
		next = recv.readshort();
		layer = ly;
	}

	Foothold::Foothold()
	{
		id = 0;
		prev = 0;
		next = 0;
		layer = 0;
	}

	Foothold::~Foothold() {}

	double Foothold::getslope() const
	{ 
		return iswall() ? 0.0f : static_cast<double>(getvdelta()) / gethdelta();
	}

	double Foothold::resolvex(double x) const
	{ 
		return isfloor() ? vertical.first() : getslope() * (x - horizontal.first()) + vertical.first(); 
	}
}
