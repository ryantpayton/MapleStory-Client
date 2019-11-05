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
#pragma once
#include "../Error.h"
#include "../Template/EnumMap.h"
#include "nlnx/node.hpp"

#include <SDL.h>
#include <SDL_mixer.h>
#include <cstdint>
#include <string>
#include <unordered_map>

namespace ms
{
class Sound
{
public:
    //! Preloaded sounds.
    enum Name {
        // UI
        BUTTONCLICK,
        BUTTONOVER,
        CHARSELECT,
        DLGNOTICE,
        MENUDOWN,
        MENUUP,
        RACESELECT,
        SCROLLUP,
        TAB,
        WORLDSELECT,
        DRAGSTART,
        DRAGEND,
        WORLDMAPOPEN,
        WORLDMAPCLOSE,

        // Login
        SELECTCHAR,
        GAMESTART,

        // Game
        JUMP,
        DROP,
        PICKUP,
        PORTAL,
        LEVELUP,
        TOMBSTONE,
        LENGTH
    };

    Sound() noexcept;
    Sound(Name name) noexcept;
    Sound(nl::node src) noexcept;

    void play() const noexcept;

    [[nodiscard]] static Error init();
    static void init_sfx() noexcept;
    static void close() noexcept;
    static bool set_sfxvolume(std::uint8_t volume) noexcept;

    [[nodiscard]] static bool is_initialized() noexcept;

private:
    std::size_t id;

    static std::size_t add_sound(nl::node src) noexcept;
    static void add_sound(Sound::Name name, nl::node src) noexcept;

    static std::unordered_map<std::size_t, Mix_Chunk*> samples;
    static EnumMap<Name, std::size_t> sound_ids;
    static bool initialized;
};

class Music
{
public:
    Music(std::string path);

    // [[nodiscard]] Error play(const std::string& bgm_path);
    [[nodiscard]] Error play();

    static Error init() noexcept;
    static bool set_bgmvolume(std::uint8_t volume) noexcept;

    [[nodiscard]] static bool is_initialized() noexcept;

private:
    static Mix_Music* stream;
    static bool initialized;
    std::string path;

    friend Sound;
};
} // namespace jrc
