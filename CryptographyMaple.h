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
#include "Cryptography.h"
#include "AES.h"
#include <mutex>

namespace Net
{
	class CryptographyMaple : public Cryptography
	{
	public:
		CryptographyMaple() {}
		~CryptographyMaple() {}
		void init(vector<uint8_t>, vector<uint8_t>, uint8_t, uint8_t);
		void encrypt(OutPacket&);
		void decrypt(char*, int, bool);
		int getlength(char*);
		void mapleencrypt(char*, int);
		void mapledecrypt(char*, int);
		void aescrypt(char*, int, bool);
		char rollleft(char, int);
		char rollright(char, int);
		void updateiv(bool);
	private:
		AES cipher;
		mutex decryptlock;
	};
}

