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
#include "Util\Misc.h"
#include "Util\Point.h"
#include "Util\Singleton.h"
#include "Util\TypeMap.h"
#include <cstdint>
#include <string>
#include <type_traits>
#include <functional>

// Manages the 'Settings' file which contains configurations set by user behaviour.
class Configuration : public Singleton<Configuration>
{
public:
	// Add the settings which will be used and load them.
	Configuration();
	// Save.
	~Configuration();

	// Load all settings. If something is missing, set the default value. Can be used for reloading.
	void load();
	// Save the current settings. 
	void save() const;

	// Base class for an entry in the settings file.
	class Entry
	{
	protected:
		using string = std::string;

		Entry(const char* n, const char* v)
			: name(n), value(v) {}

		string name;
		string value;

	private:
		friend class Configuration;

		string tostring() const
		{
			return name + " = " + value;
		}
	};

	// Setting which converts to a bool.
	class BoolEntry : public Entry
	{
	public:
		void save(bool b);
		bool load() const;

	protected:
		// Reminder to replace with "using Entry::Entry;".
		// Constructor inheritance is still bugged in MSVC.
		BoolEntry(const char* n, const char* v)
			: Entry(n, v) {}
	};

	// Setting which uses the raw string.
	class StringEntry : public Entry
	{
	public:
		void save(string str);
		string load() const;

	protected:
		StringEntry(const char* n, const char* v)
			: Entry(n, v) {}
	};

	// Setting which converts to a Point<int16_t>.
	class PointEntry : public Entry
	{
	public:
		void save(Point<int16_t> p);
		Point<int16_t> load() const;

	protected:
		PointEntry(const char* n, const char* v)
			: Entry(n, v) {}
	};

	// Setting which converts to an integer type.
	template <class T>
	class IntegerEntry : public Entry
	{
	public:
		void save(T num)
		{
			value = std::to_string(num);
		}

		T load() const
		{
			return StringConversion<T>(value).orzero();
		}

	protected:
		IntegerEntry(const char* n, const char* v) : Entry(n, v) {}
	};

	// Setting which converts to a byte.
	class ByteEntry : public IntegerEntry<uint8_t>
	{
	protected:
		ByteEntry(const char* n, const char* v) : IntegerEntry(n, v) {}
	};

	// Setting which converts to a short.
	class ShortEntry : public IntegerEntry<uint16_t>
	{
	protected:
		ShortEntry(const char* n, const char* v) : IntegerEntry(n, v) {}
	};

	// Setting which converts to an int.
	class IntEntry : public IntegerEntry<uint32_t>
	{
	protected:
		IntEntry(const char* n, const char* v) : IntegerEntry(n, v) {}
	};

	// Setting which converts to a long.
	class LongEntry : public IntegerEntry<uint64_t>
	{
	protected:
		LongEntry(const char* n, const char* v) : IntegerEntry(n, v) {}
	};

private:
	template <typename T>
	friend struct Setting;

	const char* FILENAME = "Settings";
	TypeMap<Entry> settings;
};

// IP Adress which the client will connect to.
struct ServerIP : public Configuration::StringEntry
{
	ServerIP() : StringEntry("ServerIP", "127.0.0.1") {}
};

// Wether to start in fullscreen mode.
struct Fullscreen : public Configuration::BoolEntry
{
	Fullscreen() : BoolEntry("Fullscreen", "false") {}
};

// Wether to use vsync.
struct VSync : public Configuration::BoolEntry
{
	VSync() : BoolEntry("VSync", "true") {}
};

// Music Volume, a number from 0 to 100.
struct BGMVolume : public Configuration::ByteEntry
{
	BGMVolume() : ByteEntry("BGMVolume", "50") {}
};

// Sound Volume, a number from 0 to 100.
struct SFXVolume : public Configuration::ByteEntry
{
	SFXVolume() : ByteEntry("SFXVolume", "50") {}
};

// Wether to save the last used account name.
struct SaveLogin : public Configuration::BoolEntry
{
	SaveLogin() : BoolEntry("SaveLogin", "false") {}
};

// The last used account name.
struct DefaultAccount : public Configuration::StringEntry
{
	DefaultAccount() : StringEntry("Account", "") {}
};

// The last used world.
struct DefaultWorld : public Configuration::ByteEntry
{
	DefaultWorld() : ByteEntry("World", "0") {}
};

// The last used channel.
struct DefaultChannel : public Configuration::ByteEntry
{
	DefaultChannel() : ByteEntry("Channel", "0") {}
};

// The last used character.
struct DefaultCharacter : public Configuration::ByteEntry
{
	DefaultCharacter() : ByteEntry("Character", "0") {}
};

// The default position of the character stats inventory.
struct PosSTATS : public Configuration::PointEntry
{
	PosSTATS() : PointEntry("PosSTATS", "(100,150)") {}
};

// The default position of the equip inventory.
struct PosEQINV : public Configuration::PointEntry
{
	PosEQINV() : PointEntry("PosEQINV", "(250,150)") {}
};

// The default position of the item inventory.
struct PosINV : public Configuration::PointEntry
{
	PosINV() : PointEntry("PosINV", "(300,150)") {}
};

template <typename T>
// Can be used to access settings.
struct Setting
{
	// Access a setting.
	static T& get()
	{
		static_assert(std::is_base_of<Configuration::Entry, T>::value,
			"template parameter T for Setting must inherit from Configuration::Entry.");

		auto* entry = Configuration::get().settings.get<T>();
		if (entry)
		{
			return *entry;
		}
		else
		{
			static T defaultentry;
			return defaultentry;
		}
	}
};