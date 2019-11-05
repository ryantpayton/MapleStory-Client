//////////////////////////////////////////////////////////////////////////////
// NoLifeNx - Part of the NoLifeStory project                               //
// Copyright � 2013 Peter Atashian                                          //
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
#include "node.hpp"

namespace nl {
    //Internal data structure
#pragma pack(push, 1)
    struct node::data {
        std::int32_t const name;
        std::int32_t const children;
        std::uint16_t const num;
        node::type const type;
        union {
            int64_t const ireal;
            double const dreal;
            std::int32_t const string;
            std::int32_t const vector[2];
            struct {
                std::int32_t index;
                std::uint16_t width;
                std::uint16_t height;
            } const bitmap;
            struct {
                std::int32_t index;
                std::int32_t length;
            } const audio;
        };
    };
#pragma pack(pop)
}
