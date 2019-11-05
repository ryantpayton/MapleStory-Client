//////////////////////////////////////////////////////////////////////////////////
//	This file is part of the continued Journey MMORPG client					//
//	Copyright (C) 2015-2019  Daniel Allendorf, Ryan Payton						//
//																				//
//	This program is free software: you can redistribute it and/or modify		//
//	it under the terms of the GNU Affero General Public License as published by	//
//	the Free Software Foundation, either version 3 of the License, or			//
//	(at your option) any later version.											//
//																				//
//	This program is distributed in the hope that it will be useful,				//
//	but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				//
//	GNU Affero General Public License for more details.							//
//																				//
//	You should have received a copy of the GNU Affero General Public License	//
//	along with this program.  If not, see <https://www.gnu.org/licenses/>.		//
//////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "NetConstants.h"

#include "../MapleStory.h"

#include <cstdint>

namespace ms
{
	// Used to encrypt and decrypt packets for communication with the server.
	class Cryptography
	{
	public:
		// Obtain the initialization vector from the handshake.
		Cryptography(const std::int8_t* handshake);
		Cryptography();
		~Cryptography();

		// Encrypt a byte array with the given length and iv.
		void encrypt(std::int8_t* bytes, std::size_t length);
		// Decrypt a byte array with the given length and iv.
		void decrypt(std::int8_t* bytes, std::size_t length);
		// Generate a header for the specified length and key.
		void create_header(std::int8_t* buffer, std::size_t length) const;
		// Use the 4-byte header of a received packet to determine its length.
		std::size_t check_length(const std::int8_t* header) const;

	private:
		// Add the maple custom encryption.
		void mapleencrypt(std::int8_t* bytes, std::size_t length) const;
		// Remove the maple custom encryption.
		void mapledecrypt(std::int8_t* bytes, std::size_t length) const;
		// Update a key.
		void updateiv(std::uint8_t* iv) const;
		// Perform a roll-left operation.
		std::int8_t rollleft(std::int8_t byte, std::size_t count) const;
		// Perform a roll-right operation.
		std::int8_t rollright(std::int8_t byte, std::size_t count) const;

		// Apply aesofb to a byte array.
		void aesofb(std::int8_t* bytes, std::size_t length, std::uint8_t* iv) const;
		// Encrypt a byte array with AES.
		void aesencrypt(std::uint8_t* bytes) const;
		// AES addroundkey step.
		void addroundkey(std::uint8_t* bytes, std::uint8_t round) const;
		// AES subbytes step.
		void subbytes(std::uint8_t* bytes) const;
		// AES shiftrows step.
		void shiftrows(std::uint8_t* bytes) const;
		// Aes mixcolumns step.
		void mixcolumns(std::uint8_t* bytes) const;
		// Perform a gauloise multiplication.
		std::uint8_t gmul(std::uint8_t byte) const;

#ifdef USE_CRYPTO
		std::uint8_t sendiv[HEADER_LENGTH];
		std::uint8_t recviv[HEADER_LENGTH];
#endif
	};
}