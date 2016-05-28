/////////////////////////////////////////////////////////////////////////////
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
#include "..\Constants.h"

#include <cstdint>

namespace jrc
{
	class TimedBool
	{
	public:
		TimedBool()
		{
			value = false;
			delay = 0;
		}

		explicit operator bool() const
		{
			return value;
		}

		void setfor(uint16_t millis)
		{
			delay = millis;
			value = true;
		}

		void update()
		{
			update(Constants::TIMESTEP);
		}

		void update(uint16_t timestep)
		{
			if (value)
			{
				if (timestep >= delay)
				{
					value = false;
					delay = 0;
				}
				else
				{
					delay -= timestep;
				}
			}
		}

		void operator = (bool b)
		{
			value = b;
			delay = 0;
		}

		bool operator == (bool b)
		{
			return value == b;
		}

		bool operator != (bool b)
		{
			return value != b;
		}

	private:
		uint16_t delay;
		bool value;
	};
}