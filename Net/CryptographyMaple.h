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
#include "AES256.h"

namespace Net
{
	// Used to encrypt and decrypt packets for communication with the server.
	class CryptographyMaple
	{
	public:
		CryptographyMaple();
		~CryptographyMaple();
		// Encrypt a byte array with the given length and iv.
		// Parameters: int8_t*(bytes to encrypt), size_t(length), uint8_t*(sendiv)
		void encrypt(int8_t*, size_t, uint8_t*) const;
		// Decrypt a byte array with the given length and iv.
		// Parameters: int8_t*(bytes to encrypt), size_t(length), uint8_t*(recviv)
		void decrypt(int8_t*, size_t, uint8_t*) const;
		// Use the 4-byte header of a received packet to determine its length.
		// Parameters: int8_t*(header)
		size_t getlength(const int8_t*) const;

	private:
		void mapleencrypt(int8_t*, size_t) const;
		void mapledecrypt(int8_t*, size_t) const;
		void aesofb(int8_t*, size_t, uint8_t*) const;
		void updateiv(uint8_t*) const;
		int8_t rollleft(int8_t, size_t) const;
		int8_t rollright(int8_t, size_t) const;
		AES256 cipher;
	};
}

