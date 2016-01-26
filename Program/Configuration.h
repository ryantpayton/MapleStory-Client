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
#include "Util\Singleton.h"
#include "Util\vector2d.h"
#include <cstdint>
#include <string>
#include <unordered_map>

using std::string;
using std::unordered_map;

class Configuration : public Singleton<Configuration>
{
public:
	// Call save() when destroyed.
	~Configuration();

	// Load all settings. If something is missing, set the default value. Can be used for reloading.
	void load();
	// Save the current settings. 
	void save();

	// Set the specified setting to the specified value.
	void setbool(string setting, bool value);
	// Set the specified setting to the specified value.
	void setint(string setting, uint32_t value);
	// Set the specified setting to the specified value.
	void setstring(string setting, string value);

	// Returns a bool based on the value of the setting specified.
	// Returns false if the setting is anything but "true".
	bool getbool(string setting);
	// Returns a byte based on the value of the setting specified. 
	// Returns 0 if an exception occurs.
	uint8_t getbyte(string setting);
	// Returns a short based on the value of the setting specified. 
	// Returns 0 if an exception occurs.
	uint16_t getshort(string setting);
	// Returns an int based on the value of the setting specified. 
	// Returns 0 if an exception occurs.
	uint32_t getinteger(string setting);
	// Returns a string based on the value of the setting specified.
	string getsetting(string setting);
	// Returns a short vector2d based on the value of the setting specified.
	// Returns (0, 0) if an exception occurs.
	vector2d<int16_t> getvector2d(string setting);

private:
	unordered_map<string, string> settings;
};

