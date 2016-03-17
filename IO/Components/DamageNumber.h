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
#include "Charset.h"
#include "Gameplay\Physics\PhysicsObject.h"
#include "Util\Interpolated.h"
#include "Util\Point.h"

namespace IO
{
	using std::pair;
	using Gameplay::MovingObject;

	class DamageNumber
	{
	public:
		static const size_t NUM_TYPES = 3;
		enum Type
		{
			NORMAL,
			CRITICAL,
			TOPLAYER
		};

		DamageNumber(Type type, int32_t damage, Point<int16_t> position);
		DamageNumber();
		~DamageNumber();

		void draw(Point<int16_t> parentpos, float alpha) const;
		bool update();

		static void init();

	private:
		int16_t getadvance(char c, bool first) const;

		Type type;
		bool miss;
		bool multiple;
		int8_t firstnum;
		string restnum;
		int16_t shift;
		MovingObject moveobj;
		Linear<float> opacity;

		static pair<Charset, Charset> charsets[NUM_TYPES];
	};
}

