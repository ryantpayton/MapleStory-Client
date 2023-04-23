//////////////////////////////////////////////////////////////////////////////////
//	This file is part of the continued Journey MMORPG client					//
//	Copyright (C) 2015-2019  Daniel Allendorf, Ryan Payton						//
//																				//
//	This program is free software: you can redistribute it and/or modify		//
//	it under the terms of the GNU Affero General Public License as published by	//
//	the Free Software Foundation, either version 3 of the License, or			//
//	(at your option) any later version.											//
//																				//
//	This program is distributed in the hope that it will be useful,				//
//	but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				//
//	GNU Affero General Public License for more details.							//
//																				//
//	You should have received a copy of the GNU Affero General Public License	//
//	along with this program.  If not, see <https://www.gnu.org/licenses/>.		//
//////////////////////////////////////////////////////////////////////////////////
#include "PacketSwitch.h"

#include "Handlers/AttackHandlers.h"
#include "Handlers/CashShopHandlers.h"
#include "Handlers/CommonHandlers.h"
#include "Handlers/InventoryHandlers.h"
#include "Handlers/LoginHandlers.h"
#include "Handlers/MapObjectHandlers.h"
#include "Handlers/MessagingHandlers.h"
#include "Handlers/NpcInteractionHandlers.h"
#include "Handlers/PlayerHandlers.h"
#include "Handlers/PlayerInteractionHandlers.h"
#include "Handlers/SetFieldHandlers.h"
#include "Handlers/TestingHandlers.h"

#include "../Configuration.h"

namespace ms
{
	// Opcodes for InPackets
	enum Opcode : uint16_t
	{
		/// Login 1
		LOGIN_RESULT = 0,
		SERVERSTATUS = 3,
		SELECT_CHARACTER_BY_VAC = 9,
		SERVERLIST = 10,
		CHARLIST = 11,
		SERVER_IP = 12,
		CHARNAME_RESPONSE = 13,
		ADD_NEWCHAR_ENTRY = 14,
		DELCHAR_RESPONSE = 15,
		PING = 17,

		/// Login 2
		CHECK_SPW_RESULT = 28,

		/// Inventory 1
		MODIFY_INVENTORY = 29,

		/// Player 2
		CHANGE_CHANNEL = 16,
		CHANGE_STATS = 31,
		GIVE_BUFF = 32,
		CANCEL_BUFF = 33,
		RECALCULATE_STATS = 35,
		UPDATE_SKILL = 36,

		/// Messaging 1
		SHOW_STATUS_INFO = 39,

		/// Inventory 2
		GATHER_RESULT = 52,
		SORT_RESULT = 53,

		/// Player 3

		/// Messaging 2
		SERVER_MESSAGE = 68,
		WEEK_EVENT_MESSAGE = 77,

		SKILL_MACROS = 124,
		SET_FIELD = 125,
		FIELD_EFFECT = 138,

		/// MapObject
		SPAWN_CHAR = 160,
		REMOVE_CHAR = 161,

		/// Messaging
		CHAT_RECEIVED = 162,
		SCROLL_RESULT = 167,

		/// MapObject
		SPAWN_PET = 168,
		CHAR_MOVED = 185,

		/// Attack
		ATTACKED_CLOSE = 186,
		ATTACKED_RANGED = 187,
		ATTACKED_MAGIC = 188,

		UPDATE_CHARLOOK = 197,
		SHOW_FOREIGN_EFFECT = 198,
		SHOW_ITEM_GAIN_INCHAT = 206, // TODO: Rename this (Terribly named)

		/// Player
		ADD_COOLDOWN = 234,

		/// MapObject
		SPAWN_MOB = 236,
		KILL_MOB = 237,
		SPAWN_MOB_C = 238,
		MOB_MOVED = 239,
		SHOW_MOB_HP = 250,
		SPAWN_NPC = 257,
		SPAWN_NPC_C = 259,
		DROP_LOOT = 268,
		REMOVE_LOOT = 269,
		HIT_REACTOR = 277,
		SPAWN_REACTOR = 279,
		REMOVE_REACTOR = 280,

		/// NPC Interaction
		NPC_DIALOGUE = 304,
		OPEN_NPC_SHOP = 305,
		CONFIRM_SHOP_TRANSACTION = 306,
		KEYMAP = 335,

		/// Player Interaction
		CHAR_INFO = 61,

