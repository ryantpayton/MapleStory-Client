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
#include <chrono>

namespace Util
{
	using std::uint16_t;
	using std::int64_t;
	using std::chrono::high_resolution_clock;
	using std::chrono::milliseconds;

	// Small class for measuring elapsed time between game loops.
	class StopWatch
	{
	public:
		// Start watch at construction.
		StopWatch()
		{
			last = high_resolution_clock::now();
		}

		~StopWatch() {}

		// Return time elapsed since the last measurement.
		int64_t stop()
		{
			int64_t elapsed = (std::chrono::duration_cast
				<milliseconds>(high_resolution_clock::now() - last)).count();
			last = high_resolution_clock::now();
			return elapsed;
		}

	private:
		high_resolution_clock::time_point last;
	};
}