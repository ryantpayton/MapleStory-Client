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
//																			//
// This file uses cryptography algorithms from the OdinMS Maplestory server //
// AES implemenation by Chris Lomont										//
//////////////////////////////////////////////////////////////////////////////
#include "CryptographyMaple.h"


namespace Net
{
	const uint8_t maplebytes[256] = 
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

	const uint8_t maplekey[32] =
	{
		0x13, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0xB4, 0x00, 0x00, 0x00,
		0x1B, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x33, 0x00, 0x00, 0x00, 0x52, 0x00, 0x00, 0x00
	};

	void CryptographyMaple::init(vector<uint8_t> recv, vector<uint8_t> send, uint8_t ver, uint8_t loc)
	{
		recviv = recv;
		sendiv = send;
		version = ver;
		localisation = loc;

		cipher.SetParameters(256);
	}

	void CryptographyMaple::encrypt(OutPacket* tosend)
	{
		int length = tosend->length();

		int a = (sendiv[3] << 8) | sendiv[2];
		a ^= (version);
		int b = a ^ length;
		char header[4];
		header[0] = (char)(a % 0x100);
		header[1] = (char)(a / 0x100);
		header[2] = (char)(b % 0x100);
		header[3] = (char)(b / 0x100);

		char* bytes = tosend->getbytes();
		mapleencrypt(bytes, length);
		aescrypt(bytes, length, false);
		updateiv(false);

		tosend->writeheader(header);
	}

	int CryptographyMaple::getlength(char* bytes)
	{
		int first = static_cast<uint8_t>(bytes[0]) | static_cast<uint8_t>(bytes[1]) << 8 | static_cast<uint8_t>(bytes[2]) << 16 | static_cast<uint8_t>(bytes[3]) << 24;
		unsigned short length = (first >> 16) ^ (first & 0xFFFF);
		return static_cast<int>(length);
	}

	void CryptographyMaple::decrypt(char* bytes, int length, bool end)
	{
		lock_guard<mutex> lock(decryptlock);
		aescrypt(bytes, length, true);
		mapledecrypt(bytes, length);
		if (end)
		{
			updateiv(true);
		}
	}

	void CryptographyMaple::mapleencrypt(char* bytes, int length)
	{
		for (int j = 0; j < 6; j++) {
			char remember = 0;
			char dataLength = (char)(length & 0xFF);
			if (j % 2 == 0) {
				for (int i = 0; i < length; i++) {
					char cur = bytes[i];
					cur = rollleft(cur, 3);
					cur += dataLength;
					cur ^= remember;
					remember = cur;
					cur = rollright(cur, (int)dataLength & 0xFF);
					cur = ((char)((~cur) & 0xFF));
					cur += 0x48;
					dataLength--;
					bytes[i] = cur;
				}
			}
			else {
				for (int i = length - 1; i >= 0; i--) {
					char cur = bytes[i];
					cur = rollleft(cur, 4);
					cur += dataLength;
					cur ^= remember;
					remember = cur;
					cur ^= 0x13;
					cur = rollright(cur, 3);
					dataLength--;
					bytes[i] = cur;
				}
			}
		}
	}

	void CryptographyMaple::mapledecrypt(char* bytes, int length)
	{
		for (int j = 1; j <= 6; j++) 
		{
			uint8_t remember = 0;
			uint8_t dataLength = static_cast<uint8_t>(length & 0xFF);
			uint8_t nextRemember;
			if (j % 2 == 0) 
			{
				for (int i = 0; i < length; i++) 
				{
					uint8_t cur = bytes[i];
					cur -= 0x48;
					cur = static_cast<uint8_t>((~cur) & 0xFF);
					cur = rollleft(cur, (int)dataLength & 0xFF);
					nextRemember = cur;
					cur ^= remember;
					remember = nextRemember;
					cur -= dataLength;
					cur = rollright(cur, 3);
					bytes[i] = cur;
					dataLength--;
				}
			}
			else 
			{
				for (int i = length - 1; i >= 0; i--) 
				{
					uint8_t cur = bytes[i];
					cur = rollleft(cur, 3);
					cur ^= 0x13;
					nextRemember = cur;
					cur ^= remember;
					remember = nextRemember;
					cur -= dataLength;
					cur = rollright(cur, 4);
					bytes[i] = cur;
					dataLength--;
				}
			}
		}
	}

