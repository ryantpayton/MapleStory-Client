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

namespace Program
{
	namespace Configuration
	{
		const string FILENAME = "Settings";

		const size_t NUM_SETTINGS = 12;

		const string setnames[NUM_SETTINGS] =
		{
			"ServerIP", "Fullscreen", "BGMVolume", "SFXVolume", "SaveLogin", "Account", 
			"World", "Channel", "Character", "PosSTATS", "PosEQINV", "PosINV"
		};

		const string defaultvalues[NUM_SETTINGS] =
		{
			"127.0.0.1", "false", "50", "50", "false", "", 
			"0", "0", "0", "(100,150)", "(250,150)", "(300, 150)"
		};

		std::map<string, string> settings;

		void load()
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

		void save()
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

		void setbool(string name, bool b)
		{
			settings[name] = b ? "true" : "false";
		}

		void setint(string name, uint32_t i)
		{
			settings[name] = std::to_string(i);
		}

		void setstring(string name, string str)
		{
			settings[name] = str;
		}

		bool getbool(string name)
		{
			return settings.count(name) ? settings.at(name) == "true" : false;
		}

		uint8_t getbyte(string name)
		{
			return static_cast<uint8_t>(getinteger(name));
		}

		uint16_t getshort(string name)
		{
			return static_cast<uint16_t>(getinteger(name));
		}

		uint32_t getinteger(string name)
		{
			try
			{
				return std::stoi(settings.at(name));
			}
			catch (std::exception&)
			{
				return 0;
			}
		}

		string getsetting(string name)
		{
			return settings.count(name) ? settings.at(name) : "";
		}

		vector2d<int16_t> getvector2d(string name)
		{
			try
			{
				string xstr = settings.at(name)
					.substr(1, settings.at(name).find(",") - 1);
				string ystr = settings.at(name)
					.substr(settings.at(name).find(",") + 1, settings.at(name).find(")") - settings.at(name).find(",") - 1);

				return vector2d<int16_t>
					(static_cast<int16_t>(std::stoi(xstr)), static_cast<int16_t>(std::stoi(ystr))
					);
			}
			catch (std::exception&)
			{
				return vector2d<int16_t>();
			}
		}
	}
}