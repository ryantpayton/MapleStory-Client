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
#include <cstdint>
#include <map>

namespace Character
{
	using::std::int8_t;
	using::std::int16_t;
	using::std::int32_t;
	using::std::map;
	// Class that represents the monster card collection of an individual character.
	class Monsterbook
	{
	public:
		Monsterbook();
		~Monsterbook();
		void setcover(int32_t);
		void addcard(int16_t, int8_t);
	private:
		int32_t cover;
		map<int16_t, int8_t> cards;
	};
}

