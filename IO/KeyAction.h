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
#pragma once

#include <cstdint>

namespace ms
{
	namespace KeyAction
	{
		// Maple-specific keycodes, sent via the Keymap Packet.
		enum Id : int32_t
		{
			EQUIPMENT,
			ITEMS,
			STATS,
			SKILLS,
			FRIENDS,
			WORLDMAP,
			MAPLECHAT,
			MINIMAP,
			QUESTLOG,
			KEYBINDINGS,
			SAY,
			WHISPER,
			PARTYCHAT,
			FRIENDSCHAT,
			MENU,
			QUICKSLOTS,
			TOGGLECHAT,
			GUILD,
			GUILDCHAT,
			PARTY,
			NOTIFIER,
			MAPLENEWS,			// TOSPOUSE (v83)
			CASHSHOP,			// MONSTERBOOK (v83)
			ALLIANCECHAT,		// CASHSHOP (v83)
			//NONE = 24,		// TOALLIANCE (v83)
			MANAGELEGION = 25,	// PARTYSEARCH (v83)
			MEDALS,				// FAMILY (v83)
			BOSSPARTY,			// MEDAL (v83)
			PROFESSION = 29,
			ITEMPOT,
			EVENT,
			SILENTCRUSADE = 33,
			BITS,
			BATTLEANALYSIS,
			GUIDE = 39,
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
			PICKUP,
			SIT,
			ATTACK,
			JUMP,
			INTERACT_HARVEST,
			FACE1 = 100,
			FACE2,
			FACE3,
			FACE4,
			FACE5,
			FACE6,
			FACE7,
			MAPLESTORAGE = 200,
			SAFEMODE,
			MUTE,
			EMOTICON,
			MAPLERELAY = 300,
			FAMILIAR = 1000,
			TOSPOUSE,
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