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
#include "Packethandler.h"
#include "LoginHandlers.h"
#include "PongPacket.h"
#include "UILogin.h"

namespace Net
{
	class PingHandler : public ChildHandler
	{
		void PingHandler::handle(ParentHandler* parent, InPacket* recv)
		{
			PongPacket p = PongPacket();
			parent->getsession()->dispatch(&p);
		}
	};

	class NullHandler : public ChildHandler
	{
		void NullHandler::handle(ParentHandler* parent, InPacket* recv){}
	};

	Packethandler::Packethandler()
	{
		handlers[LOGIN_RESULT] = new LoginResultHandler();
		handlers[SERVERLIST] = new ServerlistHandler();
		handlers[CHARLIST] = new CharlistHandler();
		handlers[CHARNAME_RESPONSE] = new CharnameResultHandler();
		handlers[ADD_NEWCHAR_ENTRY] = new AddNewcharHandler();
		handlers[DELCHAR_RESPONSE] = new DeleteCharResultHandler();
		handlers[SERVER_IP] = new ServerIPHandler();
		handlers[PING] = new PingHandler();
		handlers[SELECT_WORLD] = new NullHandler(); //commonly unused
		handlers[RECOMMENDED_WORLDS] = new NullHandler(); //commonly unused
		/*handlers[MODIFY_INVENTORY] = unique_ptr<vhandler>(new modify_inventory_h());
		handlers[STATS_CHANGED] = unique_ptr<vhandler>(new stats_changed_h());
		handlers[GIVE_BUFF] = unique_ptr<vhandler>(new give_buff_h());
		handlers[FORCED_STAT_RESET] = unique_ptr<vhandler>(new unhandled()); // recalcstats?
		handlers[UPDATE_SKILLS] = unique_ptr<vhandler>(new update_skills_h());
		handlers[SHOW_STATUS_INFO] = unique_ptr<vhandler>(new show_status_info_h());
		handlers[MEMO_RESULT] = unique_ptr<vhandler>(new memo_result_h());
		handlers[ENABLE_REPORT] = unique_ptr<vhandler>(new enable_report_h());
		handlers[UPDATE_GENDER] = unique_ptr<vhandler>(new unhandled()); //completely useless
		handlers[BUDDY_LIST] = unique_ptr<vhandler>(new buddylist_h());
		handlers[GUILD_OPERATION] = unique_ptr<vhandler>(new unhandled()); //TO DO
		handlers[SERVER_MESSAGE] = unique_ptr<vhandler>(new server_message_h());
		handlers[WEEK_EVENT_MESSAGE] = unique_ptr<vhandler>(new week_event_messsage_h());
		handlers[FIELD_SET_VARIABLE] = unique_ptr<vhandler>(new unhandled()); //odin doesn't have it
		handlers[FAMILY_PRIV_LIST] = unique_ptr<vhandler>(new family_plist_h());
		handlers[CANCEL_RENAME_BY_OTHER] = unique_ptr<vhandler>(new unhandled()); //odin doesn't have it
		handlers[SCRIPT_PROGRESS_MESSAGE] = unique_ptr<vhandler>(new unhandled()); //TO DO
		handlers[RECEIVE_POLICE] = unique_ptr<vhandler>(new receive_police_h());
		handlers[SKILL_MACROS] = unique_ptr<vhandler>(new skill_macros_h());
		handlers[SET_FIELD] = unique_ptr<vhandler>(new set_field_h());
		handlers[FIELD_EFFECT] = unique_ptr<vhandler>(new field_effect_h());
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
		handlers[SHOW_MOB_HP] = unique_ptr<vhandler>(new show_mob_hp_h());
		handlers[SPAWN_NPC] = unique_ptr<vhandler>(new spawn_npc_h());
		handlers[SPAWN_NPC_C] = unique_ptr<vhandler>(new spawn_npc_c_h());
		handlers[MAKE_NPC_SCRIPTED] = unique_ptr<vhandler>(new unhandled()); // odin doesn't have it so idk
		handlers[DROP_ITEM_FROMOBJECT] = unique_ptr<vhandler>(new drop_item_from_mapobject_h());
		handlers[REMOVE_MAPITEM] = unique_ptr<vhandler>(new remove_mapitem_h());
		handlers[SPAWN_REACTOR] = unique_ptr<vhandler>(new spawn_reactor_h());
		handlers[REMOVE_REACTOR] = unique_ptr<vhandler>(new remove_reactor_h());
		handlers[NPC_DIALOGUE] = unique_ptr<vhandler>(new npc_dialogue_h());
		handlers[KEYMAP] = unique_ptr<vhandler>(new keymap_h());*/
	}

	Packethandler::~Packethandler()
	{
		for (size_t i = 0; i < NUM_HANDLERS; i++)
		{
			if (handlers[i])
			{
				delete handlers[i];
			}
		}
	}

	void Packethandler::init(Datacache* ch, UI* u, Login* lg, Session* ses)
	{
		cache = ch;
		ui = u;
		login = lg;
		session = ses;
	}

	void Packethandler::handle(InPacket* recv)
	{
		if (recv->length() > 1)
		{
			bool handled = false;
			int16_t opcode = recv->readshort();
			if (opcode < NUM_HANDLERS)
			{
				ChildHandler* hnd = handlers[opcode];
				if (hnd)
				{
					hnd->handle(this, recv);
					handled = true;
				}
			}

			if (!handled)
			{
				throw new runtime_error("Unhandled packet.");
			}
		}
	}
}