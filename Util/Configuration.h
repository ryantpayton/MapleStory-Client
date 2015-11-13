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
#include <stdint.h>
#include <string>
#include <map>

namespace Util
{
	using::std::string;
	using::std::map;

	// Class that manages the settings file. Contains settings such as screen mode or the default login.
	class Configuration
	{
	public:
		// Load the settings file at construction.
		Configuration();
		// Save the settings file at destruction.
		~Configuration();

		// Load all settings. If something is missing, set the default value. Can be used for reloading.
		void load();
		// Save the current settings. 
		void save() const;
		// Set the specified setting to the specified value.
		void setbool(string setting, bool value);
		// Set the specified setting to the specified value.
		void setint(string setting, uint32_t value);
		// Set the specified setting to the specified value.
		void setstring(string setting, string value);

		// Returns a bool based on the value of the setting specified.
		bool getbool(string setting) const;
		// Returns a byte based on the value of the setting specified. Returns 0 if an exception occurs.
		uint8_t getbyte(string setting) const;
		// Returns a short based on the value of the setting specified. Returns 0 if an exception occurs.
		uint16_t getshort(string setting) const;
		// Returns an int based on the value of the setting specified. Returns 0 if an exception occurs.
		uint32_t getinteger(string setting) const;
		// Returns a string based on the value of the setting specified.
		string getsetting(string setting) const;

	private:
		map<string, string> settings;
	};
}

