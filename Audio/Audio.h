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

#include "../Error.h"
#include "../MapleStory.h"

#include "../Template/EnumMap.h"

#include <unordered_map>

#ifdef USE_NX
#include <nlnx/node.hpp>
#else
#include "../Util/WzFiles.h"
#endif

namespace ms
{
	class Sound
	{
	public:
		// Preloaded sounds
		enum Name
		{
			/// UI
			BUTTONCLICK,
			BUTTONOVER,
			CHARSELECT,
			DLGNOTICE,
			MENUDOWN,
			MENUUP,
			RACESELECT,
			SCROLLUP,
			SELECTMAP,
			TAB,
			WORLDSELECT,
			DRAGSTART,
			DRAGEND,
			WORLDMAPOPEN,
			WORLDMAPCLOSE,

			/// Login
			GAMESTART,

			/// Game
			JUMP,
			DROP,
			PICKUP,
			PORTAL,
			LEVELUP,
			TOMBSTONE,
			LENGTH
		};

		Sound(Name name);
		Sound(int32_t itemid);
		Sound(nl::node src);
		Sound();

		void play() const;

		static Error init();
		static void close();
		static bool set_sfxvolume(uint8_t volume);

	private:
		size_t id;

		static void play(size_t id);

		static size_t add_sound(nl::node src);
		static void add_sound(Name name, nl::node src);
		static void add_sound(std::string itemid, nl::node src);

		static std::string format_id(int32_t itemid);

		static std::unordered_map<size_t, uint64_t> samples;
		static EnumMap<Name, size_t> soundids;
		static std::unordered_map<std::string, size_t> itemids;
	};

	class Music
	{
	public:
		Music(std::string path);

		void play() const;
		void play_once() const;

		static Error init();
		static bool set_bgmvolume(uint8_t volume);

	private:
		std::string path;
	};
}