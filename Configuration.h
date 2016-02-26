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
#include "Util\Enum.h"
#include "Util\vector2d.h"
#include <cstdint>
#include <string>
#include <unordered_map>

using std::string;
using std::unordered_map;

class Settings
{
public:
	static const size_t NUM_SETTINGS = 12;
	enum Type
	{
		SERVER_IP, FULLSCREEN, BGM_VOLUME, SFX_VOLUME,
		SAVE_LOGIN, ACCOUNT, WORLD, CHANNEL, CHARACTER,
		POS_STATS, POS_EQINV, POS_INV
	};

	static EnumIterator<Type> getit()
	{
		return EnumIterator<Type>(SERVER_IP, POS_INV);
	}

	static string nameof(Type type)
	{
		static const string names[NUM_SETTINGS] =
		{
			"ServerIP", "Fullscreen", "BGMVolume", "SFXVolume",
			"SaveLogin", "Account", "World", "Channel", "Character",
			"PosSTATS", "PosEQINV", "PosINV"
		};
		return names[type];
	}

	static string defaultof(Type type)
	{
		const string defaults[NUM_SETTINGS] =
		{
			"127.0.0.1", "false", "50", "50",
			"false", "", "0", "0", "0",
			"(100,150)", "(250,150)", "(300, 150)"
		};
		return defaults[type];
	}

	static const string filename()
	{
		return "Settings";
	}
};

class Configuration : public Singleton<Configuration>
{
public:
	// Load all settings. If something is missing, set the default value. Can be used for reloading.
	void load();
	// Save the current settings. 
	void save() const;

	// Set the specified setting to the specified value.
	void setbool(Settings::Type setting, bool value);
	// Set the specified setting to the specified value.
	void setint(Settings::Type setting, uint32_t value);
	// Set the specified setting to the specified value.
	void setv2d(Settings::Type setting, vector2d<int16_t> value);
	// Set the specified setting to the specified value.
	void setstring(Settings::Type setting, string value);

	// Returns a bool based on the value of the setting specified.
	// Returns false if the setting is anything but "true".
	bool getbool(Settings::Type setting);
	// Returns a byte based on the value of the setting specified. 
	// Returns 0 if an exception occurs.
	uint8_t getbyte(Settings::Type setting);
	// Returns a short based on the value of the setting specified. 
	// Returns 0 if an exception occurs.
	uint16_t getshort(Settings::Type setting);
	// Returns an int based on the value of the setting specified. 
	// Returns 0 if an exception occurs.
	uint32_t getinteger(Settings::Type setting);
	// Returns a string based on the value of the setting specified.
	string getsetting(Settings::Type setting);
	// Returns a short vector2d based on the value of the setting specified.
	// Returns (0, 0) if an exception occurs.
	vector2d<int16_t> getvector2d(Settings::Type setting);

private:
	unordered_map<Settings::Type, string> settings;
};

