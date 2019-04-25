//////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015-2016 Daniel Allendorf                                   //
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
#include "../Error.h"
#include "../Template/EnumMap.h"

#include "nlnx/node.hpp"

#include <unordered_map>
#include <cstdint>
#include <string>

namespace jrc
{
	class Sound
	{
	public:
		// Preloaded sounds.
		enum Name
		{
			// UI
			BUTTONCLICK,
			BUTTONOVER,
			MENUDOWN,
			MENUUP,

			// Login
			SELECTCHAR,
			GAMESTART,

			// Game
			JUMP,
			DROP,
			PICKUP,
			PORTAL,
			LEVELUP,
			LENGTH
		};

		Sound(Name name);
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
		static void add_sound(Sound::Name name, nl::node src);

		static std::unordered_map<size_t, uint64_t> samples;
		static EnumMap<Name, size_t> soundids;
	};

	class Music
	{
	public:
		Music(std::string path);

		void play() const;

		static Error init();
		static bool set_bgmvolume(uint8_t volume);

	private:
		std::string path;
	};
}