	void CryptographyMaple::aescrypt(char* bytes, int length, bool recv)
	{
		int total = length;
		int llength = 0x5B0;
		int start = 0;
		while (length > 0) 
		{
			vector<uint8_t> miv = (recv) ? recviv : sendiv;
			for (int i = 0; i < 12; i++)
			{
				miv.push_back(miv[i % 4]);
			}

			if (length < llength) {
				llength = length;
			}

			for (int x = start; x < (start + llength); x++) {

				if ((x - start) % miv.size() == 0) 
				{
					uint8_t newiv[16];
					cipher.StartEncryption(maplekey);
					cipher.EncryptBlock(miv.data(), newiv);
					miv.clear();
					for (int j = 0; j < 16; j++) 
					{
						miv.push_back(newiv[j]);
					}
				}
				bytes[x] ^= miv[(x - start) % 16];
			}
			start += llength;
			length -= llength;
			llength = 0x5B4;
		}
	}

	char CryptographyMaple::rollleft(char in, int count) 
	{
		int tmp = (int)in & 0xFF;
		tmp = tmp << (count % 8);
		return static_cast<char>(((tmp & 0xFF) | (tmp >> 8)));
	}

	char CryptographyMaple::rollright(char in, int count) 
	{
		int tmp = (int)in & 0xFF;
		tmp = (tmp << 8) >> (count % 8);
		return static_cast<char>(((tmp & 0xFF) | (tmp >> 8)));
	}

	void CryptographyMaple::updateiv(bool recv)
	{
		uint8_t mbytes[4] = { 0xf2, 0x53, 0x50, 0xc6 };

		for (char i = 0; i < 4; i++)
		{
			char input = (recv) ? static_cast<char>(recviv[i]) : static_cast<char>(sendiv[i]);
			char elina = mbytes[1];
			char anna = input;
			char moritz = maplebytes[(int)elina & 0xFF];
			moritz -= input;
			mbytes[0] += moritz;
			moritz = mbytes[2];
			moritz ^= maplebytes[(int)anna & 0xFF];
			elina -= (int)moritz & 0xFF;
			mbytes[1] = elina;
			elina = mbytes[3];
			moritz = elina;
			elina -= (int)mbytes[0] & 0xFF;
			moritz = maplebytes[(int)moritz & 0xFF];
			moritz += input;
			moritz ^= mbytes[2];
			mbytes[2] = moritz;
			elina += (int)maplebytes[(int)anna & 0xFF] & 0xFF;
			mbytes[3] = elina;
			size_t merry = ((size_t)mbytes[0]) & 0xFF;
			merry |= (mbytes[1] << 8) & 0xFF00;
			merry |= (mbytes[2] << 16) & 0xFF0000;
			merry |= (mbytes[3] << 24) & 0xFF000000;
			size_t ret_value = merry;
			ret_value = ret_value >> 0x1d;
			merry = merry << 3;
			ret_value = ret_value | merry;
			mbytes[0] = static_cast<uint8_t>(ret_value & 0xFF);
			mbytes[1] = static_cast<uint8_t>((ret_value >> 8) & 0xFF);
			mbytes[2] = static_cast<uint8_t>((ret_value >> 16) & 0xFF);
			mbytes[3] = static_cast<uint8_t>((ret_value >> 24) & 0xFF);
		}

		if (recv)
		{
			recviv.clear();
			recviv.push_back(mbytes[0]);
			recviv.push_back(mbytes[1]);
			recviv.push_back(mbytes[2]);
			recviv.push_back(mbytes[3]);
		}
		else
		{
			sendiv.clear();
			sendiv.push_back(mbytes[0]);
			sendiv.push_back(mbytes[1]);
			sendiv.push_back(mbytes[2]);
			sendiv.push_back(mbytes[3]);
		}
	}
}