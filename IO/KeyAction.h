//////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015-2016 Daniel Allendorf                                   //
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
#include <cstdint>

namespace jrc
{
	namespace KeyAction
	{
		// Maple-specific keycodes, sent via the Keymap Packet.
		enum Id : int32_t
		{
			EQUIPMENT = 0,
			ITEMS = 1,
			STATS = 2,
			SKILLS = 3,
			FRIENDS = 4,
			WORLDMAP = 5,
			MAPLECHAT = 6,
			MINIMAP = 7,
			QUESTLOG = 8,
			KEYBINDINGS = 9,
			TOGGLECHAT = 10,
			WHISPER = 11,
			SAY = 12,
			PARTYCHAT = 13,
			MENU = 14,
			QUICKSLOTS = 15,
			GUILD = 17,
			FRIENDSCHAT = 18,
			PARTY = 19,
			NOTIFIER = 20,
			CASHSHOP = 23,
			GUILDCHAT = 24,
			MEDALS = 27,
			BITS,
			ALLIANCECHAT,
			MAPLENEWS,
			MANAGELEGION,
			PROFESSION,
			BOSSPARTY,
			ITEMPOT,
			EVENT,
			SILENTCRUSADE,
			BATTLEANALYSIS,
			GUIDE,
			VIEWERSCHAT,
			ENHANCEEQUIP,
			MONSTERCOLLECTION,
			SOULWEAPON,
			CHARINFO,
			CHANGECHANNEL,
			MAINMENU,
			SCREENSHOT,
			PICTUREMODE,
			MAPLEACHIEVEMENT,
			PICKUP = 50,
			SIT = 51,
			ATTACK = 52,
			JUMP = 53,
			INTERACT_HARVEST = 54,
			MAPLESTORAGE,
			SAFEMODE,
			MUTE,
			MONSTERBOOK,
			TOSPOUSE,
			FACE1 = 100,
			FACE2 = 101,
			FACE3 = 102,
			FACE4 = 103,
			FACE5 = 104,
			FACE6 = 105,
			FACE7 = 106,
			// Static keys
			LEFT,
			RIGHT,
			UP,
			DOWN,
			BACK,
			TAB,
			RETURN,
			ESCAPE,
			SPACE,
			DELETE,
			HOME,
			END,
			COPY,
			PASTE,
			LENGTH
		};

		inline Id actionbyid(int32_t id)
		{
			return static_cast<Id>(id);
		}
	}
}