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
#include "CryptographyMaple.h"

namespace Net
{
	CryptographyMaple::CryptographyMaple() {}

	CryptographyMaple::~CryptographyMaple() {}

	void CryptographyMaple::encrypt(int8_t* bytes, size_t packetlen, uint8_t* sendiv) const
	{
		static const uint8_t MAPLEVERSION = 83;

		size_t a = ((sendiv[3] << 8) | sendiv[2]) ^ MAPLEVERSION;
		size_t b = a ^ packetlen;
		bytes[0] = static_cast<int8_t>(a % 0x100);
		bytes[1] = static_cast<int8_t>(a / 0x100);
		bytes[2] = static_cast<int8_t>(b % 0x100);
		bytes[3] = static_cast<int8_t>(b / 0x100);
		mapleencrypt(bytes + 4, packetlen);
		aesofb(bytes + 4, packetlen, sendiv);
	}

	void CryptographyMaple::decrypt(int8_t* bytes, size_t length, uint8_t* recviv) const
	{
		aesofb(bytes, length, recviv);
		mapledecrypt(bytes, length);
	}

	size_t CryptographyMaple::getlength(const int8_t* bytes) const
	{
		uint32_t headermask = 0;
		for (size_t i = 0; i < 4; i++)
		{
			headermask |= static_cast<uint8_t>(bytes[i]) << (8 * i);
		}
		return static_cast<int16_t>((headermask >> 16) ^ (headermask & 0xFFFF));
	}

	void CryptographyMaple::mapleencrypt(int8_t* bytes, size_t length) const
	{
		for (size_t j = 0; j < 3; j++) 
		{
			int8_t remember = 0;
			int8_t datalen = static_cast<int8_t>(length & 0xFF);
			for (size_t i = 0; i < length; i++)
			{
				int8_t cur = (rollleft(bytes[i], 3) + datalen) ^ remember;
				remember = cur;
				cur = rollright(cur, static_cast<int32_t>(datalen) & 0xFF);
				bytes[i] = static_cast<int8_t>((~cur) & 0xFF) + 0x48;
				datalen--;
			}

			remember = 0;
			datalen = static_cast<int8_t>(length & 0xFF);
			for (size_t i = length; i--;)
			{
				int8_t cur = (rollleft(bytes[i], 4) + datalen) ^ remember;
				remember = cur;
				bytes[i] = rollright(cur ^ 0x13, 3);
				datalen--;
			}
		}
	}

	void CryptographyMaple::mapledecrypt(int8_t* bytes, size_t length) const
	{
		for (size_t i = 0; i < 3; i++)
		{
			uint8_t remember = 0;
			uint8_t datalen = static_cast<uint8_t>(length & 0xFF);

			for (size_t i = length; i--;)
			{
				uint8_t cur = rollleft(bytes[i], 3) ^ 0x13;
				bytes[i] = rollright((cur ^ remember) - datalen, 4);
				remember = cur;
				datalen--;
			}

			remember = 0;
			datalen = static_cast<uint8_t>(length & 0xFF);
			for (size_t i = 0; i < length; i++)
			{
				uint8_t cur = (~(bytes[i] - 0x48)) & 0xFF;
				cur = rollleft(cur, static_cast<int32_t>(datalen) & 0xFF);
				bytes[i] = rollright((cur ^ remember) - datalen, 3);
				remember = cur;
				datalen--;
			}
		}
	}

	void CryptographyMaple::aesofb(int8_t* bytes, size_t length, uint8_t* iv) const
	{
		size_t blocklength = 0x5B0;
		size_t offset = 0;
		while (offset < length)
		{
			uint8_t miv[16];
			for (size_t i = 0; i < 16; i++)
			{
				miv[i] = iv[i % 4];
			}

			size_t remaining = length - offset;
			if (remaining > blocklength)
			{
				remaining = blocklength;
			}

			for (size_t x = 0; x < remaining; x++)
			{
				size_t relpos = x % 16;
				if (relpos == 0)
				{
					cipher.encrypt(miv);
				}
				bytes[x + offset] ^= miv[relpos];
			}

			offset += blocklength;
			blocklength = 0x5B4;
		}

		updateiv(iv);
	}

