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
#include "PacketSwitch.h"
#include "RecvOpcodes.h"
#include "Handlers\CommonHandlers.h"

#include "Journey.h"
#ifdef JOURNEY_CUSTOM_VERSION
#include "Handlers\LoginHandlers.h"
#include "Handlers\SetfieldHandlers.h"
#include "Handlers\KeyboardHandlers83.h"
#include "Handlers\PlayerHandlers83.h"
#include "Handlers\MapobjectHandlers.h"
#else
#include "Handlers83\LoginHandlers83.h"
#include "Handlers83\SetfieldHandler83.h"
#include "Handlers\KeyboardHandlers83.h"
#include "Handlers\PlayerHandlers83.h"
#include "Handlers83\MapobjectHandlers83.h"
#endif
#include <iostream>

namespace Net
{
	PacketSwitch::PacketSwitch()
	{

#ifdef JOURNEY_CUSTOM_VERSION
		// Login handlers
		handlers[LOGIN_STATUS] = unique_ptr<PacketHandler>(new LoginStatusHandler());
		handlers[WORLD_STATUS] = unique_ptr<PacketHandler>(new WorldStatusHandler());
		handlers[CHARLIST] = unique_ptr<PacketHandler>(new CharlistHandler());
		handlers[NAME_RESPONSE] = unique_ptr<PacketHandler>(new NameResultHandler());
		handlers[NEW_CHAR_RESPONSE] = unique_ptr<PacketHandler>(new NewCharResponseHandler());
		handlers[DELETE_RESPONSE] = unique_ptr<PacketHandler>(new DeleteResponseHandler());
		handlers[SERVER_IP] = unique_ptr<PacketHandler>(new ServerIPHandler());

		// 'Setfield' handlers
		handlers[CHARACTER_INFO] = unique_ptr<PacketHandler>(new CharacterInfoHandler());
		handlers[WARP_TO_MAP] = unique_ptr<PacketHandler>(new WarpToMapHandler());
		handlers[CHANGE_CHANNEL] = unique_ptr<PacketHandler>(new ChangeChannelHandler());
#else
		// Login handlers
		handlers[LOGIN_RESULT] = unique_ptr<PacketHandler>(new LoginResultHandler83());
		handlers[SERVERLIST] = unique_ptr<PacketHandler>(new ServerlistHandler83());
		handlers[CHARLIST] = unique_ptr<PacketHandler>(new CharlistHandler83());
		handlers[CHARNAME_RESPONSE] = unique_ptr<PacketHandler>(new CharnameResponseHandler83());
		handlers[ADD_NEWCHAR_ENTRY] = unique_ptr<PacketHandler>(new AddNewCharEntryHandler83());
		handlers[DELCHAR_RESPONSE] = unique_ptr<PacketHandler>(new DeleteCharResponseHandler83());
		handlers[SERVER_IP] = unique_ptr<PacketHandler>(new ServerIPHandler83());

		// 'Setfield' handlers
		handlers[SET_FIELD] = unique_ptr<PacketHandler>(new SetfieldHandler83());
#endif

		handlers[PING] = unique_ptr<PacketHandler>(new PingHandler());
		handlers[SELECT_WORLD] = unique_ptr<PacketHandler>(new NullHandler()); //commonly unused
		handlers[RECOMMENDED_WORLDS] = unique_ptr<PacketHandler>(new NullHandler()); //commonly unused
		handlers[MODIFY_INVENTORY] = unique_ptr<PacketHandler>(new NullHandler()); //TO DO
		handlers[STATS_CHANGED] = unique_ptr<PacketHandler>(new StatschangedHandler83());
		//handlers[GIVE_BUFF] = unique_ptr<vhandler>(new give_buff_h());
		handlers[FORCED_STAT_RESET] = unique_ptr<PacketHandler>(new StatresetHandler83()); // recalcstats?
		handlers[UPDATE_SKILLS] = unique_ptr<PacketHandler>(new UpdateskillsHandler83());
		//handlers[SHOW_STATUS_INFO] = unique_ptr<vhandler>(new show_status_info_h());
		handlers[MEMO_RESULT] = unique_ptr<PacketHandler>(new NullHandler()); //TO DO
		handlers[ENABLE_REPORT] = unique_ptr<PacketHandler>(new NullHandler()); //idk what it does
		handlers[UPDATE_GENDER] = unique_ptr<PacketHandler>(new NullHandler()); // useless
		handlers[BUDDY_LIST] = unique_ptr<PacketHandler>(new NullHandler()); //TO DO
		handlers[GUILD_OPERATION] = unique_ptr<PacketHandler>(new NullHandler()); //TO DO
		handlers[SERVER_MESSAGE] = unique_ptr<PacketHandler>(new NullHandler());
		handlers[WEEK_EVENT_MESSAGE] = unique_ptr<PacketHandler>(new NullHandler()); //TO DO
		handlers[FAMILY_PRIV_LIST] = unique_ptr<PacketHandler>(new NullHandler()); //TO DO
		handlers[SCRIPT_PROGRESS_MESSAGE] = unique_ptr<PacketHandler>(new NullHandler()); //TO DO
		handlers[RECEIVE_POLICE] = unique_ptr<PacketHandler>(new NullHandler()); //TO DO
		handlers[SKILL_MACROS] = unique_ptr<PacketHandler>(new SkillmacrosHandler83());
		//handlers[FIELD_EFFECT] = unique_ptr<vhandler>(new field_effect_h());
		//handlers[CLOCK] = unique_ptr<vhandler>(new clock_h());
		handlers[SPAWN_PLAYER] = unique_ptr<PacketHandler>(new SpawnCharHandler83());
		handlers[REMOVE_PLAYER] = unique_ptr<PacketHandler>(new RemoveCharHandler83());
		//handlers[CHAT_RECEIVED] = unique_ptr<vhandler>(new chat_received_h());
		//handlers[SCROLL_RESULT] = unique_ptr<vhandler>(new scroll_result_h());
		handlers[SPAWN_PET] = unique_ptr<PacketHandler>(new SpawnPetHandler83());
		handlers[PLAYER_MOVED] = unique_ptr<PacketHandler>(new MoveCharHandler83());
		/*handlers[SHOW_ITEM_EFFECT] = unique_ptr<vhandler>(new unhandled()); //TO DO
		handlers[SHOW_CHAIR] = unique_ptr<vhandler>(new unhandled()); //TO DO
		handlers[SHOW_FOREIGN_EFFECT] = unique_ptr<vhandler>(new show_foreign_effect_h()); //TO DO
		handlers[LOCK_UI] = unique_ptr<vhandler>(new unhandled()); //not sure what this does
		handlers[TOGGLE_UI] = unique_ptr<vhandler>(new toggle_ui_h());*/
		handlers[SPAWN_MOB] = unique_ptr<PacketHandler>(new SpawnMobHandler83());
		handlers[KILL_MOB] = unique_ptr<PacketHandler>(new KillMobHandler83());
		//handlers[MOB_MOVED] = unique_ptr<vhandler>(new mob_moved_h());
		//handlers[MOVE_MOB_RESPONSE] = unique_ptr<vhandler>(new move_mob_response_h());
		handlers[SHOW_MOB_HP] = unique_ptr<PacketHandler>(new ShowMobHpHandler83());
		handlers[SPAWN_NPC] = unique_ptr<PacketHandler>(new SpawnNpcHandler83());
		handlers[SPAWN_NPC_C] = unique_ptr<PacketHandler>(new SpawnNpcControllerHandler83());
		handlers[MAKE_NPC_SCRIPTED] = unique_ptr<PacketHandler>(new NullHandler());
		handlers[DROP_ITEM_FROMOBJECT] = unique_ptr<PacketHandler>(new DropItemHandler());
		handlers[REMOVE_MAPITEM] = unique_ptr<PacketHandler>(new RemoveDropHandler());
		//handlers[SPAWN_REACTOR] = unique_ptr<vhandler>(new spawn_reactor_h());
		//handlers[REMOVE_REACTOR] = unique_ptr<vhandler>(new remove_reactor_h());
		//handlers[NPC_DIALOGUE] = unique_ptr<vhandler>(new npc_dialogue_h());
		handlers[KEYMAP] = unique_ptr<PacketHandler>(new KeymapHandler83());
	}

	PacketSwitch::~PacketSwitch() {}

	void PacketSwitch::forward(int8_t* buffer, size_t length) const
	{
		// Read the opcode to determine handler responsible.
		InPacket recv = InPacket(buffer, length);
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
}