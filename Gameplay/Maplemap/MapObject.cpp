//////////////////////////////////////////////////////////////////////////////
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
#include "MapObject.h"

namespace Gameplay
{
	int8_t MapObject::update(const Physics& physics)
	{
		physics.moveobject(phobj);
		return phobj.fhlayer;
	}

	void MapObject::setposition(int16_t x, int16_t y)
	{
		phobj.fx = static_cast<float>(x);
		phobj.fy = static_cast<float>(y);
	}

	void MapObject::setposition(Point<int16_t> position)
	{
		setposition(position.x(), position.y());
	}

	void MapObject::setactive(bool a)
	{
		active = a;
	}

	bool MapObject::isactive() const
	{
		return active;
	}

	int8_t MapObject::getlayer() const
	{
		return phobj.fhlayer;
	}

	int32_t MapObject::getoid() const
	{
		return oid;
	}

	Point<int16_t> MapObject::getposition() const
	{
		return Point<int16_t>(
			static_cast<int16_t>(phobj.fx),
			static_cast<int16_t>(phobj.fy)
			);
	}

	Point<double> MapObject::getrealposition() const
	{
		return Point<double>(phobj.fx, phobj.fy);
	}

	PhysicsObject& MapObject::getphobj()
	{
		return phobj;
	}
}