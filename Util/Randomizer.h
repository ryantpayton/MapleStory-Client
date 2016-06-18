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
#include <cstdint>
#include <random>

namespace jrc
{
	// Small class which can be used to generate random numbers.
	// Classes that need this functionality should have a member variable of this class.
	class Randomizer
	{
	public:
		bool nextbool() const
		{
			return nextint<int32_t>(1) == 1;
		}

		bool below(float percent) const
		{
			return nextreal<float>(1.0f) < percent;
		}

		template <class T>
		T nextreal(T high) const
		{
			return nextreal<T>(0, high);
		}

		template <class T>
		T nextreal(T low, T high) const
		{
			std::uniform_real_distribution<T> range(low, high);
			return chance(range);
		}

		size_t nextint(size_t high) const
		{
			return nextint<size_t>(high);
		}

		size_t nextint(size_t low, size_t high) const
		{
			return nextint<size_t>(low, high);
		}

		template <class T>
		T nextint(T high) const
		{
			return nextint<T>(0, high);
		}

		template <class T>
		T nextint(T low, T high) const
		{
			std::uniform_int_distribution<T> range(low, high);
			return chance(range);
		}

	private:
		template <class T>
		T chance(std::uniform_int_distribution<T> range) const
		{
			std::random_device rd;
			std::default_random_engine engine = std::default_random_engine(rd());
			return range(engine);
		}

		template <class T>
		T chance(std::uniform_real_distribution<T> range) const
		{
			std::random_device rd;
			std::default_random_engine engine = std::default_random_engine(rd());
			return range(engine);
		}
	};
}