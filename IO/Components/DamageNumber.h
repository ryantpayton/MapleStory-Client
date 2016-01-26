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
#include "Util\vector2d.h"

namespace IO
{
	class DamageNumber
	{
	public:
		enum Type
		{
			NORMAL,
			CRITICAL,
			TOPLAYER
		};

		static void init();

		DamageNumber(Type type, int damage, float alpha, vector2d<int16_t> position);
		DamageNumber();
		~DamageNumber();

		void draw(vector2d<int16_t> viewpos) const;
		bool update();

	private:
		Type type;
		bool miss;
		int8_t firstnum;
		string restnum;
		int16_t shift;
		float fx;
		float fy;
		float alpha;
	};
}

