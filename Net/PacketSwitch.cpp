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
#include "Handlers\LoginHandlers.h"
#include "Handlers\SetfieldHandlers.h"
#include "Handlers\PlayerHandlers.h"
#include "Handlers\AttackHandlers.h"
#include "Handlers\MapobjectHandlers.h"
#include "Handlers\InventoryHandlers.h"
#include "Handlers\MessagingHandlers.h"
#include "Handlers\NpcInteractionHandlers.h"

#include "..\Console.h"

namespace jrc
{
	PacketSwitch::PacketSwitch()
	{
		// Common handlers
		handlers[PING] = std::make_unique<PingHandler>();

		// Login handlers
		handlers[LOGIN_RESULT] = std::make_unique<LoginResultHandler>();
		handlers[SERVERLIST] = std::make_unique<ServerlistHandler>();
		handlers[CHARLIST] = std::make_unique<CharlistHandler>();
		handlers[CHARNAME_RESPONSE] = std::make_unique<CharnameResponseHandler>();
		handlers[ADD_NEWCHAR_ENTRY] = std::make_unique<AddNewCharEntryHandler>();
		handlers[DELCHAR_RESPONSE] = std::make_unique<DeleteCharResponseHandler>();
		handlers[SERVER_IP] = std::make_unique<ServerIPHandler>();

		// 'Setfield' handlers
		handlers[SET_FIELD] = std::make_unique<SetfieldHandler>();

		// MapObject handlers
		handlers[SPAWN_NPC] = std::make_unique<SpawnNpcHandler>();
		handlers[SPAWN_NPC_C] = std::make_unique<SpawnNpcControllerHandler>();
		handlers[SPAWN_MOB] = std::make_unique<SpawnMobHandler>();
		handlers[SPAWN_MOB_C] = std::make_unique<SpawnMobControllerHandler>();
		handlers[MOB_MOVED] = std::make_unique<MobMovedHandler>();
		handlers[SHOW_MOB_HP] = std::make_unique<ShowMobHpHandler>();
		handlers[KILL_MOB] = std::make_unique<KillMobHandler>();
		handlers[SPAWN_CHAR] = std::make_unique<SpawnCharHandler>();
		handlers[CHAR_MOVED] = std::make_unique<CharMovedHandler>();
		handlers[UPDATE_CHARLOOK] = std::make_unique<UpdateCharLookHandler>();
		handlers[SHOW_FOREIGN_EFFECT] = std::make_unique<ShowForeignEffectHandler>();
		handlers[REMOVE_CHAR] = std::make_unique<RemoveCharHandler>();
		handlers[SPAWN_PET] = std::make_unique<SpawnPetHandler>();
		handlers[DROP_LOOT] = std::make_unique<DropLootHandler>();
		handlers[REMOVE_LOOT] = std::make_unique<RemoveLootHandler>();

		// Attack handlers
		handlers[ATTACKED_CLOSE] = std::make_unique<CloseAttackHandler>();
		handlers[ATTACKED_RANGED] = std::make_unique<RangedAttackHandler>();
		handlers[ATTACKED_MAGIC] = std::make_unique<MagicAttackHandler>();

		// Player handlers
		handlers[KEYMAP] = std::make_unique<KeymapHandler>();
		handlers[SKILL_MACROS] = std::make_unique<SkillMacrosHandler>();
		handlers[CHANGE_STATS] = std::make_unique<ChangeStatsHandler>();
		handlers[GIVE_BUFF] = std::make_unique<ApplyBuffHandler>();
		handlers[RECALCULATE_STATS] = std::make_unique<RecalculateStatsHandler>();
		handlers[UPDATE_SKILLS] = std::make_unique<UpdateskillsHandler>();
		handlers[ADD_COOLDOWN] = std::make_unique<AddCooldownHandler>();

		// Messaging handlers
		handlers[SHOW_STATUS_INFO] = std::make_unique<ShowStatusInfoHandler>();
		handlers[CHAT_RECEIVED] = std::make_unique<ChatReceivedHandler>();
		handlers[SCROLL_RESULT] = std::make_unique<ScrollResultHandler>();
		handlers[SERVER_MESSAGE] = std::make_unique<ServerMessageHandler>();
		handlers[WEEK_EVENT_MESSAGE] = std::make_unique<WeekEventMessageHandler>();
		handlers[SHOW_ITEM_GAIN_INCHAT] = std::make_unique<ShowItemGainInChatHandler>();

		// Inventory Handlers
		handlers[MODIFY_INVENTORY] = std::make_unique<ModifyInventoryHandler>();
		handlers[GATHER_RESULT] = std::make_unique<GatherResultHandler>();
		handlers[SORT_RESULT] = std::make_unique<SortResultHandler>();

		// Npc Interaction Handlers
		handlers[NPC_DIALOGUE] = std::make_unique<NpcDialogueHandler>();
		handlers[OPEN_NPC_SHOP] = std::make_unique<OpenNpcShopHandler>();

		// Todo
		handlers[MOVE_MOB_RESPONSE] = std::make_unique<NullHandler>();
		handlers[MEMO_RESULT] = std::make_unique<NullHandler>();
		handlers[ENABLE_REPORT] = std::make_unique<NullHandler>();
		handlers[BUDDY_LIST] = std::make_unique<NullHandler>();
		handlers[GUILD_OPERATION] = std::make_unique<NullHandler>();
		handlers[FAMILY_PRIV_LIST] = std::make_unique<NullHandler>();
		handlers[SCRIPT_PROGRESS_MESSAGE] = std::make_unique<NullHandler>();
		handlers[RECEIVE_POLICE] = std::make_unique<NullHandler>();
		handlers[MAKE_NPC_SCRIPTED] = std::make_unique<NullHandler>();

		// Ignored
		handlers[SELECT_WORLD] = std::make_unique<NullHandler>();
		handlers[RECOMMENDED_WORLDS] = std::make_unique<NullHandler>();
		handlers[UPDATE_GENDER] = std::make_unique<NullHandler>();
	}

	void PacketSwitch::forward(int8_t* buffer, size_t length) const
	{
		// Read the opcode to determine handler responsible.
		InPacket recv = InPacket(buffer, length);
		uint16_t opcode = recv.read_short();
		if (opcode < NUM_HANDLERS)
		{
			const PacketHandler* handler = handlers[opcode].get();
			if (handler)
			{
				// Handler ok. Packet is passed on.
				try
				{
					handler->handle(recv);
				}
				catch (const PacketError& err)
				{
					// Notice about an error.
					Console::get().print("Packet Error: Opcode " + std::to_string(opcode) + ", " + std::string(err.what()));
				}
			}
			else
			{
				// Notice about unhandled packet.
				Console::get().print("Received unhandled packet. Opcode: " + std::to_string(opcode));
			}
		}
	}
}