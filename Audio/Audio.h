//////////////////////////////////////////////////////////////////////////////
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
#include "Util\EnumMap.h"
#include "nlnx\node.hpp"
#include <unordered_map>
#include <cstdint>
#include <string>

namespace Audio
{
	using std::string;
	using std::unordered_map;
	using nl::node;

	class Sound
	{
	public:
		// Preloaded sounds.
		enum Name
		{
			// UI
			BUTTONCLICK,
			BUTTONOVER,

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
		Sound(node src);
		Sound();
		~Sound();

		void play() const;

		static bool init(uint8_t volume);
		static void close();
		static bool setsfxvolume(uint8_t volume);

	private:
		size_t id;

		static void playsound(size_t id);

		static size_t addsound(node src);
		static void addsound(Sound::Name name, node src);

		static unordered_map<size_t, uint64_t> samples;
		static EnumMap<Name, size_t> soundids;
	};

	class Music
	{
	public:
		Music(string path);

		void play() const;

		static bool init(uint8_t volume);
		static bool setbgmvolume(uint8_t volume);

	private:
		string path;
	};
}