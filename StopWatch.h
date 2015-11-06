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
#include <chrono>
#include <stdint.h>

namespace Util
{
	using::std::chrono::high_resolution_clock;

	// Small class for measuring elapsed time between game loops.
	class StopWatch
	{
	public:
		// Start watch at construction.
		StopWatch()
		{
			start();
		}

		~StopWatch() {}

		// Start measurement.
		void start()
		{
			last = high_resolution_clock::now();
		}

		// Return time elapsed since last measurement.
		uint16_t evaluate()
		{
			int64_t elapsed = (std::chrono::duration_cast<std::chrono::milliseconds>(high_resolution_clock::now() - last)).count();
			start();
			return static_cast<uint16_t>(elapsed);
		}
	private:
		high_resolution_clock::time_point last;
	};
}