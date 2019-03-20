//////////////////////////////////////////////////////////////////////////////
// NoLifeNx - Part of the NoLifeStory project                               //
// Copyright © 2013 Peter Atashian                                          //
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
#include "nxfwd.hpp"
#include <cstdint>
#include <cstddef>

namespace nl {
    class audio {
    public:
        audio() = default;
        audio(audio const &) = default;
        audio & operator=(audio const &) = default;
        //Comparison operators, useful for containers
        bool operator==(audio const &) const;
        bool operator<(audio const &)const;
        //Returns whether the audio is valid or merely null
        explicit operator bool() const;
        //Does not do any sort of decompression
        //Do not free the pointer returned by this method
        //The pointer remains valid until the file this audio is part of is destroyed
        void const * data() const;
        uint32_t length() const;
        //Returns a unique id, useful for keeping track of what audio you loaded
        size_t id() const;
    private:
        audio(void const *, uint32_t);
        void const * m_data = nullptr;
        uint32_t m_length = 0;
        friend node;
    };
}
