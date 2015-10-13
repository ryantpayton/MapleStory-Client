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
#include "InPacket.h"
#include "OutPacket.h"

namespace Net
{
	class Cryptography
	{
	public:
		virtual ~Cryptography() {}
		virtual void init(vector<uint8_t>, vector<uint8_t>, uint8_t, uint8_t) = 0;
		virtual void encrypt(OutPacket*) = 0;
		virtual void decrypt(char*, int, bool) = 0;
		virtual int getlength(char*) = 0;
	protected:
		vector<uint8_t> recviv;
		vector<uint8_t> sendiv;
		uint8_t version;
		uint8_t localisation;
	};
}

