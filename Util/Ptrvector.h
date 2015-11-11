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
#include <vector>

namespace Util
{
	using::std::vector;

	// Template class for managing dynamically allocated objects in a vector.
	// Objects should be allocated when using add(V*).
	template <class V>
	class Ptrvector
	{
	public:
		Ptrvector() 
		{
		}

		~Ptrvector()
		{
			clear();
		}

		void add(V* value)
		{
			if (value != 0)
			{
				stdvector.push_back(value);
			}
		}

		V* get(size_t pos) const
		{
			return (pos < stdvector.size()) ? stdvector.at(pos) : 0;
		}

		size_t getend() const
		{
			return stdvector.size();
		}

		void remove(size_t pos)
		{
			delete stdvector[pos];
			stdvector.erase(stdvector.begin() + pos);
		}

		void clear()
		{
			for (size_t i = 0; i < stdvector.size(); i++)
			{
				delete stdvector[i];
			}

		}
	private:
		vector<V*> stdvector;
	};
}