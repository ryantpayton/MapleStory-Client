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
#include <string>
#include <cstdint>
#include <cstddef>

namespace nl {
    struct _file_data;
    typedef std::pair<int32_t, int32_t> vector2i;
    class node {
    public:
        using difference_type = ptrdiff_t;
        using value_type = node;
        using pointer = node;
        using reference = node &;
        using iterator_category = std::random_access_iterator_tag;
        struct data;
        //Type of node data
        enum class type : uint16_t {
            none = 0,
            integer = 1,
            real = 2,
            string = 3,
            vector = 4,
            bitmap = 5,
            audio = 6,
        };
        //Constructors
        node() = default;
        node(node const &);//Only reason this isn't defaulted is because msvc has issues
        node & operator=(node const &) = default;
        //These methods are primarily so nodes can be used as iterators and iterated over
        node begin() const;
        node end() const;
        node operator*() const;
        node & operator++();
        node operator++(int);
        bool operator==(node const &) const;
        bool operator!=(node const &) const;
        bool operator<(node const &) const;
        //This checks whether or not the node points to an actual node
        //Even if the node ends up being a null node, you can still use it safely
        //It'll just fall back to returning default values and more null nodes
        //However, if the file this node was obtained from was deleted
        //then the node becomes invalid and this operator cannot tell you that
        explicit operator bool() const;
        //Methods to access the children of the node by name
        //Note that the versions taking integers convert the integer to a string
        //They do not access the children by their integer index
        //If you wish to do that, use somenode.begin() + integer_index
        node operator[](unsigned int) const;
        node operator[](signed int) const;
        node operator[](unsigned long) const;
        node operator[](signed long) const;
        node operator[](unsigned long long) const;
        node operator[](signed long long) const;
        node operator[](std::string const &) const;
        node operator[](char const *) const;
        //This method uses the string value of the node, not the node's name
        node operator[](node const &) const;
        //Operators to easily cast a node to get the data
        //Allows things like string s = somenode
        //Will automatically cast between data types as needed
        //For example if the node has an integer value but you want a string
        //then the operator will automatically convert the integer to a string
        operator unsigned char() const;
        operator signed char() const;
        operator unsigned short() const;
        operator signed short() const;
        operator unsigned int() const;
        operator signed int() const;
        operator unsigned long() const;
        operator signed long() const;
        operator unsigned long long() const;
        operator signed long long() const;
        operator float() const;
        operator double() const;
        operator long double() const;
        operator std::string() const;
        operator vector2i() const;
        operator bitmap() const;
        operator audio() const;
        //Explicitly called versions of all the operators
        //When it takes a parameter, that is used as the default value
        //if a suitable data value cannot be found in the node
        int64_t get_integer(int64_t = 0) const;
        double get_real(double = 0) const;
        std::string get_string(std::string = "") const;
        vector2i get_vector(vector2i = {0, 0}) const;
        bitmap get_bitmap() const;
        audio get_audio() const;
        bool get_bool() const;
        bool get_bool(bool) const;
        //Returns the x and y coordinates of the vector data value
        int32_t x() const;
        int32_t y() const;
        //The name of the node
        std::string name() const;
        //The number of children in the node
        size_t size() const;
        //Gets the type of data contained within the node
        type data_type() const;
        //Returns the root node of the file this node was derived from
        node root() const;
        //Takes a '/' separated string, and resolves the given path
        node resolve(std::string) const;
    private:
        node(data const *, _file_data const *);
        node get_child(char const *, uint16_t) const;
        int64_t to_integer() const;
        double to_real() const;
        std::string to_string() const;
        vector2i to_vector() const;
        bitmap to_bitmap() const;
        audio to_audio() const;
        //Internal variables
        data const * m_data = nullptr;
        _file_data const * m_file = nullptr;
        friend file;
    };
    //More convenience string concatenation operators
    std::string operator+(std::string, node);
    std::string operator+(char const *, node);
    std::string operator+(node, std::string);
    std::string operator+(node, char const *);
}
