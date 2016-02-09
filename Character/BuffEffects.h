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
#include "Util\Singleton.h"
#include <unordered_map>
#include <memory>

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
		void addpercentage(int16_t value, Equipstat::Value es, CharStats& stats) const
		{
			float floatvalue = static_cast<float>(value) / 100;
			int32_t total = static_cast<int32_t>(stats.gettotal(es) * floatvalue);
			stats.addtotal(es, total);
		}
	};

	// Base class for effects which just add their value to a stat.
	class SimpleStatEffect : public BuffEffect
	{
	public:
		void applyto(int16_t value, CharStats& stats) const override
		{
			stats.addtotal(stat, value);
		}

	protected:
		SimpleStatEffect(Equipstat::Value s)
		{
			stat = s;
		}

	private:
		Equipstat::Value stat;
	};

	// Effect for MAPLE_WARRIOR
	class MapleWarriorEffect : public PercentageEffect
	{
	public:
		void applyto(int16_t value, CharStats& stats) const override
		{
			addpercentage(value, Equipstat::STR, stats);
			addpercentage(value, Equipstat::DEX, stats);
			addpercentage(value, Equipstat::INT, stats);
			addpercentage(value, Equipstat::LUK, stats);
		}
	};

	// Effect for STANCE
	class StanceEffect : public BuffEffect
	{
		void applyto(int16_t value, CharStats& stats) const override
		{
			stats.setstance(static_cast<float>(value) / 100);
		}
	};

	// Effect for BOOSTER
	class BoosterEffect : public BuffEffect
	{
		void applyto(int16_t value, CharStats& stats) const override
		{
			stats.setattackspeed(static_cast<int8_t>(value));
		}
	};

	// Effect for WATK
	class WATKEffect : public SimpleStatEffect
	{
	public:
		WATKEffect() : SimpleStatEffect(Equipstat::WATK) {}
	};

	// Effect for WDEF
	class WDEFEffect : public SimpleStatEffect
	{
	public:
		WDEFEffect() : SimpleStatEffect(Equipstat::WDEF) {}
	};

	// Effect for MATK
	class MATKEffect : public SimpleStatEffect
	{
	public:
		MATKEffect() : SimpleStatEffect(Equipstat::MAGIC) {}
	};

	// Effect for MDEF
	class MDEFEffect : public SimpleStatEffect
	{
	public:
		MDEFEffect() : SimpleStatEffect(Equipstat::MDEF) {}
	};

	// Effect for SPEED
	class SPEEDEffect : public SimpleStatEffect
	{
	public:
		SPEEDEffect() : SimpleStatEffect(Equipstat::SPEED) {}
	};

	// Effect for JUMP
	class JUMPEffect : public SimpleStatEffect
	{
	public:
		JUMPEffect() : SimpleStatEffect(Equipstat::JUMP) {}
	};

	// Effect for HYPERBODYHP
	class HyperbodyHPEffect : public PercentageEffect
	{
	public:
		void applyto(int16_t value, CharStats& stats) const override
		{
			addpercentage(value, Equipstat::HP, stats);
		}
	};

	// Effect for HYPERBODYMP
	class HyperbodyMPEffect : public PercentageEffect
	{
	public:
		void applyto(int16_t value, CharStats& stats) const override
		{
			addpercentage(value, Equipstat::MP, stats);
		}
	};


	using std::unordered_map;
	using std::unique_ptr;

	class BuffEffects : public Singleton<BuffEffects>
	{
	public:
		// Register all buffs effects.
		BuffEffects();

		// Return the buff effect associated with the buff stat.
		const BuffEffect* bystat(Buffstat::Value stat) const;

	private:
		unordered_map<Buffstat::Value, unique_ptr<BuffEffect>> buffeffects;
	};
}