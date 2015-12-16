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
#include "nlnx\nx.hpp"
#include "nlnx\node.hpp"

namespace IO
{
	const std::pair<Charset, Charset>* getcharsets(DamageNumber::Type type)
	{
		static std::map<DamageNumber::Type, std::pair<Charset, Charset>> charsets;
		if (!charsets.count(type))
		{
			switch (type)
			{
			case DamageNumber::NORMAL:
				charsets[type].first = Charset(nl::nx::effect["BasicEff.img"]["NoRed0"], Charset::LEFT);
				charsets[type].second = Charset(nl::nx::effect["BasicEff.img"]["NoRed1"], Charset::LEFT);
				break;
			case DamageNumber::CRITICAL:
				charsets[type].first = Charset(nl::nx::effect["BasicEff.img"]["NoCri0"], Charset::LEFT);
				charsets[type].second = Charset(nl::nx::effect["BasicEff.img"]["NoCri1"], Charset::LEFT);
				break;
			case DamageNumber::TOPLAYER:
				charsets[type].first = Charset(nl::nx::effect["BasicEff.img"]["NoViolet0"], Charset::LEFT);
				charsets[type].second = Charset(nl::nx::effect["BasicEff.img"]["NoViolet1"], Charset::LEFT);
				break;
			default:
				return nullptr;
			}
		}
		return &charsets[type];
	}

	DamageNumber::DamageNumber(Type t, int32_t damage, float a, vector2d<int16_t> pos)
	{
		type = t;
		miss = damage == 0;

		string number = std::to_string(damage);
		firstnum = number[0];
		if (number.size() > 1)
			restnum = number.substr(1);
		else
			restnum = "";

		const std::pair<Charset, Charset>* charsets = getcharsets(type);
		int16_t total = charsets->first.getw(firstnum);
		for (size_t i = 0; i < restnum.length(); i++)
		{
			total += charsets->second.getw(restnum[i]);
		}
		shift = total / 2;

		fx = static_cast<float>(pos.x());
		fy = static_cast<float>(pos.y());
		alpha = a;
	}

	DamageNumber::DamageNumber() {}

	DamageNumber::~DamageNumber() {}

	void DamageNumber::draw(vector2d<int16_t> viewpos) const
	{
		if (alpha > 1.0f)
			return;

		const std::pair<Charset, Charset>* charsets = getcharsets(type);
		vector2d<int16_t> drawpos = 
			vector2d<int16_t>(static_cast<int16_t>(fx), static_cast<int16_t>(fy)) + viewpos;
		if (miss)
		{
			charsets->second.draw('M', DrawArgument(drawpos, alpha));
		}
		else if (restnum.size() > 0)
		{
			drawpos.shiftx(-shift);
			charsets->first.draw(firstnum, DrawArgument(drawpos, alpha));
			drawpos.shiftx(charsets->first.getw(firstnum) - 9);

			for (size_t i = 0; i < restnum.length(); i++)
			{
				charsets->second.draw(
					restnum[i], 
					DrawArgument(drawpos + vector2d<int16_t>(0, (i % 2 == 1) ? -6 : 0), 
					alpha));
				drawpos.shiftx(charsets->second.getw(restnum[i]) - 7);
			}
		}
		else
		{
			charsets->first.draw(firstnum, DrawArgument(drawpos, alpha));
		}
	}

	bool DamageNumber::update()
	{
		fy -= 0.4f;
		alpha -= 0.008f;
		return alpha <= 0.0f;
	}
}