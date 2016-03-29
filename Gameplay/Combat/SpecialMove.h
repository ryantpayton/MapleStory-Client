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
#include "SkillHitEffect.h"
#include "SkillUseEffect.h"
#include <memory>

namespace Gameplay
{
	using std::unique_ptr;
	using Character::Weapon;

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

		virtual void applyuseeffects(Char& user, Attack::Type type) const;
		virtual void applyhiteffects(Mob& target, uint16_t level, bool twohanded) const;		
		virtual Animation getbullet(int32_t bulletid) const;

		virtual void applystats(const Char& user, Attack& attack) const = 0;
		virtual bool isoffensive() const = 0;
		virtual int32_t getid() const = 0;
		virtual ForbidReason canuse(int32_t level, Weapon::Type weapon, uint16_t job, uint16_t hp, uint16_t mp, uint16_t bullets) const = 0;

		bool isskill() const;

	protected:
		unique_ptr<SkillAction> action;
		unique_ptr<SkillBullet> bullet;
		unique_ptr<SkillUseEffect> useeffect;
		unique_ptr<SkillHitEffect> hiteffect;
	};
}