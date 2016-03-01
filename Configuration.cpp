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
#include "Configuration.h"
#include <fstream>
#include <stdexcept>

void Configuration::load()
{
	// Open the settings file.
	using std::ifstream;
	ifstream config;
	config.open(Settings::filename());
	if (config.is_open())
	{
		// Go through the file line for line. The same order is always assumed.
		for (auto it = Settings::getit(); it.hasnext(); it.increment())
		{
			string line;
			if (!getline(config, line))
				break;

			bool loaded = false;
			size_t split = line.find('=');
			if (split != string::npos && split + 2 < line.size())
			{
				Settings::Type setting = it.get();
				if (line.substr(0, split - 1) == Settings::nameof(setting))
				{
					// If everything seems correct, load the setting.
					string value = line.substr(split + 2);
					settings[setting] = value;
					loaded = true;
				}
			}

			if (!loaded)
				break;
		}
		config.close();
	}
	// Replace missing values with default ones.
	for (auto it = Settings::getit(); it.hasnext(); it.increment())
	{
		Settings::Type setting = it.get();
		if (!settings.count(setting))
		{
			settings[setting] = Settings::defaultof(setting);
		}
	}
}

void Configuration::save() const
{
	// Open the settings file.
	using std::ofstream;
	ofstream config;
	config.open(Settings::filename());
	if (config.is_open())
	{
		// Save settings line by line.
		for (auto it = Settings::getit(); it.hasnext(); it.increment())
		{
			Settings::Type setting = it.get();
			string name = Settings::nameof(setting);
			string value = settings.at(setting);
			config << name << " = " << value << std::endl;
		}
		config.close();
	}
}

void Configuration::setbool(Settings::Type setting, bool b)
{
	settings[setting] = b ? "true" : "false";
}

void Configuration::setint(Settings::Type setting, uint32_t i)
{
	settings[setting] = std::to_string(i);
}

void Configuration::setpoint(Settings::Type setting, Point<int16_t> vec)
{
	settings[setting] = vec.tostring();
}

void Configuration::setstring(Settings::Type setting, string str)
{
	settings[setting] = str;
}

bool Configuration::getbool(Settings::Type setting)
{
	return settings.count(setting) ? settings.at(setting) == "true" : false;
}

uint8_t Configuration::getbyte(Settings::Type setting)
{
	return static_cast<uint8_t>(getinteger(setting));
}

uint16_t Configuration::getshort(Settings::Type setting)
{
	return static_cast<uint16_t>(getinteger(setting));
}

uint32_t Configuration::getinteger(Settings::Type setting)
{
	try
	{
		return std::stoi(settings.at(setting));
	}
	catch (std::exception&)
	{
		return 0;
	}
}

string Configuration::getsetting(Settings::Type setting)
{
	return settings.count(setting) ? settings.at(setting) : "";
}

Point<int16_t> Configuration::getpoint(Settings::Type setting)
{
	try
	{
		string xstr = settings.at(setting)
			.substr(1, settings.at(setting)
			.find(",") - 1);
		string ystr = settings.at(setting)
			.substr(settings.at(setting)
			.find(",") + 1, settings.at(setting).find(")") - settings.at(setting).find(",") - 1);

		return Point<int16_t>
			(static_cast<int16_t>(std::stoi(xstr)), static_cast<int16_t>(std::stoi(ystr))
			);
	}
	catch (std::exception&)
	{
		return Point<int16_t>();
	}
}