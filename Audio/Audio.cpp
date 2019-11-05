//////////////////////////////////////////////////////////////////////////////
// This file is part of the LibreMaple MMORPG client                        //
// Copyright © 2015-2016 Daniel Allendorf, 2018-2019 LibreMaple Team        //
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
// along with this program.  If not, see <https://www.gnu.org/licenses/>.   //
//////////////////////////////////////////////////////////////////////////////
#include "Audio.h"

#include "../Configuration.h"

#define WIN32_LEAN_AND_MEAN
#include "nlnx/audio.hpp"
#include "nlnx/nx.hpp"

#include <cstddef>

namespace ms
{
constexpr const char *Error::messages[];

Sound::Sound() noexcept : id{0}
{
}

Sound::Sound(Name name) noexcept : id{sound_ids[name]}
{
}

Sound::Sound(nl::node src) noexcept : id{add_sound(src)}
{
}

void Sound::play() const noexcept
{
    if (!initialized)
    {
        return;
    }

    if (auto sample_iter = samples.find(id); sample_iter != samples.end())
    {
        Mix_PlayChannel(-1, sample_iter->second, 0);
    }
}

Error Sound::init()
{
    // Initialize SDL.
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        return Error::AUDIO;
    }

    // Open 44.1kHz, signed 16-bit, system byteorder, stereo audio, using
    // 1024-byte chunks.
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
    {
        return Error::AUDIO;
    }

    // Dynamically load library for decoding ogg/vorbis audio.
    Mix_Init(MIX_INIT_OGG);

    // Allocate 16 channels for playing sound effects.
    Mix_AllocateChannels(16);

    nl::node uisrc = nl::nx::sound["UI.img"];

    add_sound(Sound::Name::BUTTONCLICK, uisrc["BtMouseClick"]);
    add_sound(Sound::Name::BUTTONOVER, uisrc["BtMouseOver"]);
    add_sound(Sound::Name::CHARSELECT, uisrc["CharSelect"]);
    add_sound(Sound::Name::DLGNOTICE, uisrc["DlgNotice"]);
    add_sound(Sound::Name::SELECTCHAR, uisrc["CharSelect"]);
    add_sound(Sound::Name::MENUDOWN, uisrc["MenuDown"]);
    add_sound(Sound::Name::MENUUP, uisrc["MenuUp"]);
    add_sound(Sound::Name::RACESELECT, uisrc["RaceSelect"]);
    add_sound(Sound::Name::SCROLLUP, uisrc["ScrollUp"]);
    add_sound(Sound::Name::TAB, uisrc["Tab"]);
    add_sound(Sound::Name::WORLDSELECT, uisrc["WorldSelect"]);
    add_sound(Sound::Name::DRAGSTART, uisrc["DragStart"]);
    add_sound(Sound::Name::DRAGEND, uisrc["DragEnd"]);
    add_sound(Sound::Name::WORLDMAPOPEN, uisrc["WorldmapOpen"]);
    add_sound(Sound::Name::WORLDMAPCLOSE, uisrc["WorldmapClose"]);

    nl::node gamesrc = nl::nx::sound["Game.img"];

    add_sound(Sound::Name::GAMESTART, gamesrc["GameIn"]);
    add_sound(Sound::Name::JUMP, gamesrc["Jump"]);
    add_sound(Sound::Name::DROP, gamesrc["DropItem"]);
    add_sound(Sound::Name::PICKUP, gamesrc["PickUpItem"]);
    add_sound(Sound::Name::PORTAL, gamesrc["Portal"]);
    add_sound(Sound::Name::LEVELUP, gamesrc["LevelUp"]);
    add_sound(Sound::Name::TOMBSTONE, gamesrc["Tombstone"]);

    std::uint8_t volume = Setting<SFXVolume>::get().load();
    if (!set_sfxvolume(volume))
        return Error::Code::AUDIO;
    
    return Error::Code::NONE;
}

void Sound::init_sfx() noexcept
{
    initialized = true;
}

Mix_Music *Music::stream;

void Sound::close() noexcept
{
    if (!initialized)
    {
        return;
    }

    if (Music::stream)
    {
        Mix_HaltMusic();
        Mix_FreeMusic(Music::stream);
    }

    for (auto [_, sample] : samples)
    {
        Mix_FreeChunk(sample);
    }
    samples.clear();

    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();

    initialized = false;
}

bool Sound::set_sfxvolume(std::uint8_t vol) noexcept
{
    if (!initialized)
    {
        return false;
    }

    Mix_Volume(-1, MIX_MAX_VOLUME * static_cast<int>(vol) / 100);
    return true;
}

std::size_t Sound::add_sound(nl::node src) noexcept
{
    if (!initialized)
    {
        return 0;
    }

    nl::audio ad = src;

    auto data = static_cast<const std::byte *>(ad.data());

    if (data)
    {
        std::size_t id = ad.id();

        samples[id] = Mix_LoadWAV_RW(
            SDL_RWFromConstMem(data + 82, ad.length() - 82), 0);

        return id;
    }
    else
    {
        return 0;
    }
}

void Sound::add_sound(Name name, nl::node src) noexcept
{
    if (!initialized)
    {
        return;
    }

    std::size_t id = add_sound(src);

    if (id)
    {
        sound_ids[name] = id;
    }
}

bool Sound::is_initialized() noexcept
{
    return initialized;
}

std::unordered_map<std::size_t, Mix_Chunk *> Sound::samples;
EnumMap<Sound::Name, std::size_t> Sound::sound_ids;
bool Sound::initialized = false;

Music::Music(std::string p)
{
    path = p;
}

// Error Music::play(const std::string& bgm_path)
Error Music::play()
{
    if (!initialized)
    {
        return Error::NONE;
    }

    static std::string bgmpath = "";
    if (path == bgmpath)
    {
        return Error::NONE;
    }

    nl::audio ad = nl::nx::sound.resolve(bgmpath);
    auto data = static_cast<const std::byte *>(ad.data());

    if (data)
    {
        if (stream)
        {
            Mix_HaltMusic();
            Mix_FreeMusic(stream);
        }

        stream = Mix_LoadMUSType_RW(
            SDL_RWFromConstMem(data + 82, ad.length() - 82), MUS_OGG, 0);
        if (Mix_PlayMusic(stream, -1) == -1)
        {
            return Error::AUDIO;
        }

        bgmpath = path;
    }

    return Error::NONE;
}

Error Music::init() noexcept
{
    stream = nullptr;
    initialized = true;
    std::uint8_t volume = Setting<BGMVolume>::get().load();
    if (!set_bgmvolume(volume))
        return Error::Code::AUDIO;

    return Error::Code::NONE;
}

bool Music::set_bgmvolume(std::uint8_t vol) noexcept
{
    if (!initialized)
    {
        return false;
    }

    Mix_VolumeMusic(MIX_MAX_VOLUME * static_cast<int>(vol) / 100);
    return true;
}

bool Music::is_initialized() noexcept
{
    return initialized;
}

bool Music::initialized = false;
} // namespace ms
