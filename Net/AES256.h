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
#include <cstdint>

namespace Net
{
	class AES256
	{
	public:
		AES256();
		~AES256();
		void encrypt(uint8_t*) const;

	private:
		void addroundkey(uint8_t*, uint8_t) const;
		void subbytes(uint8_t*) const;
		void shiftrows(uint8_t*) const;
		void mixcolumns(uint8_t*) const;
		uint8_t gmul(uint8_t) const;
	};
}