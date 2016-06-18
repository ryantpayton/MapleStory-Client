/////////////////////////////////////////////////////////////////////////////
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
#include "Look\Weapon.h"

#include "..\Util\Range.h"

#include <cstdint>
#include <string>

namespace jrc
{
	class Job
	{
	public:
		enum Level : uint16_t
		{
			BEGINNER,
			FIRST,
			SECOND,
			THIRD,
			FOURTHT
		};

		static Level get_next_level(Level level)
		{
			switch (level)
			{
			case BEGINNER:
				return FIRST;
			case FIRST:
				return SECOND;
			case SECOND:
				return THIRD;
			default:
				return FOURTHT;
			}
		}

		Job(uint16_t id);
		Job();
		~Job();

		void change_job(uint16_t id);
		bool is_sub_job(uint16_t subid) const;
		bool can_use(int32_t skill_id) const;
		uint16_t get_id() const;
		uint16_t get_subjob(Level level) const;
		Level get_level() const;
		const std::string& get_name() const;
		Equipstat::Value get_primary(Weapon::Type weapontype) const;
		Equipstat::Value get_secondary(Weapon::Type weapontype) const;
		Range<int32_t> get_skillid_range(Level level) const;

	private:
		std::string get_name(uint16_t id) const;

		std::string name;
		uint16_t id;
		Level level;
	};
}