	void CryptographyMaple::updateiv(uint8_t* iv) const
	{
		static const uint8_t maplebytes[256] =
		{
			0xEC, 0x3F, 0x77, 0xA4, 0x45, 0xD0, 0x71, 0xBF, 0xB7, 0x98, 0x20, 0xFC, 0x4B, 0xE9, 0xB3, 0xE1,
			0x5C, 0x22, 0xF7, 0x0C, 0x44, 0x1B, 0x81, 0xBD, 0x63, 0x8D, 0xD4, 0xC3, 0xF2, 0x10, 0x19, 0xE0,
			0xFB, 0xA1, 0x6E, 0x66, 0xEA, 0xAE, 0xD6, 0xCE, 0x06, 0x18, 0x4E, 0xEB, 0x78, 0x95, 0xDB, 0xBA,
			0xB6, 0x42, 0x7A, 0x2A, 0x83, 0x0B, 0x54, 0x67, 0x6D, 0xE8, 0x65, 0xE7, 0x2F, 0x07, 0xF3, 0xAA,
			0x27, 0x7B, 0x85, 0xB0, 0x26, 0xFD, 0x8B, 0xA9, 0xFA, 0xBE, 0xA8, 0xD7, 0xCB, 0xCC, 0x92, 0xDA,
			0xF9, 0x93, 0x60, 0x2D, 0xDD, 0xD2, 0xA2, 0x9B, 0x39, 0x5F, 0x82, 0x21, 0x4C, 0x69, 0xF8, 0x31,
			0x87, 0xEE, 0x8E, 0xAD, 0x8C, 0x6A, 0xBC, 0xB5, 0x6B, 0x59, 0x13, 0xF1, 0x04, 0x00, 0xF6, 0x5A,
			0x35, 0x79, 0x48, 0x8F, 0x15, 0xCD, 0x97, 0x57, 0x12, 0x3E, 0x37, 0xFF, 0x9D, 0x4F, 0x51, 0xF5,
			0xA3, 0x70, 0xBB, 0x14, 0x75, 0xC2, 0xB8, 0x72, 0xC0, 0xED, 0x7D, 0x68, 0xC9, 0x2E, 0x0D, 0x62,
			0x46, 0x17, 0x11, 0x4D, 0x6C, 0xC4, 0x7E, 0x53, 0xC1, 0x25, 0xC7, 0x9A, 0x1C, 0x88, 0x58, 0x2C,
			0x89, 0xDC, 0x02, 0x64, 0x40, 0x01, 0x5D, 0x38, 0xA5, 0xE2, 0xAF, 0x55, 0xD5, 0xEF, 0x1A, 0x7C,
			0xA7, 0x5B, 0xA6, 0x6F, 0x86, 0x9F, 0x73, 0xE6, 0x0A, 0xDE, 0x2B, 0x99, 0x4A, 0x47, 0x9C, 0xDF,
			0x09, 0x76, 0x9E, 0x30, 0x0E, 0xE4, 0xB2, 0x94, 0xA0, 0x3B, 0x34, 0x1D, 0x28, 0x0F, 0x36, 0xE3,
			0x23, 0xB4, 0x03, 0xD8, 0x90, 0xC8, 0x3C, 0xFE, 0x5E, 0x32, 0x24, 0x50, 0x1F, 0x3A, 0x43, 0x8A,
			0x96, 0x41, 0x74, 0xAC, 0x52, 0x33, 0xF0, 0xD9, 0x29, 0x80, 0xB1, 0x16, 0xD3, 0xAB, 0x91, 0xB9,
			0x84, 0x7F, 0x61, 0x1E, 0xCF, 0xC5, 0xD1, 0x56, 0x3D, 0xCA, 0xF4, 0x05, 0xC6, 0xE5, 0x08, 0x49
		};

		uint8_t mbytes[4] = 
		{ 
			0xF2, 0x53, 0x50, 0xC6 
		};

		for (size_t i = 0; i < 4; i++)
		{
			uint8_t ivbyte = iv[i];
			mbytes[0] += maplebytes[mbytes[1] & 0xFF] - ivbyte;
			mbytes[1] -= mbytes[2] ^ maplebytes[ivbyte & 0xFF] & 0xFF;
			mbytes[2] ^= maplebytes[mbytes[3] & 0xFF] + ivbyte;
			mbytes[3] += (maplebytes[ivbyte & 0xFF] & 0xFF) - (mbytes[0] & 0xFF);

			size_t mask = 0;
			mask |= (mbytes[0]) & 0xFF;
			mask |= (mbytes[1] << 8) & 0xFF00;
			mask |= (mbytes[2] << 16) & 0xFF0000;
			mask |= (mbytes[3] << 24) & 0xFF000000;
			mask = (mask >> 0x1D) | (mask << 3);
			for (size_t i = 0; i < 4; i++)
			{
				size_t value = mask >> (8 * i);
				mbytes[i] = static_cast<uint8_t>(value & 0xFF);
			}
		}

		for (size_t i = 0; i < 4; i++)
		{
			iv[i] = mbytes[i];
		}
	}

	int8_t CryptographyMaple::rollleft(int8_t data, size_t count) const
	{
		int32_t mask = (data & 0xFF) << (count % 8);
		return static_cast<int8_t>((mask & 0xFF) | (mask >> 8));
	}

	int8_t CryptographyMaple::rollright(int8_t data, size_t count) const
	{
		int32_t mask = ((data & 0xFF) << 8) >> (count % 8);
		return static_cast<int8_t>((mask & 0xFF) | (mask >> 8));
	}
}