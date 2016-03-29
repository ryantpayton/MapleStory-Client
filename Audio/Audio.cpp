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
#include "Audio.h"

#define WIN32_LEAN_AND_MEAN
#include "bass.h"

#include "nlnx\nx.hpp"
#include "nlnx\audio.hpp"

namespace Audio
{
	using nl::audio;

	Sound::Sound(Sound::Name name)
	{
		if (soundids.count(name))
		{
			id = soundids.at(name);
		}
		else
		{
			id = 0;
		}
	}

	Sound::Sound(node src)
	{
		id = addsound(src);
	}

	Sound::Sound()
	{
		id = 0;
	}

	Sound::~Sound() {}

	void Sound::play() const
	{
		if (id > 0)
			playsound(id);
	}


	bool Sound::init(uint8_t volume)
	{
		if (!BASS_Init(1, 44100, 0, nullptr, 0))
			return false;

		node uisrc = nl::nx::sound["UI.img"];

		addsound(Sound::BUTTONCLICK, uisrc["BtMouseClick"]);
		addsound(Sound::BUTTONOVER, uisrc["BtMouseOver"]);
		addsound(Sound::SELECTCHAR, uisrc["CharSelect"]);

		node gamesrc = nl::nx::sound["Game.img"];

		addsound(Sound::GAMESTART, gamesrc["GameIn"]);
		addsound(Sound::JUMP, gamesrc["Jump"]);
		addsound(Sound::DROP, gamesrc["DropItem"]);
		addsound(Sound::PICKUP, gamesrc["PickUpItem"]);
		addsound(Sound::PORTAL, gamesrc["Portal"]);
		addsound(Sound::LEVELUP, gamesrc["LevelUp"]);

		return setsfxvolume(volume);
	}

	void Sound::close()
	{
		BASS_Free();
	}

	bool Sound::setsfxvolume(uint8_t vol)
	{
		return BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, vol * 100) == TRUE;
	}

	void Sound::playsound(size_t id)
	{
		if (!samples.count(id))
			return;

		HCHANNEL channel = BASS_SampleGetChannel((HSAMPLE)samples.at(id), false);
		BASS_ChannelPlay(channel, true);
	}

	size_t Sound::addsound(node src)
	{
		audio ad = src;

		auto data = reinterpret_cast<const void*>(ad.data());
		if (data)
		{
			size_t id = ad.id();

			samples[id] = BASS_SampleLoad(true, data, 82, (DWORD)ad.length(), 4, BASS_SAMPLE_OVER_POS);
			return id;
		}
		else
		{
			return 0;
		}
	}

	void Sound::addsound(Sound::Name name, node src)
	{
		size_t id = addsound(src);
		if (id)
		{
			soundids[name] = id;
		}
	}
	unordered_map<size_t, uint64_t> Sound::samples;
	unordered_map<Sound::Name, size_t> Sound::soundids;


	Music::Music(string p)
	{
		path = p;
	}

	void Music::play() const
	{
		static HSTREAM stream = 0;
		static string bgmpath = "";

		if (path == bgmpath)
			return;

		audio ad = nl::nx::sound.resolve(path);
		auto data = reinterpret_cast<const void*>(ad.data());
		if (data)
		{
			if (stream)
			{
				BASS_ChannelStop(stream);
				BASS_StreamFree(stream);
			}
			stream = BASS_StreamCreateFile(true, data, 82, ad.length(), BASS_SAMPLE_FLOAT | BASS_SAMPLE_LOOP);
			BASS_ChannelPlay(stream, true);

			bgmpath = path;
		}
	}


	bool Music::init(uint8_t volume)
	{
		return setbgmvolume(volume);
	}

	bool Music::setbgmvolume(uint8_t vol)
	{
		return BASS_SetConfig(BASS_CONFIG_GVOL_SAMPLE, vol * 100) == TRUE;
	}
}