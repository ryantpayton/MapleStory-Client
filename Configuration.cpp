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
#include "Configuration.h"
#include <fstream>
#include <stdexcept>

namespace Util
{
	const string FILENAME = "Settings";

	const size_t NUM_SETTINGS = 9;

	const string setnames[NUM_SETTINGS] =
	{
		"Fullscreen", "DPF", "BGMVolume", "SFXVolume", "SaveLogin", "Account", "World", "Channel", "Character"
	};

	const string defaultvalues[NUM_SETTINGS] =
	{
		"false", "16", "50", "50", "false", "", "0", "1", "0"
	};

	Configuration::Configuration()
	{
		load();
	}

	Configuration::~Configuration()
	{
		save();
	}

	void Configuration::load()
	{
		// Open the settings file.
		using::std::ifstream;
		ifstream config;
		string line;
		config.open(FILENAME);
		if (config.is_open()) 
		{
			// Go through the file line for line. The same order is always assumed.
			size_t i = 0;
			while (getline(config, line))
			{
				string setname = setnames[i];
				size_t split = line.find('=');
				if (split != string::npos && split + 2 < line.size())
				{
					if (line.substr(0, split - 1) == setname)
					{
						// If everything seems correct, load the setting.
						string value = line.substr(split + 2);
						settings[setname] = value;
					}
				}
				i++;
			}
			config.close();
		}
		// Replace missing values with default ones.
		for (size_t i = 0; i < NUM_SETTINGS; i++)
		{
			if (!settings.count(setnames[i]))
			{
				settings[setnames[i]] = defaultvalues[i];
			}
		}
	}

	void Configuration::save() const
	{
		// Open the settings file.
		using::std::ofstream;
		ofstream config;
		config.open(FILENAME);
		if (config.is_open())
		{
			// Save settings line by line.
			for (size_t i = 0; i < NUM_SETTINGS; i++)
			{
				config << setnames[i] << " = " << settings.at(setnames[i]) << std::endl;
			}
			config.close();
		}
	}

	void Configuration::setbool(string name, bool b)
	{
		settings[name] = b ? "true" : "false";
	}

	void Configuration::setint(string name, uint32_t i)
	{
		settings[name] = std::to_string(i);
	}

	void Configuration::setstring(string name, string str)
	{
		settings[name] = str;
	}

	bool Configuration::getbool(string name) const
	{
		return settings.count(name) ? settings.at(name) == "true" : false;
	}

	uint8_t Configuration::getbyte(string name) const
	{
		return static_cast<uint8_t>(getinteger(name));
	}

	uint16_t Configuration::getshort(string name) const
	{
		return static_cast<uint16_t>(getinteger(name));
	}

	uint32_t Configuration::getinteger(string name) const
	{
		using::std::exception;
		try
		{
			return std::stoi(settings.at(name));
		}
		catch (exception&)
		{
			return 0;
		}
	}

	string Configuration::getsetting(string name) const
	{
		return settings.count(name) ? settings.at(name) : "";
	}
}