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
#include <string>
#include <cstdint>

using namespace std;

namespace Util
{
	class Sha2
	{
	public:
		Sha2();
		~Sha2();
		string gethash(const char*, size_t);
	private:
		void process(uint8_t*);
		uint32_t rotr(uint32_t, size_t);
		uint32_t rotl(uint32_t, size_t);
		uint32_t shfr(uint32_t, size_t);
		uint32_t* hash;
	};
}