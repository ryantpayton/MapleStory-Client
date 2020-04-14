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

#include "bitmap.hpp"
#include <lz4.h>
#include <vector>

namespace nl {
    bitmap::bitmap(void const * d, uint16_t w, uint16_t h) :
        m_data(d), m_width(w), m_height(h) {}
    bool bitmap::operator<(bitmap const & o) const {
        return m_data < o.m_data;
    }
    bool bitmap::operator==(bitmap const & o) const {
        return m_data == o.m_data;
    }
    bitmap::operator bool() const {
        return m_data ? true : false;
    }
    std::vector<char> bitmap_buf;
    void const * bitmap::data() const {
        if (!m_data)
            return nullptr;
        auto const l = length();
        if (l + 0x20 > bitmap_buf.size())
            bitmap_buf.resize(l + 0x20);
        ::LZ4_decompress_fast(4 + reinterpret_cast<char const *>(m_data),
            bitmap_buf.data(), static_cast<int>(l));
        return bitmap_buf.data();
    }
    uint16_t bitmap::width() const {
        return m_width;
    }
    uint16_t bitmap::height() const {
        return m_height;
    }
    uint32_t bitmap::length() const {
        return 4u * m_width * m_height;
    }
    size_t bitmap::id() const {
        return reinterpret_cast<size_t>(m_data);
    }
}
