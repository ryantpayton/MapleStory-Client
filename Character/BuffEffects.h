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
#pragma once
#include "Buffstat.h"
#include "CharStats.h"

namespace Character
{
	// Interface for buff effects which are applied to character stats.
	class BuffEffect
	{
	public:
		virtual ~BuffEffect() {}
		virtual void applyto(int16_t value, CharStats& stats) const = 0;
	};

	// Base class for effects which apply an increase by percentage.
	class PercentageEffect : public BuffEffect
	{
	protected:
		void addpercentage(int16_t value, Equipstat es, CharStats& stats) const
		{
			float floatvalue = static_cast<float>(value) / 100;
			int32_t total = static_cast<int32_t>(stats.gettotal(es) * floatvalue);
			stats.addtotal(es, total);
		}
	};

	// Effect for BOOSTER
	class BoosterEffect : public BuffEffect
	{
		void applyto(int16_t, CharStats& stats) const override
		{
			stats.setattackspeed(-2);
		}
	};

	// Effect for HYPERBODYHP
	class HyperbodyHPEffect : public PercentageEffect
	{
	public:
		void applyto(int16_t value, CharStats& stats) const override
		{
			addpercentage(value, ES_HP, stats);
		}
	};

	// Effect for HYPERBODYMP
	class HyperbodyMPEffect : public PercentageEffect
	{
	public:
		void applyto(int16_t value, CharStats& stats) const override
		{
			addpercentage(value, ES_MP, stats);
		}
	};

	// Return the buff effect associated with the buff stat.
	const BuffEffect* geteffectbystat(Buffstat stat);
}