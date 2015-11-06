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
#include <stdint.h>

namespace Net
{
	// Opcodes for OutPackets associated with version 83 of the game.
	enum SendOpcode83 : int16_t
	{
		LOGIN = 1,
		SERVERLIST_REREQUEST = 4,
		CHARLIST_REQUEST = 5,
		STATUS_REQUEST = 6,
		ACCEPT_TOS = 7,
		SET_GENDER = 8,
		AFTER_LOGIN = 9,
		REGISTER_PIN = 10,
		SERVERLIST_REQUEST = 11,
		SELECT_CHAR = 19,
		PLAYER_LOGIN = 20,
		NAME_CHAR = 21,
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
		PICKUP_ITEM = 202,
		HASH_CHECK = 30000
	};
}