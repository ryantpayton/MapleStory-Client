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
    class bitmap {
    public:
        bitmap() = default;
        bitmap(bitmap const &) = default;
        bitmap & operator=(bitmap const &) = default;
        //Comparison operators, useful for containers
        bool operator==(bitmap const &) const;
        bool operator<(bitmap const &) const;
        //Returns whether the bitmap is valid or merely null
        explicit operator bool() const;
        //This function decompresses the data on the fly
        //Do not free the pointer returned by this method
        //Every time this function is called
        //any previous pointers returned by this method become invalid
        void const * data() const;
        uint16_t width() const;
        uint16_t height() const;
        uint32_t length() const;
        //Returns a unique id, useful for keeping track of what bitmaps you loaded
        size_t id() const;
    private:
        bitmap(void const *, uint16_t, uint16_t);
        void const * m_data = nullptr;
        uint16_t m_width = 0;
        uint16_t m_height = 0;
        friend node;
    };
}
