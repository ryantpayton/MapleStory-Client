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
#include "PacketHandler83.h"
#include "RecvOpcodes83.h"
#include "Handlers\CommonHandlers.h"
#include "Handlers\LoginHandlers83.h"
#include "Handlers\SetfieldHandler83.h"
#include "Handlers\KeyboardHandlers83.h"
#include "Handlers\PlayerHandlers83.h"
#include "Handlers\MapobjectHandlers83.h"
#include <iostream>

#define handler_ptr std::unique_ptr<PacketHandler>

namespace Net
{
	PacketHandler83::PacketHandler83()
	{
		// Register all handlers.
		handlers[LOGIN_RESULT] = handler_ptr(new LoginResultHandler83());
		handlers[SERVERLIST] = handler_ptr(new ServerlistHandler83());
		handlers[CHARLIST] = handler_ptr(new CharlistHandler83());
		handlers[CHARNAME_RESPONSE] = handler_ptr(new CharnameResultHandler83());
		handlers[ADD_NEWCHAR_ENTRY] = handler_ptr(new AddNewcharHandler83());
		handlers[DELCHAR_RESPONSE] = handler_ptr(new DeleteCharResultHandler83());
		handlers[SERVER_IP] = handler_ptr(new ServerIPHandler83());
		handlers[PING] = handler_ptr(new PingHandler());
		handlers[SELECT_WORLD] = handler_ptr(new NullHandler()); //commonly unused
		handlers[RECOMMENDED_WORLDS] = handler_ptr(new NullHandler()); //commonly unused
		handlers[MODIFY_INVENTORY] = handler_ptr(new NullHandler()); //TO DO
		handlers[STATS_CHANGED] = handler_ptr(new StatschangedHandler83());
		//handlers[GIVE_BUFF] = unique_ptr<vhandler>(new give_buff_h());
		handlers[FORCED_STAT_RESET] = handler_ptr(new StatresetHandler83()); // recalcstats?
		handlers[UPDATE_SKILLS] = handler_ptr(new UpdateskillsHandler83());
		//handlers[SHOW_STATUS_INFO] = unique_ptr<vhandler>(new show_status_info_h());
		handlers[MEMO_RESULT] = handler_ptr(new NullHandler()); //TO DO
		handlers[ENABLE_REPORT] = handler_ptr(new NullHandler()); //idk what it does
		handlers[UPDATE_GENDER] = handler_ptr(new NullHandler()); // useless
		handlers[BUDDY_LIST] = handler_ptr(new NullHandler()); //TO DO
		handlers[GUILD_OPERATION] = handler_ptr(new NullHandler()); //TO DO
		handlers[SERVER_MESSAGE] = handler_ptr(new NullHandler());
		handlers[WEEK_EVENT_MESSAGE] = handler_ptr(new NullHandler()); //TO DO
		handlers[FAMILY_PRIV_LIST] = handler_ptr(new NullHandler()); //TO DO
		handlers[SCRIPT_PROGRESS_MESSAGE] = handler_ptr(new NullHandler()); //TO DO
		handlers[RECEIVE_POLICE] = handler_ptr(new NullHandler()); //TO DO
		handlers[SKILL_MACROS] = handler_ptr(new SkillmacrosHandler83());
		handlers[SET_FIELD] = handler_ptr(new SetfieldHandler83());
		//handlers[FIELD_EFFECT] = unique_ptr<vhandler>(new field_effect_h());
		//handlers[CLOCK] = unique_ptr<vhandler>(new clock_h());
		handlers[SPAWN_PLAYER] = handler_ptr(new SpawnCharHandler83());
		handlers[REMOVE_PLAYER] = handler_ptr(new RemoveCharHandler83());
		//handlers[CHAT_RECEIVED] = unique_ptr<vhandler>(new chat_received_h());
		//handlers[SCROLL_RESULT] = unique_ptr<vhandler>(new scroll_result_h());
		//handlers[SPAWN_PET] = unique_ptr<vhandler>(new unhandled()); //TO DO
		handlers[PLAYER_MOVED] = handler_ptr(new MoveCharHandler83());
		/*handlers[SHOW_ITEM_EFFECT] = unique_ptr<vhandler>(new unhandled()); //TO DO
		handlers[SHOW_CHAIR] = unique_ptr<vhandler>(new unhandled()); //TO DO
		handlers[SHOW_FOREIGN_EFFECT] = unique_ptr<vhandler>(new show_foreign_effect_h()); //TO DO
		handlers[LOCK_UI] = unique_ptr<vhandler>(new unhandled()); //not sure what this does
		handlers[TOGGLE_UI] = unique_ptr<vhandler>(new toggle_ui_h());*/
		handlers[SPAWN_MOB] = handler_ptr(new SpawnMobHandler83());
		handlers[KILL_MOB] = handler_ptr(new KillMobHandler83());
		handlers[SPAWN_MOB_C] = handler_ptr(new SpawnMobControllerHandler83());
		//handlers[MOB_MOVED] = unique_ptr<vhandler>(new mob_moved_h());
		//handlers[MOVE_MOB_RESPONSE] = unique_ptr<vhandler>(new move_mob_response_h());
		handlers[SHOW_MOB_HP] = handler_ptr(new ShowMobHpHandler83());
		handlers[SPAWN_NPC] = handler_ptr(new SpawnNpcHandler83());
		handlers[SPAWN_NPC_C] = handler_ptr(new SpawnNpcControllerHandler83());
		handlers[MAKE_NPC_SCRIPTED] = handler_ptr(new NullHandler());
		//handlers[DROP_ITEM_FROMOBJECT] = unique_ptr<vhandler>(new drop_item_from_mapobject_h());
		//handlers[REMOVE_MAPITEM] = unique_ptr<vhandler>(new remove_mapitem_h());
		//handlers[SPAWN_REACTOR] = unique_ptr<vhandler>(new spawn_reactor_h());
		//handlers[REMOVE_REACTOR] = unique_ptr<vhandler>(new remove_reactor_h());
		//handlers[NPC_DIALOGUE] = unique_ptr<vhandler>(new npc_dialogue_h());
		handlers[KEYMAP] = handler_ptr(new KeymapHandler83());
	}

	PacketHandler83::~PacketHandler83() {}

	void PacketHandler83::handle(InPacket& recv) const
	{
		try
		{
			// Read the opcode to determine handler responsible.
			uint16_t opcode = recv.readshort();
			if (opcode < NUM_HANDLERS)
			{
				const PacketHandler* handler = handlers[opcode].get();
				if (handler)
				{
					// Handler ok. Packet is passed on.
					handler->handle(recv);
				}
				else
				{
					// Notice about unhandled packet.
					std::cout << "Received Unhandled Packet. Opcode: " << opcode << std::endl;
				}
			}
		}
		catch (const PacketError& pe)
		{
			// This occurs when the data sent by the server is incorrect.
			std::cout << "Packet Error. Message: " << pe.what() << std::endl;
		}
	}
}