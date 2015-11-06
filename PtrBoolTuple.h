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

namespace Util
{
	// Template for a pair of two dynamically allocated objects of type V, which are differentiated by a bool.
	template <typename V>
	class PtrBoolTuple
	{
	public:
		// Initialise variables with nullptr.
		PtrBoolTuple() 
		{
			first = nullptr;
			second = nullptr;
		}

		// Delete objects on destruction.
		~PtrBoolTuple()
		{
			clear();
		}

		// Return a variable by bool.
		// Parameters: bool(first value)
		V* get(bool b) const
		{
			return b ? first : second;
		}

		// Check if the tuple contains a value.
		// Parameters: bool(first value)
		bool contains(bool b) const
		{
			return b ? (first != nullptr) : (second != nullptr);
		}

		// Add a value to the specified position.
		// Return if an old value was replaced.
		// Parameters: bool(first value), V*(dynamically allocated object)
		bool add(bool b, V* value)
		{
			bool replaced = false;
			if (value)
			{
				if (contains(b))
				{
					remove(b);
					replaced = true;
				}
				padd(b, value);
			}
			return replaced;
		}

		// Remove a value by boolean.
		// Parameters: bool(first value)
		void remove(bool b)
		{
			if (b && first)
			{
				delete first;
			}
			else if (!b && second)
			{
				delete second;
			}
		}

		// Remove both values and destroy them.
		void clear()
		{
			if (first) delete first;
			if (second) delete second;
		}
	private:
		void padd(bool b, V* value)
		{
			if (b) first = value; else second = value;
		}

		V* first;
		V* second;
	};
}