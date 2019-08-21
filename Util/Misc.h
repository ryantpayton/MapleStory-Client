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

#include "../Console.h"

#include <cstdint>
#include <string>

namespace ms
{
	namespace string_conversion
	{
		template<typename T>
		inline T or_default(const std::string& str, T def)
		{
			try
			{
				int32_t intval = std::stoi(str);
				return static_cast<T>(intval);
			}
			catch (const std::exception& ex)
			{
				Console::get().print(__func__, ex);
				return def;
			}
		}

		template<typename T>
		inline T or_zero(const std::string& str)
		{
			return or_default<T>(str, T(0));
		}
	};

	namespace string_format
	{
		// Format a number string so that each 3 decimal points
		// are seperated by a ',' character.
		void split_number(std::string& input);

		// Prefix an id with zeroes so that it has the minimum specified length.
		std::string extend_id(int32_t id, size_t length);
	};

	namespace bytecode
	{
		// Check if a bit mask contains the specified value.
		bool compare(int32_t mask, int32_t value);
	}
}