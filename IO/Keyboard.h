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
#include "..\Util\Optional.h"

#include <map>
#include <cstdint>

namespace jrc
{
	class Keyboard
	{
	public:
		//Maple-specific keycodes, sent via the Keymap Packet.
		enum Action : int32_t
		{
			EQUIPS = 0,
			INVENTORY = 1,
			CHARSTATS = 2,
			SKILLBOOK = 3,
			BUDDYLIST = 4,
			WORLDMAP = 5,
			MESSAGE = 6,
			MINIMAP = 7,
			QUESTLOG = 8,
			KEYCONFIG = 9,
			CHATALL = 10,
			WHISPER = 11,
			CHATPT = 12,
			CHATBUDDY = 13,
			MAINMENU = 14,
			TOGGLEQS = 15,
			CHATWINDOW = 16,
			GUILD = 17,
			CHATGUILD = 18,
			PARTY = 19,
			HELPER = 20,
			CHATSPOUSE = 21,
			MONSTERBOOK = 22,
			CASHSHOP = 23,
			CHATALLIANCE = 24,
			PARTYSEARCH = 25,
			FAMILY = 26,
			GMSMEDALS = 27,
			BOSS = 28,
			CHATSQUAD = 29,
			PROFESSION = 30,
			ITEMPOT = 31,
			EVENT = 32,
			WHEEL = 33,
			LEVELGUIDE = 34,
			CRUSADE = 35,
			BITS = 36,
			KNOWHOW = 37,
			EPISODE = 38,
			GUIDE = 39,
			EQUIPENHANCE = 40,
			EQUIPENHANCE2 = 41,
			PICKUP = 50,
			SIT = 51,
			ATTACK = 52,
			JUMP = 53,
			NPCCHAT = 54,
			LEFT = 60,
			RIGHT = 61,
			UP = 62,
			DOWN = 63,
			BACK = 64,
			RETURN = 65,
			SPACE = 66,
			TAB = 67,
			COPY = 68,
			PASTE = 69,
			NOACTION = 70,
			FACE1 = 100,
			FACE7 = 106
		};

		static Action actionbyid(int32_t id)
		{
			return static_cast<Action>(id);
		}

		// Keytypes decide how a keycode is interpreted.
		enum Keytype
		{
			NONE = 0,
			SKILL = 1,
			ITEM = 2,
			CASH = 3,
			MENU = 4,
			ACTION = 5,
			FACE = 6,
			MACRO = 8,
			LETTER = 9,
			NUMBER = 10
		};

		static Keytype typebyid(uint8_t id)
		{
			if (id < SKILL || id > MACRO)
				return NONE;

			return static_cast<Keytype>(id);
		}

		struct Mapping
		{
			Keyboard::Keytype type;
			int32_t action;

			Mapping()
			{
				type = Keyboard::NONE;
				action = 0;
			}

			Mapping(Keyboard::Keytype t, int32_t a)
			{
				type = t;
				action = a;
			}
		};

		Keyboard();

		void assign(uint8_t key, uint8_t type, int32_t action);
		int32_t shiftcode() const;
		int32_t ctrlcode() const;
		Action getctrlaction(int32_t keycode) const;
		Mapping gettextmapping(int32_t keycode, bool shift) const;
		Optional<const Mapping> getmapping(int32_t keycode) const;

	private:
		std::map<int32_t, Mapping> keymap;
		std::map<int32_t, Mapping> maplekeys;
		std::map<int32_t, Action> textactions;
		std::map<int32_t, bool> keystate;
	};
}