		/// Cash Shop
		SET_CASH_SHOP = 127
	};

	PacketSwitch::PacketSwitch()
	{
		// Common handlers
		emplace<PING, PingHandler>();

		// Login handlers
		emplace<LOGIN_RESULT, LoginResultHandler>();
		emplace<SERVERSTATUS, ServerStatusHandler>();
		emplace<SELECT_CHARACTER_BY_VAC, SelectCharacterHandler>();
		emplace<SERVERLIST, ServerlistHandler>();
		emplace<CHARLIST, CharlistHandler>();
		emplace<SERVER_IP, ServerIPHandler>();
		emplace<CHARNAME_RESPONSE, CharnameResponseHandler>();
		emplace<ADD_NEWCHAR_ENTRY, AddNewCharEntryHandler>();
		emplace<DELCHAR_RESPONSE, DeleteCharResponseHandler>();

		// SetField handlers
		emplace<SET_FIELD, SetFieldHandler>();

		// MapObject handlers
		emplace<SPAWN_CHAR, SpawnCharHandler>();
		emplace<CHAR_MOVED, CharMovedHandler>();
		emplace<UPDATE_CHARLOOK, UpdateCharLookHandler>();
		emplace<SHOW_FOREIGN_EFFECT, ShowForeignEffectHandler>();
		emplace<REMOVE_CHAR, RemoveCharHandler>();
		emplace<SPAWN_PET, SpawnPetHandler>();
		emplace<SPAWN_NPC, SpawnNpcHandler>();
		emplace<SPAWN_NPC_C, SpawnNpcControllerHandler>();
		emplace<SPAWN_MOB, SpawnMobHandler>();
		emplace<SPAWN_MOB_C, SpawnMobControllerHandler>();
		emplace<MOB_MOVED, MobMovedHandler>();
		emplace<SHOW_MOB_HP, ShowMobHpHandler>();
		emplace<KILL_MOB, KillMobHandler>();
		emplace<DROP_LOOT, DropLootHandler>();
		emplace<REMOVE_LOOT, RemoveLootHandler>();
		emplace<HIT_REACTOR, HitReactorHandler>();
		emplace<SPAWN_REACTOR, SpawnReactorHandler>();
		emplace<REMOVE_REACTOR, RemoveReactorHandler>();

		// Attack handlers
		emplace<ATTACKED_CLOSE, CloseAttackHandler>();
		emplace<ATTACKED_RANGED, RangedAttackHandler>();
		emplace<ATTACKED_MAGIC, MagicAttackHandler>();

		// Player handlers
		emplace<CHANGE_CHANNEL, ChangeChannelHandler>();
		emplace<KEYMAP, KeymapHandler>();
		emplace<SKILL_MACROS, SkillMacrosHandler>();
		emplace<CHANGE_STATS, ChangeStatsHandler>();
		emplace<GIVE_BUFF, ApplyBuffHandler>();
		emplace<CANCEL_BUFF, CancelBuffHandler>();
		emplace<RECALCULATE_STATS, RecalculateStatsHandler>();
		emplace<UPDATE_SKILL, UpdateSkillHandler>();
		emplace<ADD_COOLDOWN, AddCooldownHandler>();

		// Messaging handlers
		emplace<SHOW_STATUS_INFO, ShowStatusInfoHandler>();
		emplace<CHAT_RECEIVED, ChatReceivedHandler>();
		emplace<SCROLL_RESULT, ScrollResultHandler>();
		emplace<SERVER_MESSAGE, ServerMessageHandler>();
		emplace<WEEK_EVENT_MESSAGE, WeekEventMessageHandler>();
		emplace<SHOW_ITEM_GAIN_INCHAT, ShowItemGainInChatHandler>();

		// Inventory Handlers
		emplace<MODIFY_INVENTORY, ModifyInventoryHandler>();
		emplace<GATHER_RESULT, GatherResultHandler>();
		emplace<SORT_RESULT, SortResultHandler>();

		// NPC Interaction Handlers
		emplace<NPC_DIALOGUE, NpcDialogueHandler>();
		emplace<OPEN_NPC_SHOP, OpenNpcShopHandler>();

		// Player Interaction
		emplace<CHAR_INFO, CharInfoHandler>();

		// Cash Shop
		emplace<SET_CASH_SHOP, SetCashShopHandler>();

		// TODO: New handlers, they need coded and moved to a proper file.
		emplace<CHECK_SPW_RESULT, CheckSpwResultHandler>();
		emplace<FIELD_EFFECT, FieldEffectHandler>();
	}

