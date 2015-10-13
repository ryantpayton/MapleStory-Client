/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015 SYJourney                                               //
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

using namespace std;

namespace Util
{
	template <typename K, typename V>
	class ptrmap
	{
	public:
		ptrmap()
		{
		}

		~ptrmap()
		{
			clear();
		}

		V get(K key)
		{
			return contains(key) ? stdmap[key] : 0;
		}

		bool contains(K k)
		{
			return stdmap.count(k) > 0;
		}

		typename map<K, V>::iterator getbegin()
		{
			return stdmap.begin();
		}

		typename map<K, V>::iterator getend()
		{
			return stdmap.end();
		}

		void add(K key, V value)
		{
			if (value != 0)
			{
				if (contains(key))
				{
					remove(key);
				}
				stdmap[key] = value;
			}
		}

		void remove(K key)
		{
			if (contains(key))
			{
				delete stdmap[key];
				stdmap.erase(key);
			}
		}

		void changekey(K from, K to)
		{
			if (contains(from))
			{
				remove(to);
				stdmap[to] = stdmap[from];
				stdmap[from] = 0;
			}
		}

		V extract(K key)
		{
			if (contains(key))
			{
				V ret = stdmap[key];
				stdmap[key] = 0;
				return ret;
			}
			else
			{
				return 0;
			}
		}

		void clear()
		{
			for (map<K, V>::iterator mpit = stdmap.begin(); mpit != stdmap.end(); ++mpit)
			{
				delete mpit->second;
			}
			stdmap.clear();
		}
	private:
		map<K, V> stdmap;
	};
}

