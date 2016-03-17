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
#include "DamageNumber.h"
#include "Constants.h"

#include "nlnx\nx.hpp"
#include "nlnx\node.hpp"

namespace IO
{
	DamageNumber::DamageNumber(Type t, int32_t damage, Point<int16_t> pos)
	{
		type = t;

		if (damage > 0)
		{
			miss = false;

			string number = std::to_string(damage);
			firstnum = number[0];
			if (number.size() > 1)
			{
				restnum = number.substr(1);
				multiple = true;
			}
			else
			{
				restnum = "";
				multiple = false;
			}

			int16_t total = getadvance(firstnum, true);
			for (size_t i = 0; i < restnum.length(); i++)
			{
				char c = restnum[i];
				int16_t advance;
				if (i < restnum.length() - 1)
				{
					char n = restnum[i + 1];
					advance = (getadvance(c, false) + getadvance(n, false)) / 2;
				}
				else
				{
					advance = getadvance(c, false);
				}
				total += advance;
			}
			shift = total / 2;
		}
		else
		{
			shift = charsets[type].second.getw('M') / 2;
			miss = true;
		}

		opacity.set(1.5f);
		moveobj.setx(pos.x());
		moveobj.sety(pos.y());
		moveobj.vspeed = -0.25;
	}

	DamageNumber::DamageNumber() {}

	DamageNumber::~DamageNumber() {}

	void DamageNumber::draw(Point<int16_t> parentpos, float alpha) const
	{
		Point<int16_t> position = moveobj.getposition(alpha) + parentpos - Point<int16_t>(0, shift);
		float interopc = opacity.get(alpha);
		if (miss)
		{
			charsets[type].second.draw('M', DrawArgument(position, interopc));
		}
		else
		{
			charsets[type].first.draw(firstnum, DrawArgument(position, interopc));

			if (multiple)
			{
				position.shiftx(getadvance(firstnum, true));

				for (size_t i = 0; i < restnum.length(); i++)
				{
					char c = restnum[i];
					charsets[type].second.draw(c, DrawArgument(position, interopc));

					int16_t advance;
					if (i < restnum.length() - 1)
					{
						char n = restnum[i + 1];
						advance = (getadvance(c, false) + getadvance(n, false)) / 2;
					}
					else
					{
						advance = getadvance(c, false);
					}

					position.shiftx(advance);
				}
			}
		}
	}

	int16_t DamageNumber::getadvance(char c, bool first) const
	{
		static const size_t LENGTH = 10;
		static const int16_t advances[LENGTH] =
		{
			24, 20, 22, 22, 24, 23, 24, 22, 24, 24
		};

		size_t index = c - 48;
		if (index < LENGTH)
		{
			int16_t advance = advances[index];
			switch (type)
			{
			case CRITICAL:
				if (first)
				{
					advance += 8;
				}
				else
				{
					advance += 4;
				}
				break;
			default:
				if (first)
				{
					advance += 2;
				}
			}
			return advance;
		}
		else
		{
			return 0;
		}
	}

	bool DamageNumber::update()
	{
		moveobj.move();
		opacity -= Constants::TIMESTEP * 0.0015f;
		return opacity.last() <= 0.0f;
	}


	void DamageNumber::init()
	{
		charsets[NORMAL].first = Charset(nl::nx::effect["BasicEff.img"]["NoRed1"], Charset::LEFT);
		charsets[NORMAL].second = Charset(nl::nx::effect["BasicEff.img"]["NoRed0"], Charset::LEFT);
		charsets[CRITICAL].first = Charset(nl::nx::effect["BasicEff.img"]["NoCri1"], Charset::LEFT);
		charsets[CRITICAL].second = Charset(nl::nx::effect["BasicEff.img"]["NoCri0"], Charset::LEFT);
		charsets[TOPLAYER].first = Charset(nl::nx::effect["BasicEff.img"]["NoViolet1"], Charset::LEFT);
		charsets[TOPLAYER].second = Charset(nl::nx::effect["BasicEff.img"]["NoViolet0"], Charset::LEFT);
	}
	pair<Charset, Charset> DamageNumber::charsets[NUM_TYPES];
}