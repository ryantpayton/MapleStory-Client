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

#include "..\..\Constants.h"

#include <nlnx\nx.hpp>
#include <nlnx\node.hpp>

namespace jrc
{
	DamageNumber::DamageNumber(Type t, int32_t damage, int16_t starty)
	{
		type = t;

		if (damage > 0)
		{
			miss = false;

			std::string number = std::to_string(damage);
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

		moveobj.sety(starty);
		moveobj.vspeed = -0.25;
		opacity.set(1.5f);
	}

	DamageNumber::DamageNumber() {}

	void DamageNumber::draw(double viewx, double viewy, float alpha) const
	{
		Point<int16_t> absolute = moveobj.getabsolute(viewx, viewy, alpha);
		Point<int16_t> position = absolute - Point<int16_t>(0, shift);
		float interopc = opacity.get(alpha);

		if (miss)
		{
			charsets[type].second
				.draw('M', { position, interopc });
		}
		else
		{
			charsets[type].first
				.draw(firstnum, { position, interopc });

			if (multiple)
			{
				int16_t first_advance = getadvance(firstnum, true);
				position.shiftx(first_advance);

				for (size_t i = 0; i < restnum.length(); i++)
				{
					char c = restnum[i];
					Point<int16_t> yshift = { 0, (i % 2) ? -2 : 2 };
					charsets[type].second
						.draw(c, { position + yshift, interopc });

					int16_t advance;
					if (i < restnum.length() - 1)
					{
						char n = restnum[i + 1];
						int16_t c_advance = getadvance(c, false);
						int16_t n_advance = getadvance(n, false);
						advance = (c_advance + n_advance) / 2;
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
		constexpr size_t LENGTH = 10;
		constexpr int16_t advances[LENGTH] =
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

	void DamageNumber::setx(int16_t headx)
	{
		moveobj.setx(headx);
	}

	bool DamageNumber::update()
	{
		moveobj.move();
		opacity -= Constants::TIMESTEP * 0.0015f;
		return opacity.last() <= 0.0f;
	}


	int16_t DamageNumber::rowheight(bool critical)
	{
		return critical ? 36 : 30;
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
	std::pair<Charset, Charset> DamageNumber::charsets[NUM_TYPES];
}