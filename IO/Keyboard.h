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

#include "KeyAction.h"
#include "KeyType.h"

#include <map>

namespace ms
{
	class Keyboard
	{
	public:
		struct Mapping
		{
			KeyType::Id type;
			std::int32_t action;

			Mapping() : type(KeyType::Id::NONE), action(0) {}
			Mapping(KeyType::Id in_type, std::int32_t in_action) : type(in_type), action(in_action) {}
		};

		Keyboard();

		void assign(std::uint8_t key, std::uint8_t type, std::int32_t action);
		void remove(std::uint8_t key);

		std::int32_t leftshiftcode() const;
		std::int32_t rightshiftcode() const;
		std::int32_t capslockcode() const;
		std::int32_t leftctrlcode() const;
		std::int32_t rightctrlcode() const;
		std::map<std::int32_t, Mapping> get_maplekeys() const;
		KeyAction::Id get_ctrl_action(std::int32_t keycode) const;
		Mapping get_mapping(std::int32_t keycode) const;
		Mapping get_maple_mapping(std::int32_t keycode) const;
		Mapping get_text_mapping(std::int32_t keycode, bool shift) const;

	private:
		std::map<std::int32_t, Mapping> keymap;
		std::map<std::int32_t, Mapping> maplekeys;
		std::map<std::int32_t, KeyAction::Id> textactions;
		std::map<std::int32_t, bool> keystate;
	};
}