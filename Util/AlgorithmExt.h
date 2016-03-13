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
#include <map>
#include <unordered_map>
#include <functional>
#include <vector>

class Algorithm
{
public:
	template<typename K, typename V>
	static void remove_all(std::unordered_map<K, V>& container, std::function<bool(V&)> predicate)
	{
		std::vector<K> toremove;
		for (auto& it : container)
		{
			if (predicate(it.second))
			{
				toremove.push_back(it.first);
			}
		}
		for (auto& it : toremove)
		{
			container.erase(it);
		}
	}
};