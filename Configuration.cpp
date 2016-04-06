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
#include "Configuration.h"

#include <fstream>

Configuration::Configuration()
{
	settings.emplace<ServerIP>();
	settings.emplace<Fullscreen>();
	settings.emplace<VSync>();
	settings.emplace<BGMVolume>();
	settings.emplace<SFXVolume>();
	settings.emplace<SaveLogin>();
	settings.emplace<DefaultAccount>();
	settings.emplace<DefaultWorld>();
	settings.emplace<DefaultChannel>();
	settings.emplace<DefaultCharacter>();
	settings.emplace<PosSTATS>();
	settings.emplace<PosEQINV>();
	settings.emplace<PosINV>();

	load();
}

Configuration::~Configuration()
{
	save();
}

void Configuration::load()
{
	using std::string;
	using std::unordered_map;
	unordered_map<string, string> rawsettings;

	using std::ifstream;
	ifstream file;
	file.open(FILENAME);
	if (file.is_open())
	{
		// Go through the file line for line.
		string line;
		while (getline(file, line))
		{
			// If the setting is not empty, load the value.
			size_t split = line.find('=');
			if (split != string::npos && split + 2 < line.size())
			{
				rawsettings.emplace(
					line.substr(0, split - 1),
					line.substr(split + 2)
					);
			}
		}
	}

	// Replace default values with loaded values.
	for (auto& setting : settings)
	{
		auto rsiter = rawsettings.find(setting.second->name);
		if (rsiter != rawsettings.end())
			setting.second->value = rsiter->second;
	}
}

void Configuration::save() const
{
	// Open the settings file.
	using std::ofstream;
	ofstream config;
	config.open(FILENAME);
	if (config.is_open())
	{
		// Save settings line by line.
		for (auto& setting : settings)
		{
			config << setting.second->tostring() << std::endl;
		}
	}
}

void Configuration::BoolEntry::save(bool b)
{
	value = b ? "true" : "false";
}

bool Configuration::BoolEntry::load() const
{
	return value == "true";
}

void Configuration::StringEntry::save(string str)
{
	value = str;
}

string Configuration::StringEntry::load() const
{
	return value;
}

void Configuration::PointEntry::save(Point<int16_t> vec)
{
	value = vec.tostring();
}

Point<int16_t> Configuration::PointEntry::load() const
{
	string xstr = value
		.substr(1, value
		.find(",") - 1);
	string ystr = value
		.substr(value
		.find(",") + 1, value.find(")") - value.find(",") - 1);

	auto x = StringConversion<int16_t>(xstr).orzero();
	auto y = StringConversion<int16_t>(ystr).orzero();
	return{ x, y };
}