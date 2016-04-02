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
#include <type_traits>
#include <utility>

template <typename K, typename V>
class EnumMap
{
public:
	EnumMap()
	{
		static_assert(std::is_enum<K>::value,
			"Template parameter 'K' for EnumMap must be an enum.");
	}

	void clear()
	{
		for (size_t i = 0; i < K::LENGTH; i++)
		{
			values[i] = V();
		}
	}

	V& operator [](K key)
	{
		return values[key];
	}

	const V& operator[](K key) const
	{
		return values[key];
	}

	template <typename K, typename V>
	class iterator
	{
	public:
		using underlying_type_k = typename std::underlying_type<K>::type;

		iterator(EnumMap<K, V>& p, underlying_type_k i)
			: parent(p), index(i) {}

		std::pair<K, V&> operator *()
		{
			return std::make_pair(first(), second());
		}

		K first() const
		{
			return static_cast<K>(index);
		}

		V& second()
		{
			if (index == K::LENGTH)
			{
				throw std::out_of_range("iterator out of range");
			}
			else
			{
				K key = first();
				return parent[key];
			}
		}

		iterator<K, V>& operator ++()
		{
			index++;
			return *this;
		}

		bool operator != (const iterator<K, V>& other) const
		{
			return index != other.index;
		}

	private:
		iterator<K, V>& operator = (const iterator<K, V>&) = delete;

		EnumMap<K, V>& parent;
		underlying_type_k index;
	};

	template <typename K, typename V>
	class const_iterator
	{
	public:
		using underlying_type_k = typename std::underlying_type<K>::type;

		const_iterator(const EnumMap<K, V>& p, underlying_type_k i)
			: parent(p), index(i) {}

		std::pair<K, const V&> operator *()
		{
			return std::make_pair(first(), second());
		}

		K first() const
		{
			return static_cast<K>(index);
		}

		const V& second()
		{
			if (index == K::LENGTH)
			{
				throw std::out_of_range("iterator out of range");
			}
			else
			{
				K key = first();
				return parent[key];
			}
		}

		const_iterator<K, V>& operator ++()
		{
			index++;
			return *this;
		}

		bool operator != (const const_iterator<K, V>& other) const
		{
			return index != other.index;
		}

	private:
		const_iterator<K, V>& operator = (const const_iterator<K, V>&) = delete;

		const EnumMap<K, V>& parent;
		underlying_type_k index;
	};

	iterator<K, V> begin()
	{
		return iterator<K, V>(*this, 0);
	}

	iterator<K, V> end()
	{
		return iterator<K, V>(*this, K::LENGTH);
	}

	const_iterator<K, V> begin() const
	{
		return const_iterator<K, V>(*this, 0);
	}

	const_iterator<K, V> end() const
	{
		return const_iterator<K, V>(*this, K::LENGTH);
	}

private:
	V values[K::LENGTH];
};