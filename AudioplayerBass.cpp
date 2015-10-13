/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015 SYJourney                                               //
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
#include "AudioplayerBass.h"

namespace Program
{
	bool AudioplayerBass::init(HWND hwnd)
	{
		return BASS_Init(1, 44100, 0, hwnd, 0) == TRUE;
	}

	void AudioplayerBass::close()
	{
		if (bgm != 0)
		{
			BASS_ChannelStop(bgm);
			BASS_StreamFree(bgm);
		}
		for (map<size_t, HSAMPLE>::iterator snit = soundcache.begin(); snit != soundcache.end(); ++snit)
		{
			BASS_SampleStop(snit->second);
			BASS_SampleFree(snit->second);
		}
		BASS_Free();
	}

	void AudioplayerBass::playbgm(void* data, size_t length)
	{
		if (bgm != 0)
		{
			BASS_ChannelStop(bgm);
			BASS_StreamFree(bgm);
		}
		bgm = BASS_StreamCreateFile(true, data, 82, length, BASS_SAMPLE_FLOAT | BASS_SAMPLE_LOOP);
		BASS_ChannelPlay(bgm, true);
	}

	void AudioplayerBass::cachesound(void* data, size_t length, size_t id)
	{
		if (!soundcache.count(id))
		{
			soundcache[id] = BASS_SampleLoad(true, data, 82, (DWORD)length, 4, BASS_SAMPLE_OVER_POS);
		}
	}

	void AudioplayerBass::playsound(size_t id)
	{
		if (soundcache.count(id))
		{
			HCHANNEL channel = BASS_SampleGetChannel(soundcache[id], false);
			BASS_ChannelPlay(channel, true);
		}
	}

	void AudioplayerBass::setsfxvolume(char vol)
	{
		BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, vol * 100);
	}

	void AudioplayerBass::setbgmvolume(char vol)
	{
		BASS_SetConfig(BASS_CONFIG_GVOL_SAMPLE, vol * 100);
	}
}
