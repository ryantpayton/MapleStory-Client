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
#include "Util\Singleton.h"
#include <cstdint>
#include <chrono>

using std::chrono::high_resolution_clock;
using std::chrono::microseconds;

// Small class for measuring elapsed time between game loops.
class Timer : public Singleton<Timer>
{
public:
	Timer() 
	{
		start();
	}

	~Timer() {}

	// Start the timer by setting the last measurement to now.
	void start()
	{
		last = high_resolution_clock::now();
	}

	// Return time elapsed since the last measurement.
	double stop()
	{
		double elapsed = static_cast<double>((std::chrono::duration_cast
			<microseconds>(high_resolution_clock::now() - last)).count());
		last = high_resolution_clock::now();
		return elapsed;
	}

private:
	high_resolution_clock::time_point last;
};