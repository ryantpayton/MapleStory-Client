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
#include "Audioplayer.h"
#include "Program\Configuration.h"
#include "bass.h"
#include "nlnx\nx.hpp"
#include "nlnx\audio.hpp"
#include "nlnx\node.hpp"
#include <unordered_map>

namespace Audioplayer
{
	using std::unordered_map;

	HSTREAM bgm;
	unordered_map<size_t, HSAMPLE> soundcache;
	string bgmpath = "";

	bool init()
	{
		if (BASS_Init(1, 44100, 0, nullptr, 0) == TRUE)
		{
			setbgmvolume(Program::Configuration::getbyte("BGMVolume"));
			setsfxvolume(Program::Configuration::getbyte("SFXVolume"));
			return true;
		}
		else
		{
			return false;
		}
	}

	void close()
	{
		if (bgm)
		{
			BASS_ChannelStop(bgm);
			BASS_StreamFree(bgm);
		}
		for (auto& snit : soundcache)
		{
			BASS_SampleStop(snit.second);
			BASS_SampleFree(snit.second);
		}
		BASS_Free();
	}

	void setsfxvolume(uint8_t vol)
	{
		BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, vol * 100);
	}

	void setbgmvolume(uint8_t vol)
	{
		BASS_SetConfig(BASS_CONFIG_GVOL_SAMPLE, vol * 100);
	}

	void playbgm(const void* data, size_t length)
	{
		if (bgm)
		{
			BASS_ChannelStop(bgm);
			BASS_StreamFree(bgm);
		}
		bgm = BASS_StreamCreateFile(true, data, 82, length, BASS_SAMPLE_FLOAT | BASS_SAMPLE_LOOP);
		BASS_ChannelPlay(bgm, true);
	}

	void playbgm(string path)
	{
		if (path == bgmpath)
			return;

		nl::audio toplay = nl::nx::sound.resolve(path);
		if (toplay.data())
		{
			playbgm(reinterpret_cast<const void*>(toplay.data()), toplay.length());
			bgmpath = path;
		}
	}

	void playsound(size_t id)
	{
		if (soundcache.count(id))
		{
			HCHANNEL channel = BASS_SampleGetChannel(soundcache[id], false);
			BASS_ChannelPlay(channel, true);
		}
	}

	void cachesound(const void* data, size_t length, size_t id)
	{
		if (!soundcache.count(id))
		{
			soundcache[id] = BASS_SampleLoad(true, data, 82, (DWORD)length, 4, BASS_SAMPLE_OVER_POS);
		}
	}

	size_t addsound(string path)
	{
		using nl::audio;
		audio toplay = nl::nx::sound.resolve(path);

		if (toplay.data())
		{
			cachesound(reinterpret_cast<const void*>(toplay.data()), toplay.length(), toplay.id());
			return toplay.id();
		}
		else
		{
			return 0;
		}
	}
}
