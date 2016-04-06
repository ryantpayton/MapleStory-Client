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
#include <stdexcept>

template <typename T>
class BoolPair
{
public:
	BoolPair(T f, T s)
	{
		first = f;
		second = s;
	}

	BoolPair() {}

	template <typename...Args>
	void set(bool b, Args&&...args)
	{
		if (b)
		{
			first = T(std::forward<Args>(args)...);
		}
		else
		{
			second = T(std::forward<Args>(args)...);
		}
	}

	T& operator [](bool b)
	{
		return b ? first : second;
	}

	const T& operator [](bool b) const
	{
		return b ? first : second;
	}

	template <typename T>
	class iterator
	{
	public:
		iterator(BoolPair<T>& p, size_t i)
			: parent(p), index(i) {}

		pair<bool, T&> operator *()
		{
			return pair<bool, T&>(first(), second());
		}

		bool first() const
		{
			return index == 0;
		}

		T& second()
		{
			switch (index)
			{
			case 0:
				return parent[true];
			case 1:
				return parent[false];
			default:
				throw std::out_of_range("iterator out of range");
			}
		}

		iterator<T>& operator ++() 
		{
			index++;
			return *this;
		}

		bool operator != (const iterator<T>& other) const
		{
			return index != other.index;
		}

	private:
		iterator& operator = (const iterator&) = delete;

		BoolPair<T>& parent;
		size_t index;
	};

	template <typename T>
	class const_iterator
	{
	public:
		const_iterator(const BoolPair<T>& p, size_t i)
			: parent(p), index(i) {}

		pair<bool, const T&> operator *() const
		{
			return pair<bool, T&>(first(), second());
		}

		bool first() const
		{
			return index == 0;
		}

		const T& second() const
		{
			switch (index)
			{
			case 0:
				return parent[true];
			case 1:
				return parent[false];
			default:
				throw std::out_of_range("iterator out of range");
			}
		}

		iterator<T>& operator ++()
		{
			index++;
			return *this;
		}

		bool operator != (const iterator<T>& other) const
		{
			return index != other.index;
		}

	private:
		const_iterator& operator = (const const_iterator&) = delete;

		const BoolPair<T>& parent;
		size_t index;
	};

	iterator<T> begin()
	{
		return iterator<T>(*this, 0);
	}

	iterator<T> end()
	{
		return iterator<T>(*this, 2);
	}

	const_iterator<T> begin() const
	{
		return iterator<T>(*this, 0);
	}

	const_iterator<T> end() const
	{
		return iterator<T>(*this, 2);
	}

private:
	T first;
	T second;
};