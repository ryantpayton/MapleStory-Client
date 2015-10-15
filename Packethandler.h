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
#include "ParentHandler.h"
#include "ChildHandler.h"

namespace Net
{
	const size_t NUM_HANDLERS = 335;

	enum RecvOpcode : int16_t
	{
		LOGIN_RESULT = 0,
		SERVERLIST = 10,
		CHARLIST = 11,
		SERVER_IP = 12,
		CHARNAME_RESPONSE = 13,
		ADD_NEWCHAR_ENTRY = 14,
		DELCHAR_RESPONSE = 15,
		PING = 17,
		SELECT_WORLD = 26,
		RECOMMENDED_WORLDS = 27,
		MODIFY_INVENTORY = 29,
		STATS_CHANGED = 31,
		GIVE_BUFF = 32,
		FORCED_STAT_RESET = 35,
		UPDATE_SKILLS = 36,
		SHOW_STATUS_INFO = 39,
		MEMO_RESULT = 41,
		ENABLE_REPORT = 47,
		UPDATE_GENDER = 58,
		BUDDY_LIST = 63,
		GUILD_OPERATION = 65,
		SERVER_MESSAGE = 68,
		WEEK_EVENT_MESSAGE = 77,
		FIELD_SET_VARIABLE = 92,
		FAMILY_PRIV_LIST = 100,
		CANCEL_RENAME_BY_OTHER = 120,
		SCRIPT_PROGRESS_MESSAGE = 122,
		RECEIVE_POLICE = 123,
		SKILL_MACROS = 124,
		SET_FIELD = 125,
		FIELD_EFFECT = 138,
		CLOCK = 147,
		SPAWN_PLAYER = 160,
		REMOVE_PLAYER = 161,
		CHAT_RECEIVED = 162,
		SCROLL_RESULT = 167,
		SPAWN_PET = 168,
		PLAYER_MOVED = 185,
		SHOW_ITEM_EFFECT = 194,
		SHOW_CHAIR = 196,
		SHOW_FOREIGN_EFFECT = 198,
		LOCK_UI = 221,
		TOGGLE_UI = 222,
		SPAWN_MOB = 236,
		KILL_MOB = 237,
		SPAWN_MOB_C = 238,
		MOB_MOVED = 239,
		MOVE_MOB_RESPONSE = 240,
		SHOW_MOB_HP = 250,
		SPAWN_NPC = 257,
		SPAWN_NPC_C = 259,
		MAKE_NPC_SCRIPTED = 263,
		DROP_ITEM_FROMOBJECT = 268,
		REMOVE_MAPITEM = 269,
		SPAWN_REACTOR = 279,
		REMOVE_REACTOR = 280,
		NPC_DIALOGUE = 304,
		KEYMAP = 335
	};

	class Packethandler : public ParentHandler
	{
	public:
		Packethandler();
		~Packethandler();
		void init(Datacache*, UI*, Login*, Session*);
		void handle(InPacket*);
		Datacache* getcache() { return cache; }
		UI* getui() { return ui; }
		Login* getlogin() { return login; }
		Session* getsession() { return session; }
	private:
		ChildHandler* handlers[NUM_HANDLERS];
		Datacache* cache;
		UI* ui;
		Login* login;
		Session* session;
	};
}

