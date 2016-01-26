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
#include "Graphics\Animation.h"
#include "Util\vector2d.h"
#include "Net\InPacket.h"

namespace Gameplay
{
	using nl::node;
	using Graphics::Animation;
	using Net::InPacket;

	// Represents an obj (map decoration) on a map.
	class Obj
	{
	public:
		Obj(node source);
		Obj(InPacket& recv);
		// Empty destructor.
		~Obj();

		// Update animation.
		void update();
		// Draw the obj at the specified position.
		void draw(vector2d<int16_t> viewpos, float inter) const;
		// Return depth of the obj.
		uint8_t getz() const;

	private:
		Animation animation;
		vector2d<int16_t> pos;
		uint8_t z;
		bool flip;
	};
}
