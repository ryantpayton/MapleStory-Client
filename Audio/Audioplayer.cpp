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
#include "AudioPlayer.h"
#include "nlnx\nx.hpp"
#include "nlnx\audio.hpp"

namespace Audio
{
	using nl::audio;

	AudioPlayer::AudioPlayer()
	{
		bgmpath = "";
	}

	AudioPlayer::~AudioPlayer()
	{
		close();
	}

	bool AudioPlayer::init(uint8_t sfxvolume, uint8_t bgmvolume)
	{
		BOOL result = BASS_Init(1, 44100, 0, nullptr, 0);
		if (result == TRUE)
		{
			setsfxvolume(sfxvolume);
			setbgmvolume(bgmvolume);

			node uisrc = nl::nx::sound["UI.img"];
			addsound(BUTTONCLICK, uisrc["BtMouseClick"]);
			addsound(BUTTONOVER, uisrc["BtMouseOver"]);
			addsound(SELECTCHAR, uisrc["CharSelect"]);

			node gamesrc = nl::nx::sound["Game.img"];
			addsound(GAMESTART, gamesrc["GameIn"]);
			addsound(JUMP, gamesrc["Jump"]);
			addsound(DROP, gamesrc["DropItem"]);
			addsound(PICKUP, gamesrc["PickUpItem"]);
			addsound(PORTAL, gamesrc["Portal"]);
			addsound(LEVELUP, gamesrc["LevelUp"]);
		}
		return result == TRUE;
	}

	void AudioPlayer::close()
	{
		BASS_Free();
	}

	void AudioPlayer::setsfxvolume(uint8_t vol)
	{
		BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, vol * 100);
	}

	void AudioPlayer::setbgmvolume(uint8_t vol)
	{
		BASS_SetConfig(BASS_CONFIG_GVOL_SAMPLE, vol * 100);
	}

	void AudioPlayer::playbgm(const void* data, size_t length)
	{
		if (bgm)
		{
			BASS_ChannelStop(bgm);
			BASS_StreamFree(bgm);
		}
		bgm = BASS_StreamCreateFile(true, data, 82, length, BASS_SAMPLE_FLOAT | BASS_SAMPLE_LOOP);
		BASS_ChannelPlay(bgm, true);
	}

	void AudioPlayer::playbgm(string path)
	{
		if (path == bgmpath)
			return;

		audio toplay = nl::nx::sound.resolve(path);
		if (toplay.data())
		{
			playbgm(reinterpret_cast<const void*>(toplay.data()), toplay.length());
			bgmpath = path;
		}
	}

	void AudioPlayer::playsound(Sound sound) const
	{
		if (staticsounds.count(sound))
		{
			HCHANNEL channel = BASS_SampleGetChannel(staticsounds.at(sound), false);
			BASS_ChannelPlay(channel, true);
		}
	}

	void AudioPlayer::playsound(size_t id) const
	{
		if (soundcache.count(id))
		{
			HCHANNEL channel = BASS_SampleGetChannel(soundcache.at(id), false);
			BASS_ChannelPlay(channel, true);
		}
	}

	size_t AudioPlayer::addsound(node src)
	{
		audio ad = src;

		const void* data = reinterpret_cast<const void*>(ad.data());
		if (data == nullptr)
			return 0;

		size_t id = ad.id();
		if (!soundcache.count(id))
			soundcache[id] = BASS_SampleLoad(true, data, 82, (DWORD)ad.length(), 4, BASS_SAMPLE_OVER_POS);

		return id;
	}

	void AudioPlayer::addsound(Sound sound, node src)
	{
		audio ad = src;
		const void* data = reinterpret_cast<const void*>(ad.data());
		if (data)
		{
			staticsounds[sound] = BASS_SampleLoad(true, data, 82, (DWORD)ad.length(), 4, BASS_SAMPLE_OVER_POS);
		}
	}
}