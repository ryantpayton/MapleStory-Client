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
#include "PacketHandler83.h"
#include "RecvOpcodes83.h"
#include "CommonHandlers.h"
#include "LoginHandlers83.h"
#include "SetfieldHandler83.h"
#include "KeyboardHandlers83.h"
#include "PlayerHandlers83.h"
#include "MapobjectHandlers83.h"
#include <iostream>

namespace Net
{
	PacketHandler83::PacketHandler83()
	{
		// Initialise array with 0.
		for (uint16_t i = 0; i < NUM_HANDLERS; i++)
		{
			handlers[i] = 0;
		}
		// Register all handlers.
		handlers[LOGIN_RESULT] = new LoginResultHandler83();
		handlers[SERVERLIST] = new ServerlistHandler83();
		handlers[CHARLIST] = new CharlistHandler83();
		handlers[CHARNAME_RESPONSE] = new CharnameResultHandler83();
		handlers[ADD_NEWCHAR_ENTRY] = new AddNewcharHandler83();
		handlers[DELCHAR_RESPONSE] = new DeleteCharResultHandler83();
		handlers[SERVER_IP] = new ServerIPHandler83();
		handlers[PING] = new PingHandler();
		handlers[SELECT_WORLD] = new NullHandler(); //commonly unused
		handlers[RECOMMENDED_WORLDS] = new NullHandler(); //commonly unused
		handlers[MODIFY_INVENTORY] = new NullHandler(); //TO DO
		handlers[STATS_CHANGED] = new StatschangedHandler83();
		//handlers[GIVE_BUFF] = unique_ptr<vhandler>(new give_buff_h());
		handlers[FORCED_STAT_RESET] = new StatresetHandler83(); // recalcstats?
		handlers[UPDATE_SKILLS] = new UpdateskillsHandler83();
		//handlers[SHOW_STATUS_INFO] = unique_ptr<vhandler>(new show_status_info_h());
		handlers[MEMO_RESULT] = new NullHandler(); //TO DO
		handlers[ENABLE_REPORT] = new NullHandler(); //idk what it does
		handlers[UPDATE_GENDER] = new NullHandler(); // useless
		handlers[BUDDY_LIST] = new NullHandler(); //TO DO
		handlers[GUILD_OPERATION] = new NullHandler(); //TO DO
		handlers[SERVER_MESSAGE] = new NullHandler();
		handlers[WEEK_EVENT_MESSAGE] = new NullHandler(); //TO DO
		handlers[FAMILY_PRIV_LIST] = new NullHandler(); //TO DO
		handlers[SCRIPT_PROGRESS_MESSAGE] = new NullHandler(); //TO DO
		handlers[RECEIVE_POLICE] = new NullHandler(); //TO DO
		handlers[SKILL_MACROS] = new SkillmacrosHandler83();
		handlers[SET_FIELD] = new SetfieldHandler83();
		/*handlers[FIELD_EFFECT] = unique_ptr<vhandler>(new field_effect_h());
		handlers[CLOCK] = unique_ptr<vhandler>(new clock_h());
		handlers[SPAWN_PLAYER] = unique_ptr<vhandler>(new spawn_player_h());
		handlers[REMOVE_PLAYER] = unique_ptr<vhandler>(new remove_player_h());
		handlers[CHAT_RECEIVED] = unique_ptr<vhandler>(new chat_received_h());
		handlers[SCROLL_RESULT] = unique_ptr<vhandler>(new scroll_result_h());
		handlers[SPAWN_PET] = unique_ptr<vhandler>(new unhandled()); //TO DO
		handlers[PLAYER_MOVED] = unique_ptr<vhandler>(new player_moved_h());
		handlers[SHOW_ITEM_EFFECT] = unique_ptr<vhandler>(new unhandled()); //TO DO
		handlers[SHOW_CHAIR] = unique_ptr<vhandler>(new unhandled()); //TO DO
		handlers[SHOW_FOREIGN_EFFECT] = unique_ptr<vhandler>(new show_foreign_effect_h()); //TO DO
		handlers[LOCK_UI] = unique_ptr<vhandler>(new unhandled()); //not sure what this does
		handlers[TOGGLE_UI] = unique_ptr<vhandler>(new toggle_ui_h());
		handlers[SPAWN_MOB] = unique_ptr<vhandler>(new spawn_mob_h());
		handlers[KILL_MOB] = unique_ptr<vhandler>(new kill_mob_h());
		handlers[SPAWN_MOB_C] = unique_ptr<vhandler>(new spawn_mob_c_h());
		handlers[MOB_MOVED] = unique_ptr<vhandler>(new mob_moved_h());
		handlers[MOVE_MOB_RESPONSE] = unique_ptr<vhandler>(new move_mob_response_h());
		handlers[SHOW_MOB_HP] = unique_ptr<vhandler>(new show_mob_hp_h());*/
		handlers[SPAWN_NPC] = new SpawnNpcHandler83();
		handlers[SPAWN_NPC_C] = new SpawnNpcControllerHandler83();
		handlers[MAKE_NPC_SCRIPTED] = new NullHandler(); // odin doesn't have it so idk
		//handlers[DROP_ITEM_FROMOBJECT] = unique_ptr<vhandler>(new drop_item_from_mapobject_h());
		//handlers[REMOVE_MAPITEM] = unique_ptr<vhandler>(new remove_mapitem_h());
		//handlers[SPAWN_REACTOR] = unique_ptr<vhandler>(new spawn_reactor_h());
		//handlers[REMOVE_REACTOR] = unique_ptr<vhandler>(new remove_reactor_h());
		//handlers[NPC_DIALOGUE] = unique_ptr<vhandler>(new npc_dialogue_h());
		handlers[KEYMAP] = new KeymapHandler83();
	}

	PacketHandler83::~PacketHandler83() 
	{
		// Destroy registered handlers.
		for (uint16_t i = 0; i < NUM_HANDLERS; i++)
		{
			if (handlers[i])
			{
				delete handlers[i];
			}
		}
	}

	bool PacketHandler83::handle(ClientInterface& client, InPacket& recv) const
	{
		try
		{
			// Read the opcode to determine handler responsible.
			uint16_t opcode = recv.readshort();
			if (opcode < NUM_HANDLERS)
			{
				const PacketHandler* handler = handlers[opcode];
				if (handler)
				{
					// Handler ok. Packet is passed on.
					handler->handle(client, recv);
				}
				else
				{
					// Notice about unhandled packet.
					std::cout << "Received Unhandled Packet. Opcode: " << opcode << std::endl;
				}
			}
		}
		catch (PacketError& pe)
		{
			// This occurs when the packet length or data sent by the server is incorrect.
			std::cout << "Packet Error. Message: " << pe.what() << std::endl;
			return false;
		}
		return true;
	}
}