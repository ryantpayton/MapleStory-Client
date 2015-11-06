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
#include "InputConstants.h"
#include "KeyTarget.h"
#include <map>
#include <cstdint>

namespace IO
{
	using::std::int32_t;
	using::std::map;
	using::std::pair;

	class Keyboard
	{
	public:
		Keyboard();
		void addtarget(Keytype, KeyTarget*);
		void addmapping(uint8_t, Keytype, int32_t);
		void focustarget(KeyTarget*);
		void sendinput(bool, int32_t);
	private:
		map<Keytype, KeyTarget*> targets;
		KeyTarget* focused;
		map<int32_t, pair<Keytype, int32_t>> keymap;
		map<int32_t, pair<Keytype, int32_t>> maplekeys;
		map<int32_t, bool> keystate;
	};
}