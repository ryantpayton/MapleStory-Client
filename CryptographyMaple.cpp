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
#include "CryptographyMaple.h"
#include "CryptographyConstants83.h"

namespace Net
{
	CryptographyMaple::CryptographyMaple() 
	{
		cipher.SetParameters(256);
		cipher.StartEncryption(maplekey);
	}

	CryptographyMaple::~CryptographyMaple() {}

	void CryptographyMaple::encrypt(int8_t* bytes, size_t packetlen, uint8_t* sendiv)
	{
		size_t a = ((sendiv[3] << 8) | sendiv[2]) ^ MAPLEVERSION;
		size_t b = a ^ packetlen;
		bytes[0] = static_cast<int8_t>(a % 0x100);
		bytes[1] = static_cast<int8_t>(a / 0x100);
		bytes[2] = static_cast<int8_t>(b % 0x100);
		bytes[3] = static_cast<int8_t>(b / 0x100);
		mapleencrypt(bytes + 4, packetlen);
		aesofb(bytes + 4, packetlen, sendiv);
	}

	void CryptographyMaple::decrypt(int8_t* bytes, size_t length, uint8_t* recviv)
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

	void CryptographyMaple::aesofb(int8_t* bytes, size_t length, uint8_t* iv)
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
					cipher.EncryptBlock(miv, miv);
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