	void PacketSwitch::forward(const int8_t* bytes, size_t length) const
	{
		// Wrap the bytes with a parser
		InPacket recv = { bytes, length };

		// Read the opcode to determine handler responsible
		uint16_t opcode = recv.read_short();

		bool opcode_error = false;

		if (opcode < NUM_HANDLERS)
		{
			if (auto& handler = handlers[opcode])
			{
				// Handler is good, packet is passed on

				try
				{
					handler->handle(recv);
				}
				catch (const PacketError& err)
				{
					// Notice about an error
					warn(err.what(), opcode);
					opcode_error = true;
				}
			}
			else
			{
				// Warn about an unhandled packet
				warn(MSG_UNHANDLED, opcode);
				opcode_error = true;
			}
		}
		else
		{
			// Warn about a packet with opcode out of bounds
			warn(MSG_OUTOFBOUNDS, opcode);
			opcode_error = true;
		}

#if LOG_LEVEL >= LOG_NETWORK
		if (!opcode_error)
			LOG(LOG_NETWORK, "Received Packet: " << OpcodeName(opcode));
#endif
	}

	void PacketSwitch::warn(const std::string& message, size_t opcode) const
	{
		std::string opcode_msg = OpcodeName(opcode);
		LOG(LOG_NETWORK, "[PacketSwitch::warn]: Opcode [" << opcode_msg << "] Error: " << message);
	}

