/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015 Daniel Allendorf                                               //
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
#include "MapObject.h"
#include "Util\rectangle2d.h"

namespace Gameplay
{
	class Drop : public MapObject
	{
	public:
		enum State
		{
			DROPPED,
			FLOATING,
			PICKEDUP
		};

		virtual int8_t update(const Physics& physics) override;

		void init(int8_t);
		void expire(int8_t, const PhysicsObject*);

		rectangle2d<int16_t> bounds() const;

	protected:
		Drop(int32_t oid, int32_t owner, Point<int16_t> start,
			Point<int16_t> dest, int8_t type, int8_t mode, bool playerdrop);

		int32_t owner;
		Point<int16_t> dest;
		int8_t pickuptype;
		bool playerdrop;

		const PhysicsObject* looter;
		State state;
		float opacity;
		double basey;
		double moved;
	};
}

