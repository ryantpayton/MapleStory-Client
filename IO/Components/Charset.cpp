/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015 Daniel Allendorf                                        //
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
#include "Charset.h"

namespace jrc
{
	Charset::Charset(nl::node src, Alignment alg)
	{
		for (auto sub : src)
		{
			if (sub.data_type() == nl::node::type::bitmap)
			{
				int8_t c = sub.name()[0];
				if (c == '\\')
				{
					c = '/';
				}
				chars[c] = sub;
			}
		}
		alignment = alg;
	}

	Charset::~Charset() {}

	void Charset::draw(int8_t c, const DrawArgument& args) const
	{
		auto iter = chars.find(c);
		if (iter != chars.end())
			iter->second.draw(args);
	}

	int16_t Charset::getw(int8_t c) const
	{
		auto iter = chars.find(c);
		return iter != chars.end() ? iter->second.width() : 0;
	}

	int16_t Charset::draw(std::string str, const DrawArgument& args) const
	{
		size_t length = str.size();
		int16_t shift = 0;
		int16_t total = 0;

		switch (alignment)
		{
		case CENTER:
			for (size_t i = 0; i < length; i++)
			{
				total += getw(str[i]);
			}
			shift -= total / 2;
		case LEFT:
			for (size_t i = 0; i < length; i++)
			{
				draw(str[i], DrawArgument(args.getpos() + Point<int16_t>(shift, 0), args.getopacity()));
				shift += getw(str[i]);
			}
			break;
		case RIGHT:
			for (size_t i = length - 1; i >= 0 && i < length; i--)
			{
				shift += getw(str[i]);
				draw(str[i], DrawArgument(args.getpos() - Point<int16_t>(shift, 0), args.getopacity()));
			}
			break;
		}
		return shift;
	}

	int16_t Charset::draw(std::string str, int16_t hspace, const DrawArgument& args) const
	{
		size_t length = str.size();
		int16_t shift = 0;

		switch (alignment)
		{
		case CENTER:
			shift -= (hspace * static_cast<int16_t>(length)) / 2;
		case LEFT:
			for (size_t i = 0; i < length; i++)
			{
				draw(str[i], DrawArgument(args.getpos() + Point<int16_t>(shift, 0), args.getopacity()));
				shift += hspace;
			}
			break;
		case RIGHT:
			for (size_t i = length - 1; i >= 0 && i < length; i--)
			{
				shift += hspace;
				draw(str[i], DrawArgument(args.getpos() - Point<int16_t>(shift, 0), args.getopacity()));
			}
			break;
		}
		return shift;
	}
}
