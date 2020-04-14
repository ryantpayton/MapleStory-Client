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

#include "../MapleStory.h"

#ifndef USE_NX
#include "../Error.h"

#include <array>
#include <string>

namespace ms
{
	namespace WzFiles
	{
		// Number of needed files
		constexpr uint8_t NUM_FILES = 18;

		// Names of the needed game files
		constexpr std::array<const char*, NUM_FILES> filenames =
		{
			"Character.wz",
			"Effect.wz",
			"Etc.wz",
			"Item.wz",
			"Map.wz",
			"MapPretty.wz",	// From v167,
			"MapLatest.wz",	// From latest,
			"Map001.wz",	// From latest,
			"Mob.wz",
			"Morph.wz",
			"Npc.wz",
			"Quest.wz",
			"Reactor.wz",
			"Skill.wz",
			"Sound.wz",
			"String.wz",
			"TamingMob.wz",
			"UI.wz"			// From latest
		};

		// Initialize WZ
		// When successful test if the UI file is the correct version
		Error init();
	};
}

// TODO: Remove below once WZ is implemented and has these functions
namespace nl
{
	class bitmap
	{
	public:
		bitmap() = default;
		bitmap(bitmap const&) = default;

		void const* data() const { return 0; }
		uint16_t width() const { return 0; }
		uint16_t height() const { return 0; }
		size_t id() const { return 0; }
	};

	class audio
	{
	public:
		audio() = default;
		audio(audio const&) = default;

		void const* data() const { return 0; }
		uint32_t length() const { return 0; }
		size_t id() const { return 0; }
	};

	struct _file_data;
	typedef std::pair<int32_t, int32_t> vector2i;
	class node
	{
	public:
		using difference_type = ptrdiff_t;
		using value_type = node;
		using pointer = node;
		using reference = node&;
		using iterator_category = std::random_access_iterator_tag;
		struct data;
		enum class type : uint16_t {
			none = 0,
			integer = 1,
			real = 2,
			string = 3,
			vector = 4,
			bitmap = 5,
			audio = 6,
		};
		node() = default;
		node(node const&) {}
		node& operator=(node const&) = default;
		node begin() const { return node(); }
		node end() const { return node(); }
		node operator*() const { return node(); }
		node& operator++() { return *new node(); }
		node operator++(int) { return node(); }
		bool operator==(node const&) const { return false; }
		bool operator!=(node const&) const { return false; }
		bool operator<(node const&) const { return false; }
		explicit operator bool() const { return false; }
		node operator[](unsigned int) const { return node(); }
		node operator[](signed int) const { return node(); }
		node operator[](unsigned long) const { return node(); }
		node operator[](signed long) const { return node(); }
		node operator[](unsigned long long) const { return node(); }
		node operator[](signed long long) const { return node(); }
		node operator[](std::string const&) const { return node(); }
		node operator[](char const*) const { return node(); }
		node operator[](node const&) const { return node(); }
		operator unsigned char() const { return '0'; }
		operator signed char() const { return '0'; }
		operator unsigned short() const { return 0; }
		operator signed short() const { return 0; }
		operator unsigned int() const { return 0; }
		operator signed int() const { return 0; }
		operator unsigned long() const { return 0; }
		operator signed long() const { return 0; }
		operator unsigned long long() const { return 0; }
		operator signed long long() const { return 0; }
		operator float() const { return 0; }
		operator double() const { return 0; }
		operator long double() const { return 0; }
		operator std::string() const { return ""; }
		operator vector2i() const { return vector2i(); }
		operator bitmap() const { return bitmap(); }
		operator audio() const { return audio(); }
		int64_t get_integer(int64_t = 0) const { return 0; }
		double get_real(double = 0) const { return 0; }
		std::string get_string(std::string = "") const { return ""; }
		vector2i get_vector(vector2i = { 0, 0 }) const { return vector2i(); }
		bitmap get_bitmap() const { return bitmap(); }
		audio get_audio() const { return audio(); }
		bool get_bool() const { return false; }
		bool get_bool(bool) const { return false; }
		int32_t x() const { return 0; }
		int32_t y() const { return 0; }
		std::string name() const { return ""; }
		size_t size() const { return 0; }
		type data_type() const { return type::none; }
		node root() const { return node(); }
		node resolve(std::string) const { return node(); }

	private:
		node(data const*, _file_data const*) {}
		node get_child(char const*, uint16_t) const {}
		int64_t to_integer() const {}
		double to_real() const {}
		std::string to_string() const {}
		vector2i to_vector() const {}
		bitmap to_bitmap() const {}
		audio to_audio() const {}
		data const* m_data = nullptr;
		_file_data const* m_file = nullptr;
		//friend file;
	};

	inline std::string operator+(std::string, node) { return ""; }
	inline std::string operator+(char const*, node) { return ""; }
	inline std::string operator+(node, std::string) { return ""; }
	inline std::string operator+(node, char const*) { return ""; }

	namespace nx
	{
		static nl::node base, character, effect, etc, item, map, mapPretty, mapLatest, map001, mob, morph, npc, quest, reactor, skill, sound, string, tamingmob, ui;
	}
}
#endif