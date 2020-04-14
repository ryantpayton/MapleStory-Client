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
#include <string>

namespace nl {
    struct _file_data;
    class file {
    public:
        typedef _file_data data;
        struct header;
        //Creates a null file object
        //Nothing can really be done until you call open()
        file() = default;
        //Used to construct an nx file from a filename
        //Multiple file objects can be created from the same filename
        //and the resulting nodes are interchangeable
        file(std::string name);
        //Destructor calls close()
        ~file();
        //Files cannot be copied
        file(file const &) = delete;
        //Files cannot be copied
        file & operator=(file const &) = delete;
        //Transfers ownership of the file handle to another file
        file(file &&);
        //Transfers ownership of the file handle to another file
        file & operator=(file &&);
        //Opens the file with the given name
        void open(std::string name);
        //Closes the given file
        //Any nodes derived from this file become invalid after closing it
        //Any attempts to use invalid nodes will result in undefined behavior
        void close();
        //Obtains the root node from which all other nodes may be accessed
        //If the file is not open, a null node is returned
        node root() const;
        //Effectivelly calls root()
        operator node() const;
        //Returns the number of strings in the file
        uint32_t string_count() const;
        //Returns the number of bitmaps in the file
        uint32_t bitmap_count() const;
        //Returns the number of audios in the file
        uint32_t audio_count() const;
        //Returns the number of nodes in the file
        uint32_t node_count() const;
        //Returns the string with a given id number
        std::string get_string(uint32_t) const;
    private:
        data * m_data = nullptr;
        friend node;
        friend bitmap;
        friend audio;
    };
}
