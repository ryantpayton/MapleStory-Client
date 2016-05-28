//////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2016 Daniel Allendorf                                        //
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
#include "..\Character\Look\Weapon.h"
#include "..\Util\EnumMap.h"

#include <string>

namespace jrc
{
	class Messages
	{
	public:
		enum Type
		{
			// Cannot use a skill
			SKILL_WEAPONTYPE,
			SKILL_HPCOST,
			SKILL_MPCOST,
			SKILL_NOARROWS,
			SKILL_NOBULLETS,
			SKILL_NOSTARS,

			// Scrolling result
			SCROLL_SUCCESS,
			SCROLL_FAILURE,
			SCROLL_DESTROYED,
			LENGTH
		};

		Messages();

		std::string stringfor(Type type);

	private:
		EnumMap<Type, std::string> messages;
	};


	class InChatMessage
	{
	public:
		InChatMessage(Messages::Type type);

		void drop() const;

	private:
		Messages::Type type;
	};


	class NoBulletsMessage : public InChatMessage
	{
	public:
		NoBulletsMessage(Weapon::Type weapon);

	private:
		Messages::Type messagebyweapon(Weapon::Type weapon);
	};
}