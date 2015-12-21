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
#include <string>
#include <map>

namespace Character
{
	using::std::int16_t;
	using::std::int64_t;
	using::std::string;
	using::std::map;
	using::std::pair;

	// Class that stores information on the questlog of an individual character.
	class Questlog
	{
	public:
		Questlog();
		~Questlog();

		void addstarted(int16_t, string);
		void addprogress(int16_t, int16_t, string);
		void addcompleted(int16_t, int64_t);
		bool isstarted(int16_t);
		int16_t getlaststarted();

	private:
		map<int16_t, string> started;
		map<int16_t, pair<int16_t, string>> progress;
		map<int16_t, int64_t> completed;
	};
}

