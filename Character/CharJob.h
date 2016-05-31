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
#include "Look\Weapon.h"
#include <cstdint>
#include <string>

namespace jrc
{
	class CharJob
	{
	public:
		enum Level
		{
			JOB_BEGINNER,
			JOB_FIRST,
			JOB_SECOND,
			JOB_THIRD,
			JOB_FOURTHT
		};

		CharJob(uint16_t id);
		CharJob();
		~CharJob();

		void changejob(uint16_t id);
		bool issubjob(uint16_t subid) const;
		bool can_use(int32_t skill_id) const;
		uint16_t getid() const;
		uint16_t getsubjob(Level level) const;
		std::string getname() const;
		Level getlevel() const;
		Equipstat::Value primarystat(Weapon::Type weapontype) const;
		Equipstat::Value secondarystat(Weapon::Type weapontype) const;

	private:
		std::string getname(uint16_t id) const;

		std::string name;
		uint16_t id;
		Level level;
	};
}