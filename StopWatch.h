/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015 SYJourney                                               //
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

using namespace std;

namespace Program
{
	class StopWatch
	{
	public:
		StopWatch()
		{
			start = chrono::steady_clock::now();
		}

		~StopWatch() {}

		short evaluate()
		{
			chrono::steady_clock::time_point time = chrono::steady_clock::now();
			double elapsed = (chrono::duration_cast<chrono::duration<double>>(time - start)).count();
			start = time;
			return static_cast<short>(1000 * elapsed);
		}
	private:
		chrono::steady_clock::time_point start;
	};
}