	std::string PacketSwitch::OpcodeName(size_t opcode) const
	{
		std::string opcode_msg = std::to_string(opcode);

		switch (opcode)
		{
			case 0: opcode_msg = "LOGIN_STATUS"; break;
			case 1: opcode_msg = "GUEST_ID_LOGIN"; break;
			case 2: opcode_msg = "ACCOUNT_INFO"; break;
			case 3: opcode_msg = "SERVERSTATUS"; break;
			case 4: opcode_msg = "GENDER_DONE"; break;
			case 5: opcode_msg = "CONFIRM_EULA_RESULT"; break;
			case 6: opcode_msg = "CHECK_PINCODE"; break;
			case 7: opcode_msg = "UPDATE_PINCODE"; break;
			case 8: opcode_msg = "VIEW_ALL_CHAR"; break;
			case 9: opcode_msg = "SELECT_CHARACTER_BY_VAC"; break;
			case 10: opcode_msg = "SERVERLIST"; break;
			case 11: opcode_msg = "CHARLIST"; break;
			case 12: opcode_msg = "SERVER_IP"; break;
			case 13: opcode_msg = "CHAR_NAME_RESPONSE"; break;
			case 14: opcode_msg = "ADD_NEW_CHAR_ENTRY"; break;
			case 15: opcode_msg = "DELETE_CHAR_RESPONSE"; break;
			case 16: opcode_msg = "CHANGE_CHANNEL"; break;
			case 17: opcode_msg = "PING"; break;
			case 18: opcode_msg = "KOREAN_INTERNET_CAFE_SHIT"; break;
			case 20: opcode_msg = "CHANNEL_SELECTED"; break;
			case 21: opcode_msg = "HACKSHIELD_REQUEST"; break;
			case 22: opcode_msg = "RELOG_RESPONSE"; break;
			case 25: opcode_msg = "CHECK_CRC_RESULT"; break;
			case 26: opcode_msg = "LAST_CONNECTED_WORLD"; break;
			case 27: opcode_msg = "RECOMMENDED_WORLD_MESSAGE"; break;
			case 28: opcode_msg = "CHECK_SPW_RESULT"; break;
			case 29: opcode_msg = "INVENTORY_OPERATION"; break;
			case 30: opcode_msg = "INVENTORY_GROW"; break;
			case 31: opcode_msg = "STAT_CHANGED"; break;
			case 32: opcode_msg = "GIVE_BUFF"; break;
			case 33: opcode_msg = "CANCEL_BUFF"; break;
			case 34: opcode_msg = "FORCED_STAT_SET"; break;
			case 35: opcode_msg = "FORCED_STAT_RESET"; break;
			case 36: opcode_msg = "UPDATE_SKILLS"; break;
			case 37: opcode_msg = "SKILL_USE_RESULT"; break;
			case 38: opcode_msg = "FAME_RESPONSE"; break;
			case 39: opcode_msg = "SHOW_STATUS_INFO"; break;
			case 40: opcode_msg = "OPEN_FULL_CLIENT_DOWNLOAD_LINK"; break;
			case 41: opcode_msg = "MEMO_RESULT"; break;
			case 42: opcode_msg = "MAP_TRANSFER_RESULT"; break;
			case 43: opcode_msg = "WEDDING_PHOTO"; break;
			case 45: opcode_msg = "CLAIM_RESULT"; break;
			case 46: opcode_msg = "CLAIM_AVAILABLE_TIME"; break;
			case 47: opcode_msg = "CLAIM_STATUS_CHANGED"; break;
			case 48: opcode_msg = "SET_TAMING_MOB_INFO"; break;
			case 49: opcode_msg = "QUEST_CLEAR"; break;
			case 50: opcode_msg = "ENTRUSTED_SHOP_CHECK_RESULT"; break;
			case 51: opcode_msg = "SKILL_LEARN_ITEM_RESULT"; break;
			case 52: opcode_msg = "GATHER_ITEM_RESULT"; break;
			case 53: opcode_msg = "SORT_ITEM_RESULT"; break;
			case 55: opcode_msg = "SUE_CHARACTER_RESULT"; break;
			case 57: opcode_msg = "TRADE_MONEY_LIMIT"; break;
			case 58: opcode_msg = "SET_GENDER"; break;
			case 59: opcode_msg = "GUILD_BBS_PACKET"; break;
			case 61: opcode_msg = "CHAR_INFO"; break;
			case 62: opcode_msg = "PARTY_OPERATION"; break;
			case 63: opcode_msg = "BUDDYLIST"; break;
			case 65: opcode_msg = "GUILD_OPERATION"; break;
			case 66: opcode_msg = "ALLIANCE_OPERATION"; break;
			case 67: opcode_msg = "SPAWN_PORTAL"; break;
			case 68: opcode_msg = "SERVERMESSAGE"; break;
			case 69: opcode_msg = "INCUBATOR_RESULT"; break;
			case 70: opcode_msg = "SHOP_SCANNER_RESULT"; break;
			case 71: opcode_msg = "SHOP_LINK_RESULT"; break;
			case 72: opcode_msg = "MARRIAGE_REQUEST"; break;
			case 73: opcode_msg = "MARRIAGE_RESULT"; break;
			case 74: opcode_msg = "WEDDING_GIFT_RESULT"; break;
			case 75: opcode_msg = "NOTIFY_MARRIED_PARTNER_MAP_TRANSFER"; break;
			case 76: opcode_msg = "CASH_PET_FOOD_RESULT"; break;
			case 77: opcode_msg = "SET_WEEK_EVENT_MESSAGE"; break;
			case 78: opcode_msg = "SET_POTION_DISCOUNT_RATE"; break;
			case 79: opcode_msg = "BRIDLE_MOB_CATCH_FAIL"; break;
			case 80: opcode_msg = "IMITATED_NPC_RESULT"; break;
			case 81: opcode_msg = "IMITATED_NPC_DATA"; break;
			case 82: opcode_msg = "LIMITED_NPC_DISABLE_INFO"; break;
			case 83: opcode_msg = "MONSTER_BOOK_SET_CARD"; break;
			case 84: opcode_msg = "MONSTER_BOOK_SET_COVER"; break;
			case 85: opcode_msg = "HOUR_CHANGED"; break;
			case 86: opcode_msg = "MINIMAP_ON_OFF"; break;
			case 87: opcode_msg = "CONSULT_AUTHKEY_UPDATE"; break;
			case 88: opcode_msg = "CLASS_COMPETITION_AUTHKEY_UPDATE"; break;
			case 89: opcode_msg = "WEB_BOARD_AUTHKEY_UPDATE"; break;
			case 90: opcode_msg = "SESSION_VALUE"; break;
			case 91: opcode_msg = "PARTY_VALUE"; break;
			case 92: opcode_msg = "FIELD_SET_VARIABLE"; break;
			case 93: opcode_msg = "BONUS_EXP_CHANGED"; break;
			case 94: opcode_msg = "FAMILY_CHART_RESULT"; break;
			case 95: opcode_msg = "FAMILY_INFO_RESULT"; break;
			case 96: opcode_msg = "FAMILY_RESULT"; break;
			case 97: opcode_msg = "FAMILY_JOIN_REQUEST"; break;
			case 98: opcode_msg = "FAMILY_JOIN_REQUEST_RESULT"; break;
			case 99: opcode_msg = "FAMILY_JOIN_ACCEPTED"; break;
			case 100: opcode_msg = "FAMILY_PRIVILEGE_LIST"; break;
			case 101: opcode_msg = "FAMILY_FAMOUS_POINT_INC_RESULT"; break;
			case 102: opcode_msg = "FAMILY_NOTIFY_LOGIN_OR_LOGOUT"; break;
			case 103: opcode_msg = "FAMILY_SET_PRIVILEGE"; break;
			case 104: opcode_msg = "FAMILY_SUMMON_REQUEST"; break;
			case 105: opcode_msg = "NOTIFY_LEVELUP"; break;
			case 106: opcode_msg = "NOTIFY_MARRIAGE"; break;
			case 107: opcode_msg = "NOTIFY_JOB_CHANGE"; break;
			case 108: opcode_msg = "SET_BUY_EQUIP_EXT"; break;
			case 109: opcode_msg = "MAPLE_TV_USE_RES"; break;
			case 110: opcode_msg = "AVATAR_MEGAPHONE_RESULT"; break;
			case 111: opcode_msg = "SET_AVATAR_MEGAPHONE"; break;
			case 112: opcode_msg = "CLEAR_AVATAR_MEGAPHONE"; break;
			case 113: opcode_msg = "CANCEL_NAME_CHANGE_RESULT"; break;
			case 114: opcode_msg = "CANCEL_TRANSFER_WORLD_RESULT"; break;
			case 115: opcode_msg = "DESTROY_SHOP_RESULT"; break;
			case 116: opcode_msg = "FAKE_GM_NOTICE"; break;
			case 117: opcode_msg = "SUCCESS_IN_USE_GACHAPON_BOX"; break;
			case 118: opcode_msg = "NEW_YEAR_CARD_RES"; break;
			case 119: opcode_msg = "RANDOM_MORPH_RES"; break;
			case 120: opcode_msg = "CANCEL_NAME_CHANGE_BY_OTHER"; break;
			case 121: opcode_msg = "SET_EXTRA_PENDANT_SLOT"; break;
			case 122: opcode_msg = "SCRIPT_PROGRESS_MESSAGE"; break;
			case 123: opcode_msg = "DATA_CRC_CHECK_FAILED"; break;
			case 124: opcode_msg = "MACRO_SYS_DATA_INIT"; break;
			case 125: opcode_msg = "SET_FIELD"; break;
			case 126: opcode_msg = "SET_ITC"; break;
			case 127: opcode_msg = "SET_CASH_SHOP"; break;
			case 128: opcode_msg = "SET_BACK_EFFECT"; break;
			case 129: opcode_msg = "SET_MAP_OBJECT_VISIBLE"; break;
			case 130: opcode_msg = "CLEAR_BACK_EFFECT"; break;
			case 131: opcode_msg = "BLOCKED_MAP"; break;
			case 132: opcode_msg = "BLOCKED_SERVER"; break;
			case 133: opcode_msg = "FORCED_MAP_EQUIP"; break;
			case 134: opcode_msg = "MULTICHAT"; break;
			case 135: opcode_msg = "WHISPER"; break;
			case 136: opcode_msg = "SPOUSE_CHAT"; break;
			case 137: opcode_msg = "SUMMON_ITEM_INAVAILABLE"; break;
			case 138: opcode_msg = "FIELD_EFFECT"; break;
			case 139: opcode_msg = "FIELD_OBSTACLE_ONOFF"; break;
			case 140: opcode_msg = "FIELD_OBSTACLE_ONOFF_LIST"; break;
			case 141: opcode_msg = "FIELD_OBSTACLE_ALL_RESET"; break;
			case 142: opcode_msg = "BLOW_WEATHER"; break;
			case 143: opcode_msg = "PLAY_JUKEBOX"; break;
			case 144: opcode_msg = "ADMIN_RESULT"; break;
			case 145: opcode_msg = "OX_QUIZ"; break;
			case 146: opcode_msg = "GMEVENT_INSTRUCTIONS"; break;
			case 147: opcode_msg = "CLOCK"; break;
			case 148: opcode_msg = "CONTI_MOVE"; break;
			case 149: opcode_msg = "CONTI_STATE"; break;
			case 150: opcode_msg = "SET_QUEST_CLEAR"; break;
			case 151: opcode_msg = "SET_QUEST_TIME"; break;
			case 152: opcode_msg = "WARN_MESSAGE"; break;
			case 153: opcode_msg = "SET_OBJECT_STATE"; break;
			case 154: opcode_msg = "STOP_CLOCK"; break;
			case 155: opcode_msg = "ARIANT_ARENA_SHOW_RESULT"; break;
			case 157: opcode_msg = "PYRAMID_GAUGE"; break;
			case 158: opcode_msg = "PYRAMID_SCORE"; break;
			case 160: opcode_msg = "SPAWN_PLAYER"; break;
			case 161: opcode_msg = "REMOVE_PLAYER_FROM_MAP"; break;
			case 162: opcode_msg = "CHATTEXT"; break;
			case 163: opcode_msg = "CHATTEXT1"; break;
			case 164: opcode_msg = "CHALKBOARD"; break;
			case 165: opcode_msg = "UPDATE_CHAR_BOX"; break;
			case 166: opcode_msg = "SHOW_CONSUME_EFFECT"; break;
			case 167: opcode_msg = "SHOW_SCROLL_EFFECT"; break;
			case 168: opcode_msg = "SPAWN_PET"; break;
			case 170: opcode_msg = "MOVE_PET"; break;
			case 171: opcode_msg = "PET_CHAT"; break;
			case 172: opcode_msg = "PET_NAMECHANGE"; break;
			case 173: opcode_msg = "PET_EXCEPTION_LIST"; break;
			case 174: opcode_msg = "PET_COMMAND"; break;
			case 175: opcode_msg = "SPAWN_SPECIAL_MAPOBJECT"; break;
			case 176: opcode_msg = "REMOVE_SPECIAL_MAPOBJECT"; break;
			case 177: opcode_msg = "MOVE_SUMMON"; break;
			case 178: opcode_msg = "SUMMON_ATTACK"; break;
			case 179: opcode_msg = "DAMAGE_SUMMON"; break;
			case 180: opcode_msg = "SUMMON_SKILL"; break;
			case 181: opcode_msg = "SPAWN_DRAGON"; break;
			case 182: opcode_msg = "MOVE_DRAGON"; break;
			case 183: opcode_msg = "REMOVE_DRAGON"; break;
			case 185: opcode_msg = "MOVE_PLAYER"; break;
			case 186: opcode_msg = "CLOSE_RANGE_ATTACK"; break;
			case 187: opcode_msg = "RANGED_ATTACK"; break;
			case 188: opcode_msg = "MAGIC_ATTACK"; break;
			case 189: opcode_msg = "ENERGY_ATTACK"; break;
			case 190: opcode_msg = "SKILL_EFFECT"; break;
			case 191: opcode_msg = "CANCEL_SKILL_EFFECT"; break;
			case 192: opcode_msg = "DAMAGE_PLAYER"; break;
			case 193: opcode_msg = "FACIAL_EXPRESSION"; break;
			case 194: opcode_msg = "SHOW_ITEM_EFFECT"; break;
			case 196: opcode_msg = "SHOW_CHAIR"; break;
			case 197: opcode_msg = "UPDATE_CHAR_LOOK"; break;
			case 198: opcode_msg = "SHOW_FOREIGN_EFFECT"; break;
			case 199: opcode_msg = "GIVE_FOREIGN_BUFF"; break;
			case 200: opcode_msg = "CANCEL_FOREIGN_BUFF"; break;
			case 201: opcode_msg = "UPDATE_PARTYMEMBER_HP"; break;
			case 202: opcode_msg = "GUILD_NAME_CHANGED"; break;
			case 203: opcode_msg = "GUILD_MARK_CHANGED"; break;
			case 204: opcode_msg = "THROW_GRENADE"; break;
			case 205: opcode_msg = "CANCEL_CHAIR"; break;
			case 206: opcode_msg = "SHOW_ITEM_GAIN_INCHAT"; break;
			case 207: opcode_msg = "DOJO_WARP_UP"; break;
			case 208: opcode_msg = "LUCKSACK_PASS"; break;
			case 209: opcode_msg = "LUCKSACK_FAIL"; break;
			case 210: opcode_msg = "MESO_BAG_MESSAGE"; break;
			case 211: opcode_msg = "UPDATE_QUEST_INFO"; break;
			case 214: opcode_msg = "PLAYER_HINT"; break;
			case 219: opcode_msg = "KOREAN_EVENT"; break;
			case 220: opcode_msg = "OPEN_UI"; break;
			case 221: opcode_msg = "LOCK_UI"; break;
			case 222: opcode_msg = "DISABLE_UI"; break;
			case 223: opcode_msg = "SPAWN_GUIDE"; break;
			case 224: opcode_msg = "TALK_GUIDE"; break;
			case 225: opcode_msg = "SHOW_COMBO"; break;
			case 234: opcode_msg = "COOLDOWN"; break;
			case 236: opcode_msg = "SPAWN_MONSTER"; break;
			case 237: opcode_msg = "KILL_MONSTER"; break;
			case 238: opcode_msg = "SPAWN_MONSTER_CONTROL"; break;
			case 239: opcode_msg = "MOVE_MONSTER"; break;
			case 240: opcode_msg = "MOVE_MONSTER_RESPONSE"; break;
			case 242: opcode_msg = "APPLY_MONSTER_STATUS"; break;
			case 243: opcode_msg = "CANCEL_MONSTER_STATUS"; break;
			case 244: opcode_msg = "RESET_MONSTER_ANIMATION"; break;
			case 246: opcode_msg = "DAMAGE_MONSTER"; break;
			case 249: opcode_msg = "ARIANT_THING"; break;
			case 250: opcode_msg = "SHOW_MONSTER_HP"; break;
			case 251: opcode_msg = "CATCH_MONSTER"; break;
			case 252: opcode_msg = "CATCH_MONSTER_WITH_ITEM"; break;
			case 253: opcode_msg = "SHOW_MAGNET"; break;
			case 257: opcode_msg = "SPAWN_NPC"; break;
			case 258: opcode_msg = "REMOVE_NPC"; break;
			case 259: opcode_msg = "SPAWN_NPC_REQUEST_CONTROLLER"; break;
			case 260: opcode_msg = "NPC_ACTION"; break;
			case 265: opcode_msg = "SPAWN_HIRED_MERCHANT"; break;
			case 266: opcode_msg = "DESTROY_HIRED_MERCHANT"; break;
			case 267: opcode_msg = "UPDATE_HIRED_MERCHANT"; break;
			case 268: opcode_msg = "DROP_ITEM_FROM_MAPOBJECT"; break;
			case 269: opcode_msg = "REMOVE_ITEM_FROM_MAP"; break;
			case 270: opcode_msg = "CANNOT_SPAWN_KITE"; break;
			case 271: opcode_msg = "SPAWN_KITE"; break;
			case 272: opcode_msg = "REMOVE_KITE"; break;
			case 273: opcode_msg = "SPAWN_MIST"; break;
			case 274: opcode_msg = "REMOVE_MIST"; break;
			case 275: opcode_msg = "SPAWN_DOOR"; break;
			case 276: opcode_msg = "REMOVE_DOOR"; break;
			case 277: opcode_msg = "REACTOR_HIT"; break;
			case 279: opcode_msg = "REACTOR_SPAWN"; break;
			case 280: opcode_msg = "REACTOR_DESTROY"; break;
			case 281: opcode_msg = "SNOWBALL_STATE"; break;
			case 282: opcode_msg = "HIT_SNOWBALL"; break;
			case 283: opcode_msg = "SNOWBALL_MESSAGE"; break;
			case 284: opcode_msg = "LEFT_KNOCK_BACK"; break;
			case 285: opcode_msg = "COCONUT_HIT"; break;
			case 286: opcode_msg = "COCONUT_SCORE"; break;
			case 287: opcode_msg = "GUILD_BOSS_HEALER_MOVE"; break;
			case 288: opcode_msg = "GUILD_BOSS_PULLEY_STATE_CHANGE"; break;
			case 289: opcode_msg = "MONSTER_CARNIVAL_START"; break;
			case 290: opcode_msg = "MONSTER_CARNIVAL_OBTAINED_CP"; break;
			case 291: opcode_msg = "MONSTER_CARNIVAL_PARTY_CP"; break;
			case 292: opcode_msg = "MONSTER_CARNIVAL_SUMMON"; break;
			case 293: opcode_msg = "MONSTER_CARNIVAL_MESSAGE"; break;
			case 294: opcode_msg = "MONSTER_CARNIVAL_DIED"; break;
			case 295: opcode_msg = "MONSTER_CARNIVAL_LEAVE"; break;
			case 297: opcode_msg = "ARIANT_ARENA_USER_SCORE"; break;
			case 299: opcode_msg = "SHEEP_RANCH_INFO"; break;
			case 300: opcode_msg = "SHEEP_RANCH_CLOTHES"; break;
			case 301: opcode_msg = "ARIANT_SCORE"; break;
			case 302: opcode_msg = "HORNTAIL_CAVE"; break;
			case 303: opcode_msg = "ZAKUM_SHRINE"; break;
			case 304: opcode_msg = "NPC_TALK"; break;
			case 305: opcode_msg = "OPEN_NPC_SHOP"; break;
			case 306: opcode_msg = "CONFIRM_SHOP_TRANSACTION"; break;
			case 307: opcode_msg = "ADMIN_SHOP_MESSAGE"; break;
			case 308: opcode_msg = "ADMIN_SHOP"; break;
			case 309: opcode_msg = "STORAGE"; break;
			case 310: opcode_msg = "FREDRICK_MESSAGE"; break;
			case 311: opcode_msg = "FREDRICK"; break;
			case 312: opcode_msg = "RPS_GAME"; break;
			case 313: opcode_msg = "MESSENGER"; break;
			case 314: opcode_msg = "PLAYER_INTERACTION"; break;
			case 315: opcode_msg = "TOURNAMENT"; break;
			case 316: opcode_msg = "TOURNAMENT_MATCH_TABLE"; break;
			case 317: opcode_msg = "TOURNAMENT_SET_PRIZE"; break;
			case 318: opcode_msg = "TOURNAMENT_UEW"; break;
			case 319: opcode_msg = "TOURNAMENT_CHARACTERS"; break;
			case 320: opcode_msg = "WEDDING_PROGRESS"; break;
			case 321: opcode_msg = "WEDDING_CEREMONY_END"; break;
			case 322: opcode_msg = "PARCEL"; break;
			case 323: opcode_msg = "CHARGE_PARAM_RESULT"; break;
			case 324: opcode_msg = "QUERY_CASH_RESULT"; break;
			case 325: opcode_msg = "CASHSHOP_OPERATION"; break;
			case 327: opcode_msg = "CASHSHOP_GIFT_INFO_RESULT"; break;
			case 328: opcode_msg = "CASHSHOP_CHECK_NAME_CHANGE"; break;
			case 329: opcode_msg = "CASHSHOP_CHECK_NAME_CHANGE_POSSIBLE_RESULT"; break;
			case 330: opcode_msg = "CASHSHOP_REGISTER_NEW_CHARACTER_RESULT"; break;
			case 331: opcode_msg = "CASHSHOP_CHECK_TRANSFER_WORLD_POSSIBLE_RESULT"; break;
			case 332: opcode_msg = "CASHSHOP_GACHAPON_STAMP_RESULT"; break;
			case 333: opcode_msg = "CASHSHOP_CASH_ITEM_GACHAPON_RESULT"; break;
			case 334: opcode_msg = "CASHSHOP_CASH_GACHAPON_OPEN_RESULT"; break;
			case 335: opcode_msg = "KEYMAP"; break;
			case 336: opcode_msg = "AUTO_HP_POT"; break;
			case 337: opcode_msg = "AUTO_MP_POT"; break;
			case 341: opcode_msg = "SEND_TV"; break;
			case 342: opcode_msg = "REMOVE_TV"; break;
			case 343: opcode_msg = "ENABLE_TV"; break;
			case 347: opcode_msg = "MTS_OPERATION2"; break;
			case 348: opcode_msg = "MTS_OPERATION"; break;
			case 349: opcode_msg = "MAPLELIFE_RESULT"; break;
			case 350: opcode_msg = "MAPLELIFE_ERROR"; break;
			case 354: opcode_msg = "VICIOUS_HAMMER"; break;
			case 358: opcode_msg = "VEGA_SCROLL"; break;
			default: break;
		}

		return opcode_msg;
	}
}