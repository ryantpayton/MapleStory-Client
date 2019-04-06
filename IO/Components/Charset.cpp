//////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015-2016 Daniel Allendorf                                   //
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
	Charset::Charset(nl::node src, Alignment alignment)
		: alignment(alignment) {

		for (auto sub : src)
		{
			std::string name = sub.name();
			if (sub.data_type() != nl::node::type::bitmap || name.empty())
				continue;

			char c = *name.begin();
			if (c == '\\')
			{
				c = '/';
			}
			chars.emplace(c, sub);
		}
	}

	Charset::Charset()
		: alignment(LEFT) {}

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

	int16_t Charset::draw(const std::string& text, const DrawArgument& args) const
	{
		int16_t shift = 0;
		int16_t total = 0;

		switch (alignment)
		{
		case CENTER:
			for (char c : text)
			{
				int16_t width = getw(c);

				draw(c, args + Point<int16_t>(shift, 0));
				shift += width + 2;
				total += width;
			}
			shift -= total / 2;
			break;
		case LEFT:
			for (char c : text)
			{
				draw(c, args + Point<int16_t>(shift, 0));
				shift += getw(c);
			}
			break;
		case RIGHT:
			for (auto iter = text.rbegin(); iter != text.rend(); ++iter)
			{
				char c = *iter;
				shift += getw(c);
				draw(c, args - Point<int16_t>(shift, 0));
			}
			break;
		}
		return shift;
	}

	int16_t Charset::draw(const std::string& text, int16_t hspace, const DrawArgument& args) const
	{
		size_t length = text.size();
		int16_t shift = 0;

		switch (alignment)
		{
		case CENTER:
			shift -= hspace * static_cast<int16_t>(length) / 2;
		case LEFT:
			for (char c : text)
			{
				draw(c, args + Point<int16_t>(shift, 0));
				shift += hspace;
			}
			break;
		case RIGHT:
			for (auto iter = text.rbegin(); iter != text.rend(); ++iter)
			{
				char c = *iter;
				shift += hspace;
				draw(c, args - Point<int16_t>(shift, 0));
			}
			break;
		}
		return shift;
	}
}