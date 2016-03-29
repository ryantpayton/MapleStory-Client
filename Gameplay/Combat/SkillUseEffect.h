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
#include "Character\Char.h"
#include "Graphics\Animation.h"
#include "Util\BoolPair.h"
#include <unordered_map>
#include <vector>

namespace Gameplay
{
	using std::vector;
	using std::unordered_map;
	using Character::Char;
	using Graphics::Animation;

	// Interface for skill effects.
	class SkillUseEffect
	{
	public:
		virtual ~SkillUseEffect() {}

		virtual void apply(Char& target) const = 0;

	protected:
		struct Effect
		{
			Animation animation;
			int8_t z;

			Effect(node src)
			{
				animation = src;
				z = src["z"];
			}

			Effect()
			{
				z = 0;
			}
		};
	};


	// An effect which does nothing.
	class NoUseEffect : public SkillUseEffect
	{
	public:
		void apply(Char&) const override {}
	};


	// An effect which displays an animation over the character's position.
	class SingleUseEffect : public SkillUseEffect
	{
	public:
		SingleUseEffect(node src);

		void apply(Char& target) const override;

	private:
		Effect effect;
	};


	// An effect which displays an animation over the character's position.
	// The effect changes based on wether the character uses a twohanded weapon.
	class TwoHUseEffect : public SkillUseEffect
	{
	public:
		TwoHUseEffect(node src);

		void apply(Char& target) const override;

	private:
		BoolPair<Effect> effects;
	};


	// An effect which displays multiple animations over the character's position.
	class MultiUseEffect : public SkillUseEffect
	{
	public:
		MultiUseEffect(node src);

		void apply(Char& target) const override;

	private:
		vector<Effect> effects;
	};


	// The animation changes with the character level.
	class ByLevelUseEffect : public SkillUseEffect
	{
	public:
		ByLevelUseEffect(node src);

		void apply(Char& target) const override;

	private:
		unordered_map<uint16_t, Effect> leveleffects;
	};
}