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

namespace IO
{
	Charset::Charset(node src, CharsetAlignment alg)
	{
		for (node sub = src.begin(); sub != src.end(); ++sub)
		{
			if (sub.data_type() == node::type::bitmap)
			{
				int8_t c = sub.name()[0];
				if (c == '\\')
				{
					c = '/';
				}
				chars.add(c, new Texture(sub));
			}
		}
		alignment = alg;
	}

	Charset::~Charset() {}

	void Charset::draw(int8_t c, const DrawArgument& args) const
	{
		const Texture* txt = chars.get(c);
		if (txt)
		{
			txt->draw(args);
		}
	}

	int32_t Charset::getw(int8_t c) const
	{
		const Texture* txt = chars.get(c);
		if (txt)
			return txt->getdimensions().x();
		else
			return 0;
	}

	int32_t Charset::draw(string str, const DrawArgument& args) const
	{
		size_t length = str.size();
		int32_t shift = 0;
		int32_t total = 0;

		using::Util::vector2d;
		using::Graphics::AlphaArgument;
		switch (alignment)
		{
		case CHA_CENTER:
			for (size_t i = 0; i < length; i++)
			{
				total += getw(str[i]);
			}
			shift -= total / 2;
		case CHA_LEFT:
			for (size_t i = 0; i < length; i++)
			{
				draw(str[i], AlphaArgument(args.getpos() + vector2d<int32_t>(shift, 0), args.getalpha()));
				shift += getw(str[i]);
			}
			break;
		case CHA_RIGHT:
			for (size_t i = length - 1; i >= 0 && i < length; i--)
			{
				shift += getw(str[i]);
				draw(str[i], AlphaArgument(args.getpos() - vector2d<int32_t>(shift, 0), args.getalpha()));
			}
			break;
		}
		return shift;
	}
}
