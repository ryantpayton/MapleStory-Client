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
#include "Net\OutPacket.h"
#include "Net\SendOpcodes.h"

#include "Journey.h"
#ifdef JOURNEY_CUSTOM_VERSION

namespace Net
{
	const string MACS = "68-5D-43-F8-B8-6C, 7A-79-19-8B-31-3F";
	const string HWID = "685D43F8_B86C7A79";

	// Packet which tells the server which character was picked.
	// Also sends (fake) Mac-adress and Hardware Id.
	class SelectCharPacket : public OutPacket
	{
	public:
		SelectCharPacket(int32_t cid) : OutPacket(SELECT_CHAR)
		{
			writeint(cid);
			writestr(MACS);
			writestr(HWID);
		}
	};

	// Packet which registers a pic and tells the server which character was picked.
	// Also sends (fake) Mac-adress and Hardware Id.
	class RegisterPicPacket : public OutPacket
	{
	public:
		RegisterPicPacket(int32_t cid, string pic) : OutPacket(REGISTER_PIC)
		{
			writeint(cid);
			writestr(MACS);
			writestr(HWID);
			writestr(pic);
		}
	};

	// Packet which requests using the specified character with the specified pic.
	// Also sends (fake) mac-adress and hardware id.
	class SelectCharPicPacket : public OutPacket
	{
	public:
		SelectCharPicPacket(string pic, int32_t cid) : OutPacket(SELECT_CHAR_PIC)
		{
			writestr(pic);
			writeint(cid);
			writestr(MACS);
			writestr(HWID);
		}
	};
}

#else

namespace Net
{
	const string MACS = "68-5D-43-F8-B8-6C, 7A-79-19-8B-31-3F";
	const string HWID = "685D43F8_B86C7A79";

	// Packet which tells the server which character was picked.
	// Also sends (fake) Mac-adress and Hardware Id.
	class SelectCharPacket : public OutPacket
	{
	public:
		SelectCharPacket(int32_t cid) : OutPacket(SELECT_CHAR)
		{
			writeint(cid);
			writestr(MACS);
			writestr(HWID);
		}
	};

	// Packet which registers a pic and tells the server which character was picked.
	// Also sends (fake) Mac-adress and Hardware Id.
	class RegisterPicPacket : public OutPacket
	{
	public:
		RegisterPicPacket(int32_t cid, string pic) : OutPacket(REGISTER_PIC)
		{
			skip(1);
			writeint(cid);
			writestr(MACS);
			writestr(HWID);
			writestr(pic);
		}
	};

	// Packet which requests using the specified character with the specified pic.
	// Also sends (fake) mac-adress and hardware id.
	class SelectCharPicPacket : public OutPacket
	{
	public:
		SelectCharPicPacket(string pic, int32_t cid) : OutPacket(SELECT_CHAR_PIC)
		{
			writestr(pic);
			writeint(cid);
			writestr(MACS);
			writestr(HWID);
		}
	};
}

#endif