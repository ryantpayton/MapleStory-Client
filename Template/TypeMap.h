//////////////////////////////////////////////////////////////////////////////////
//	This file is part of the continued Journey MMORPG client					//
//	Copyright (C) 2015-2019  Daniel Allendorf, Ryan Payton						//
//																				//
//	This program is free software: you can redistribute it and/or modify		//
//	it under the terms of the GNU Affero General Public License as published by	//
//	the Free Software Foundation, either version 3 of the License, or			//
//	(at your option) any later version.											//
//																				//
//	This program is distributed in the hope that it will be useful,				//
//	but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				//
//	GNU Affero General Public License for more details.							//
//																				//
//	You should have received a copy of the GNU Affero General Public License	//
//	along with this program.  If not, see <https://www.gnu.org/licenses/>.		//
//////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <typeindex>
#include <unordered_map>
#include <memory>

namespace ms
{
	template <typename T>
	// An unordered map which uses the type as key.
	class TypeMap
	{
	public:
		using t_ptr = typename std::unique_ptr<T>;
		using pair = typename std::pair<std::type_index, t_ptr>;
		using cpair = typename std::pair<const std::type_index, t_ptr>;
		using underlying_map = typename std::unordered_map<std::type_index, t_ptr>;

		template <typename D, typename...Args>
		D* emplace(Args&& ... args)
		{
			auto* ptr = container.emplace(typeid(D), std::make_unique<D>(args...)).first->second.get();
			return static_cast<D*>(ptr);
		}

		template <typename D>
		void erase()
		{
			container.erase(typeid(D));
		}

		void clear()
		{
			container.clear();
		}

		template <typename D>
		D* get()
		{
			return static_cast<D*>(container[typeid(D)].get());
		}

		template <typename D>
		const D* get() const
		{
			return static_cast<D*>(container[typeid(D)].get());
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
}