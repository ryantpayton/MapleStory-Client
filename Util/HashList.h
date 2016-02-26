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
#include "Optional.h"
#include <map>
#include <vector>

using std::map;
using std::vector;

template <typename T>
class HashList
{
public:
	HashList()
	{
		index = 0;
	}

	Optional<T> get(size_t index) const
	{
		if (values.count(index))
			return values.at(index);
		else
			return nullptr;
	}

	void add(T value)
	{
		values[index] = value;
		index++;
	}

	void remove(size_t index)
	{
		values.erase(index);
	}

	void remove(vector<size_t> indices)
	{
		for (auto& index : indices)
		{
			values.erase(index);
		}
	}

	template<typename T, typename ...Args>
	void foreach(void(T::*action)(Args...)const, Args... args) const
	{
		for (auto& value : values)
		{
			(value.second.*action)(std::forward<Args>(args)...);
		}
	}

	template<typename T, typename ...Args>
	vector<size_t> collect(bool(T::*predicate)(Args...), Args... args)
	{
		vector<size_t> result;
		for (auto& value : values)
		{
			if ((value.second.*predicate)(std::forward<Args>(args)...))
				result.push_back(value.first);
		}
		return result;
	}

private:
	map<size_t, T> values;
	size_t index;
};