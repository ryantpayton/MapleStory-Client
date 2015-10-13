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
#include "Packet.h"

namespace Net
{
	enum SendOpcode : int16_t
	{
		LOGIN = 1,
		SERVER_REREQ = 4,
		CHARL_REQ = 5,
		STATUS_REQ = 6,
		ACCEPT_TOS = 7,
		SET_GENDER = 8,
		AFTER_LOGIN = 9,
		REG_PIN = 10,
		SERVER_REQUEST = 11,
		SELECT_CHAR = 19,
		PLAYER_LOGIN = 20,
		CHECK_CHARNAME = 21,
		CREATE_CHAR = 22,
		DELETE_CHAR = 23,
		PONG = 24,
		REGISTER_PIC = 29,
		SELECT_CHAR_PIC = 30,
		CHANGEMAP = 38,
		MOVE_PLAYER = 41,
		CLOSE_ATTACK = 44,
		GENERAL_CHAT = 49,
		TALK_TO_NPC = 58,
		MOVE_ITEM = 71,
		USE_ITEM = 72,
		SCROLL_EQUIP = 86,
		SPEND_AP = 87,
		MOVE_MONSTER = 188,
		PICKUP_ITEM = 202
	};

	class OutPacket : public Packet
	{
	public:
		OutPacket(int16_t);
		OutPacket() {}
		~OutPacket() {}
		void skip(size_t);
		void writeheader(char*);
		void writestr(string);
		void writech(int8_t c) { bytes.push_back(c); }
		void writesh(int16_t s) { write<int16_t>(s); }
		void writeint(int32_t i) { write<int32_t>(i); }
		void writelg(int64_t l) { write<int64_t>(l); }
		void writeopc(SendOpcode o) { write<int16_t>(o); }
	private:
		template <class T>
		void write(T num)
		{
			size_t size = sizeof(T) / sizeof(int8_t);
			for (size_t i = 0; i < size; i++)
			{
				bytes.push_back(static_cast<int8_t>(num));
				num = num >> 8;
			}
		}
	};
}

