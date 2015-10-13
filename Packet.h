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
#include <vector>
#include <cstdint>

using namespace std;

namespace Net
{
	class Packet
	{
	public:
		virtual ~Packet() {}
		virtual void skip(size_t) = 0;
		char* getbytes() { return bytes.data(); }
		size_t length() { return bytes.size(); }
		void writearray(const char* recv, int32_t length)
		{
			for (int32_t i = length - 1; i >= 0; i--)
			{
				bytes.push_back(recv[i]);
			}
		}
	protected:
		vector<char> bytes;
	};
}

