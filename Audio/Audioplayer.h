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
#include "nlnx\node.hpp"

#include <cstdint>
#include <string>
#include <unordered_map>

#define WIN32_LEAN_AND_MEAN
#include "bass.h"

namespace Audio
{
	using std::string;
	using std::unordered_map;
	using nl::node;

	class AudioPlayer : public Singleton<AudioPlayer>
	{
	public:
		// Preloaded sounds.
		static const size_t NUM_SOUNDS = 9;
		enum Sound
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
			LEVELUP
		};

		AudioPlayer();
		~AudioPlayer();

		bool init(uint8_t sfxvolume, uint8_t bgmvolume);
		void close();
		void setsfxvolume(uint8_t volume);
		void setbgmvolume(uint8_t volume);

		void playsound(Sound sound) const;
		void playsound(size_t soundid) const;
		void playbgm(string path);
		size_t addsound(node src);

	private:
		void playbgm(const void* data, size_t length);
		void addsound(Sound sound, node src);

		unordered_map<size_t, HSAMPLE> staticsounds;
		unordered_map<size_t, HSAMPLE> soundcache;

		HSTREAM bgm;
		string bgmpath;
	};
}