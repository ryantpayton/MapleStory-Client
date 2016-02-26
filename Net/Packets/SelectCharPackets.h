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

namespace Net
{
	const string MACS = "68-5D-43-F8-B8-6C, 7A-79-19-8B-31-3F";
	const string HWID = "685D43F8_B86C7A79";

	// Tell the server which character was picked.
	// Opcode: SELECT_CHAR(19)
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

	// Registers a pic and tells the server which character was picked.
	// Opcode: REGISTER_PIC(29)
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

	// Requests using the specified character with the specified pic.
	// Opcode: SELECT_CHAR_PIC(30)
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