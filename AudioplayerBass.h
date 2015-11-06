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
#include "Audioplayer.h"
#include "bass.h"
#include <map>

namespace Program
{
	using::std::map;

	class AudioplayerBass : public Audioplayer
	{
	public:
		AudioplayerBass();
		~AudioplayerBass();
		bool geterror() const;
		void close();
		void playsound(size_t);
		void playbgm(void*, size_t);
		void cachesound(void*, size_t, size_t);
		void setsfxvolume(uint8_t);
		void setbgmvolume(uint8_t);
	private:
		HSTREAM bgm;
		map<size_t, HSAMPLE> soundcache;
		bool error;
	};
}

