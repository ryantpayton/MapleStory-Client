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

#include "audio.hpp"

namespace nl {
    audio::audio(void const * d, uint32_t l) :
        m_data(d), m_length(l) {}
    bool audio::operator<(audio const & o) const {
        return m_data < o.m_data;
    }
    bool audio::operator==(audio const & o) const {
        return m_data == o.m_data;
    }
    audio::operator bool() const {
        return m_data ? true : false;
    }
    void const * audio::data() const {
        return m_data;
    }
    uint32_t audio::length() const {
        return m_length;
    }
    size_t audio::id() const {
        return reinterpret_cast<size_t>(m_data);
    }
}
