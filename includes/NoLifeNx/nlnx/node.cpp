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

#include "node_impl.hpp"
#include "file_impl.hpp"
#include "bitmap.hpp"
#include "audio.hpp"
#include <cstring>
#include <stdexcept>
#include <vector>
#include <sstream>

namespace nl {
    node::node(node const & o) :
        m_data(o.m_data), m_file(o.m_file) {}
    node::node(data const * d, file::data const * f) :
        m_data(d), m_file(f) {}
    node node::begin() const {
        if (!m_data)
            return {nullptr, m_file};
        return {m_file->node_table + m_data->children, m_file};
    }
    node node::end() const {
        if (!m_data)
            return {nullptr, m_file};
        return {m_file->node_table + m_data->children + m_data->num, m_file};
    }
    node node::operator*() const {
        return *this;
    }
    node & node::operator++() {
        ++m_data;
        return *this;
    }
    node node::operator++(int) {
        return {m_data++, m_file};
    }
    bool node::operator==(node const & o) const {
        return m_data == o.m_data;
    }
    bool node::operator!=(node const & o) const {
        return m_data != o.m_data;
    }
    bool node::operator<(node const & o) const {
        return m_data < o.m_data;
    }
    std::string operator+(std::string s, node n) {
        return s + n.get_string();
    }
    std::string operator+(char const * s, node n) {
        return s + n.get_string();
    }
    std::string operator+(node n, std::string s) {
        return n.get_string() + s;
    }
    std::string operator+(node n, char const * s) {
        return n.get_string() + s;
    }
    node node::operator[](unsigned int n) const {
        return operator[](std::to_string(n));
    }
    node node::operator[](signed int n) const {
        return operator[](std::to_string(n));
    }
    node node::operator[](unsigned long n) const {
        return operator[](std::to_string(n));
    }
    node node::operator[](signed long n) const {
        return operator[](std::to_string(n));
    }
    node node::operator[](unsigned long long n) const {
        return operator[](std::to_string(n));
    }
    node node::operator[](signed long long n) const {
        return operator[](std::to_string(n));
    }
    node node::operator[](std::string const & o) const {
        return get_child(o.c_str(), static_cast<uint16_t>(o.length()));
    }
    node node::operator[](char const * o) const {
        return get_child(o, static_cast<uint16_t>(std::strlen(o)));
    }
    node node::operator[](node const & o) const {
        return operator[](o.get_string());
    }
    node::operator unsigned char() const {
        return static_cast<unsigned char>(get_integer());
    }
    node::operator signed char() const {
        return static_cast<signed char>(get_integer());
    }
    node::operator unsigned short() const {
        return static_cast<unsigned short>(get_integer());
    }
    node::operator signed short() const {
        return static_cast<signed short>(get_integer());
    }
    node::operator unsigned int() const {
        return static_cast<unsigned int>(get_integer());
    }
    node::operator signed int() const {
        return static_cast<signed int>(get_integer());
    }
    node::operator unsigned long() const {
        return static_cast<unsigned long>(get_integer());
    }
    node::operator signed long() const {
        return static_cast<signed long>(get_integer());
    }
    node::operator unsigned long long() const {
        return static_cast<unsigned long long>(get_integer());
    }
    node::operator signed long long() const {
        return static_cast<signed long long>(get_integer());
    }
    node::operator float() const {
        return static_cast<float>(get_real());
    }
    node::operator double() const {
        return static_cast<double>(get_real());
    }
    node::operator long double() const {
        return static_cast<long double>(get_real());
    }
    node::operator std::string() const {
        return get_string();
    }
    node::operator vector2i() const {
        return get_vector();
    }
    node::operator bitmap() const {
        return get_bitmap();
    }
    node::operator audio() const {
        return get_audio();
    }
    node::operator bool() const {
        return m_data ? true : false;
    }
    int64_t node::get_integer(int64_t def) const {
        if (!m_data)
            return def;
        switch (m_data->type) {
        case type::none:
        case type::vector:
        case type::bitmap:
        case type::audio:
            return def;
        case type::integer:
            return to_integer();
        case type::real:
            return static_cast<int64_t>(to_real());
        case type::string:
            return std::stoll(to_string());
        default:
            throw std::runtime_error("Unknown node type");
        }
    }
    double node::get_real(double def) const {
        if (!m_data)
            return def;
        switch (m_data->type) {
        case type::none:
        case type::vector:
        case type::bitmap:
        case type::audio:
            return def;
        case type::integer:
            return static_cast<double>(to_integer());
        case type::real:
            return to_real();
        case type::string:
            return std::stod(to_string());
        default:
            throw std::runtime_error("Unknown node type");
        }
    }
    std::string node::get_string(std::string def) const {
        if (!m_data)
            return def;
        switch (m_data->type) {
        case type::none:
        case type::vector:
        case type::bitmap:
        case type::audio:
            return def;
        case type::integer:
            return std::to_string(to_integer());
        case type::real:
            return std::to_string(to_real());
        case type::string:
            return to_string();
        default:
            throw std::runtime_error("Unknown node type");
        }
    }
    vector2i node::get_vector(vector2i def) const {
        if (m_data && m_data->type == type::vector)
            return to_vector();
        return def;
    }
    bitmap node::get_bitmap() const {
        if (m_data && m_data->type == type::bitmap && m_file->header->bitmap_count)
            return to_bitmap();
        return {nullptr, 0, 0};
    }
    audio node::get_audio() const {
        if (m_data && m_data->type == type::audio && m_file->header->audio_count)
            return to_audio();
        return {nullptr, 0};
    }
    bool node::get_bool() const {
        return m_data && m_data->type == type::integer && to_integer() ? true : false;
    }
    bool node::get_bool(bool def) const {
        return m_data && m_data->type == type::integer ? to_integer() ? true : false : def;
    }
    int32_t node::x() const {
        return m_data && m_data->type == type::vector ? m_data->vector[0] : 0;
    }
    int32_t node::y() const {
        return m_data && m_data->type == type::vector ? m_data->vector[1] : 0;
    }
    std::string node::name() const {
        if (!m_data)
            return {};
        auto const s = reinterpret_cast<char const *>(m_file->base)
            + m_file->string_table[m_data->name];
        return {s + 2, *reinterpret_cast<uint16_t const *>(s)};
    }
    size_t node::size() const {
        return m_data ? m_data->num : 0u;
    }
    node::type node::data_type() const {
        return m_data ? m_data->type : type::none;
    }
    node node::get_child(char const * const o, uint16_t const l) const {
        if (!m_data)
            return {nullptr, m_file};
        auto p = m_file->node_table + m_data->children;
        auto n = m_data->num;
        auto const b = reinterpret_cast<const char *>(m_file->base);
        auto const t = m_file->string_table;
        for (;;) {
            if (!n)
                return {nullptr, m_file};
            auto const n2 = static_cast<decltype(n)>(n >> 1);
            auto const p2 = p + n2;
            auto const sl = b + t[p2->name];
            auto const l1 = *reinterpret_cast<uint16_t const *>(sl);
            auto const s = reinterpret_cast<uint8_t const *>(sl + 2);
            auto const os = reinterpret_cast<uint8_t const *>(o);
            bool z = false;
            auto const len = l1 < l ? l1 : l;
            for (auto i = 0U; i < len; ++i) {
                if (s[i] > os[i]) {
                    n = n2;
                    z = true;
                    break;
                } else if (s[i] < os[i]) {
                    p = p2 + 1;
                    n -= n2 + 1;
                    z = true;
                    break;
                }
            }
            if (z)
                continue;
            else if (l1 < l)
                p = p2 + 1, n -= n2 + 1;
            else if (l1 > l)
                n = n2;
            else
                return {p2, m_file};
        }
    }
    int64_t node::to_integer() const {
        return m_data->ireal;
    }
    double node::to_real() const {
        return m_data->dreal;
    }
    std::string node::to_string() const {
        auto const s = reinterpret_cast<char const *>(m_file->base)
            + m_file->string_table[m_data->string];
        return {s + 2, *reinterpret_cast<uint16_t const *>(s)};
    }
    vector2i node::to_vector() const {
        return {m_data->vector[0], m_data->vector[1]};
    }
    bitmap node::to_bitmap() const {
        return {reinterpret_cast<char const *>(m_file->base)
            + m_file->bitmap_table[m_data->bitmap.index],
            m_data->bitmap.width, m_data->bitmap.height};
    }
    audio node::to_audio() const {
        return {reinterpret_cast<char const *>(m_file->base)
            + m_file->audio_table[m_data->audio.index],
            m_data->audio.length};
    }
    node node::root() const {
        return {m_file->node_table, m_file};
    }
    node node::resolve(std::string path) const {
        std::istringstream stream(path);
        std::vector<std::string> parts;
        std::string segment;
        while (std::getline(stream, segment, '/'))
            parts.push_back(segment);
        auto n = *this;
        for (auto & part : parts) {
            n = n[part];
        }
        return n;
    }
}
