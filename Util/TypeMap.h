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
#include <typeindex>
#include <unordered_map>
#include <memory>

template <typename T>
// An unordered map which uses the type as key.
class TypeMap
{
public:
	using t_ptr = typename std::unique_ptr<T>;
	using pair = typename std::pair<std::type_index, t_ptr>;
	using cpair = typename std::pair<const std::type_index, t_ptr>;
	using underlying_map = typename std::unordered_map<std::type_index, t_ptr>;
	
	template <typename T, typename...Args>
	void emplace(Args&&... args)
	{
		container.emplace(typeid(T), std::make_unique<T>(args...));
	}

	template <typename T>
	void erase()
	{
		container.erase(typeid(T));
	}

	void clear()
	{
		container.clear();
	}

	template <typename T>
	T* get()
	{
		return reinterpret_cast<T*>(container[typeid(T)].get());
	}

	template <typename T>
	const T* get() const
	{
		return reinterpret_cast<T*>(container[typeid(T)].get());
	}

	typename underlying_map::iterator begin()
	{
		return container.begin();
	}

	typename underlying_map::iterator end()
	{
		return container.end();
	}

	typename underlying_map::const_iterator begin() const
	{
		return container.begin();
	}

	typename underlying_map::const_iterator end() const
	{
		return container.end();
	}

private:
	underlying_map container;
};