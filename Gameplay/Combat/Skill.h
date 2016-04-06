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
#include "SpecialMove.h"

namespace Gameplay
{
	class Skill : public SpecialMove
	{
	public:
		enum Name
		{
			// Beginner
			FOLLOW_THE_LEAD = 8,
			ANGEL_BLESSING = 12,
			THREE_SNAILS = 1000,
			HEAL = 1001,
			FEATHER = 1002,
			LEGENDARY_SPIRIT = 1003,
			MONSTER_RIDER = 1004,
			ECHO_OF_THE_HERO = 1005,

			// Swordman
			IMPROVED_HP_RECOVERY = 1000000,
			IMPROVED_MAX_HP = 1000001,
			IMPROVED_LADDER_RECOVERY = 1000002,
			IRON_BODY = 1001003,
			POWER_STRIKE = 1001004,
			SLASH_BLAST = 1001005,

			// Fighter
			SWORD_MASTERY = 1100000,
			AXE_MASTERY = 1100001,
			SWORD_FA = 1100002,
			AXE_FA = 1100003,
			SWORD_BOOSTER = 1101004,
			AXE_BOOSTER = 1101005,
			RAGE = 1101006,
			POWER_GUARD = 1101007,

			// Crusader

			// Hero
			HERO_RUSH = 1121006,
			BRANDISH = 1121008,

			// Paladin
			PALADIN_RUSH = 1221007,

			// Spearman
			SPEAR_MASTERY = 1300000,
			PA_MASTERY = 1300001,
			SPEAR_FA = 1300002,
			PA_FA = 1300003,
			SPEAR_BOOSTER = 1301004,
			PA_BOOSTER = 1301005,
			IRON_WILL = 1301006,
			HYPER_BODY = 1301007,

			// Dragon Knight
			DRAGON_BUSTER = 1311001,
			DRAGON_FURY = 1311002,
			PA_BUSTER = 1311003,
			PA_FURY = 1311004,
			SACRIFICE = 1311005,
			DRAGONS_ROAR = 1311006,

			// Dark Knight
			DK_RUSH = 1321003,

			// Magician
			IMPROVE_HP_RECOVERY = 2000000,
			IMPROVE_MAX_HP = 2000001,
			MAGIC_GUARD = 2001002,
			MAGIC_ARMOR = 2001003,
			ENERGY_BOLT = 2001004,
			MAGIC_CLAW = 2001005,

			// F/P Mage
			FP_TELEPORT = 2101002,

			// I/L Mage
			IL_TELEPORT = 2201002,

			// Priest
			PRIEST_TELEPORT = 2301001,

			// Hermit
			AVENGER = 4111005,
			FLASH_JUMP = 4111006,

			// Night Lord
			TRIPLE_THROW = 4121007,

			// Bandit
			MESO_EXPLOSION = 4211006
		};

		enum IconType
		{
			NORMAL,
			MOUSEOVER,
			DISABLED
		};

		struct Level
		{
			float chance = 0.0f;
			float damage = 0.0f;
			float critical = 0.0f;
			float ignoredef = 0.0f;
			uint8_t attackcount = 1;
			uint8_t bulletcount = 1;
			uint8_t mobcount = 1;
			int32_t hpcost = 0;
			int32_t mpcost = 0;
			int16_t bulletcost = 0;
			int32_t matk = 0;
			int32_t mastery = 0;
			float hrange = 1.0f;
			rectangle2d<int16_t> range;
		};

		Skill(int32_t skillid);
		Skill();
		~Skill();

		void applyuseeffects(Char& user, Attack::Type type) const override;
		void applyhiteffects(Mob& target, uint16_t level, bool twohanded) const override;
		void applystats(const Char& user, Attack& attack) const override;

		bool isoffensive() const override;
		int32_t getid() const override;
		ForbidReason canuse(int32_t level, Weapon::Type weapon, uint16_t job, uint16_t hp, uint16_t mp, uint16_t bullets) const override;

	private:
		unordered_map<int32_t, Level> levels;
		Sound usesound;
		Sound hitsound;
		Weapon::Type reqweapon;
		int32_t skillid;
		bool passive;
		bool offensive;
		bool overregular;
	};
}