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
#include "SkillAction.h"
#include "SkillBullet.h"
#include "SkillSound.h"
#include "SkillHitEffect.h"
#include "SkillUseEffect.h"

#include <memory>

namespace jrc
{
	// Base class for attacks and buffs.
	class SpecialMove
	{
	public:
		enum ForbidReason
		{
			FBR_NONE,
			FBR_WEAPONTYPE,
			FBR_HPCOST,
			FBR_MPCOST,
			FBR_BULLETCOST,
			FBR_OTHER
		};

		virtual ~SpecialMove() {}

		void applyuseeffects(Char& user, Attack::Type type) const;
		void applyhiteffects(const AttackUser& user, Mob& target) const;
		bool isskill() const;
		Animation getbullet(const Char& user, int32_t bulletid) const;

		virtual void applystats(const Char& user, Attack& attack) const = 0;
		virtual bool isoffensive() const = 0;
		virtual int32_t getid() const = 0;
		virtual ForbidReason canuse(int32_t level, Weapon::Type weapon, uint16_t job, uint16_t hp, uint16_t mp, uint16_t bullets) const = 0;

	protected:
		std::unique_ptr<SkillAction> action;
		std::unique_ptr<SkillBullet> bullet;
		std::unique_ptr<SkillSound> sound;
		std::unique_ptr<SkillUseEffect> useeffect;
		std::unique_ptr<SkillHitEffect> hiteffect;
	};
}