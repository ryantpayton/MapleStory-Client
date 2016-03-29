//////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2016 Daniel Allendorf                                        //
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
#include "Attack.h"
#include "Gameplay\Maplemap\Mob.h"
#include "Util\BoolPair.h"

namespace Gameplay
{
	class SkillHitEffect
	{
	public:
		virtual ~SkillHitEffect() {}

		virtual void apply(Mob& target, uint16_t level, bool twohanded) const = 0;

	protected:
		struct Effect
		{
			Animation animation;
			int8_t pos;

			Effect(node src)
			{
				animation = src;
				pos = src["pos"];
			}

			Effect()
			{
				pos = 0;
			}
		};
	};


	class NoHitEffect : public SkillHitEffect
	{
	public:
		void apply(Mob&, uint16_t, bool) const override {}
	};


	class SingleHitEffect : public SkillHitEffect
	{
	public:
		SingleHitEffect(node src);

		void apply(Mob& target, uint16_t level, bool twohanded) const override;

	private:
		Effect effect;
	};


	// The animation changes depending on the weapon used.
	class TwoHHitEffect : public SkillHitEffect
	{
	public:
		TwoHHitEffect(node src);

		void apply(Mob& target, uint16_t level, bool twohanded) const override;

	private:
		BoolPair<Effect> effects;
	};


	// The animation changes with the character level.
	class ByLevelHitEffect : public SkillHitEffect
	{
	public:
		ByLevelHitEffect(node src);

		void apply(Mob& target, uint16_t level, bool twohanded) const override;

	private:
		unordered_map<uint16_t, Effect> effects;
	};


	// The animation changes with the character level and weapon used.
	class ByLevelTwoHHitEffect : public SkillHitEffect
	{
	public:
		ByLevelTwoHHitEffect(node src);

		void apply(Mob& target, uint16_t level, bool twohanded) const override;

	private:
		unordered_map<uint16_t, BoolPair<Effect>> effects;
	};
}