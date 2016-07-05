/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015-2016 Daniel Allendorf                                   //
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
#include "Player.h"
#include "PlayerStates.h"

#include "../Constants.h"
#include "../Data/WeaponData.h"
#include "../IO/UI.h"
#include "../IO/UITypes/UIStatsinfo.h"
#include "../Net/Packets/GameplayPackets.h"
#include "../Net/Packets/InventoryPackets.h"

namespace jrc
{
	const PlayerNullState nullstate;

	const PlayerState* get_state(Char::State state)
	{
		static PlayerStandState standing;
		static PlayerWalkState walking;
		static PlayerFallState falling;
		static PlayerProneState lying;
		static PlayerClimbState climbing;
		static PlayerSitState sitting;
		static PlayerFlyState flying;

		switch (state)
		{
		case Char::STAND:
			return &standing;
		case Char::WALK:
			return &walking;
		case Char::FALL:
			return &falling;
		case Char::PRONE:
			return &lying;
		case Char::LADDER:
		case Char::ROPE:
			return &climbing;
		case Char::SIT:
			return &sitting;
		case Char::SWIM:
			return &flying;
		default:
			return nullptr;
		}
	}

	Player::Player(const CharEntry& entry)
		: Char(entry.cid, entry.look, entry.stats.name), stats(entry.stats) {

		attacking = false;
		underwater = false;

		set_state(STAND);
		set_direction(false);
	}

	Player::Player() 
		: Char(0, {}, "") {}

	void Player::respawn(Point<int16_t> pos, bool uw)
	{
		set_position(pos.x(), pos.y());
		underwater = uw;
		keysdown.clear();
		attacking = false;
		ladder = nullptr;
		nullstate.update_state(*this);
	}

	void Player::send_action(KeyAction::Id action, bool down)
	{
		const PlayerState* pst = get_state(state);
		if (pst)
		{
			pst->send_action(*this, action, down);
		}
		keysdown[action] = down;
	}

	void Player::recalc_stats(bool equipchanged)
	{
		Weapon::Type weapontype = get_weapontype();

		stats.set_weapontype(weapontype);
		stats.init_totalstats();

		if (equipchanged)
		{
			inventory.recalc_stats(weapontype);
		}

		for (auto stat : Equipstat::values)
		{
			int32_t inventory_total = inventory.get_stat(stat);
			stats.add_value(stat, inventory_total);
		}

		auto passive_skills = skillbook.collect_passives();
		for (auto& passive : passive_skills)
		{
			int32_t skill_id = passive.first;
			int32_t skill_level = passive.second;

			passive_buffs.apply_buff(stats, skill_id, skill_level);
		}

		for (const Buff& buff : buffs.values())
		{
			active_buffs.apply_buff(stats, buff.stat, buff.value);
		}

		stats.close_totalstats();

		if (auto statsinfo = UI::get().get_element<UIStatsinfo>())
			statsinfo->update_all_stats();
	}

	void Player::change_equip(int16_t slot)
	{
		if (int32_t itemid = inventory.get_item_id(InventoryType::EQUIPPED, slot))
		{
			look.add_equip(itemid);
		}
		else
		{
			look.remove_equip(Equipslot::by_id(slot));
		}
	}

	void Player::use_item(int32_t itemid)
	{
		InventoryType::Id type = InventoryType::by_item_id(itemid);
		if (int16_t slot = inventory.find_item(type, itemid))
		{
			switch (type)
			{
			case InventoryType::USE:
				UseItemPacket(slot, itemid).dispatch();
				break;
			}
		}
	}

	void Player::draw(Layer::Id layer, double viewx, double viewy, float alpha) const
	{
		if (layer == get_layer())
		{
			Char::draw(viewx, viewy, alpha);
		}
	}

	int8_t Player::update(const Physics& physics)
	{
		const PlayerState* pst = get_state(state);
		if (pst)
		{
			pst->update(*this);
			physics.move_object(phobj);

			bool aniend = Char::update(physics, get_stancespeed());
			if (aniend && attacking)
			{
				attacking = false;
				nullstate.update_state(*this);
			}
			else
			{
				pst->update_state(*this);
			}
		}

		uint8_t stancebyte = flip ? state : state + 1;
		Movement newmove(phobj, stancebyte);
		bool needupdate = lastmove.hasmoved(newmove);
		if (needupdate)
		{
			MovePlayerPacket(newmove).dispatch();
			lastmove = newmove;
		}

		return get_layer();
	}

	int8_t Player::get_integer_attackspeed() const
	{
		int32_t weapon_id = look.get_equips().get_weapon();
		if (weapon_id <= 0)
			return 0;

		const WeaponData& weapon = WeaponData::get(weapon_id);

		int8_t base_speed = stats.get_attackspeed();
		int8_t weapon_speed = weapon.get_speed();
		return base_speed + weapon_speed;
	}

	void Player::set_direction(bool flipped)
	{
		if (!attacking)
			Char::set_direction(flipped);
	}

	void Player::set_state(State st)
	{
		if (!attacking)
		{
			Char::set_state(st);

			const PlayerState* pst = get_state(st);
			if (pst)
			{
				pst->initialize(*this);
			}
		}
	}

	bool Player::is_attacking() const
	{
		return attacking;
	}

	bool Player::can_attack() const
	{
		return !attacking && !is_climbing() && !is_sitting() && look.get_equips().has_weapon();
	}

	SpecialMove::ForbidReason Player::can_use(const SpecialMove& move) const
	{
		if (move.is_skill() && state == PRONE)
			return SpecialMove::FBR_OTHER;

		if (move.is_attack() && (state == LADDER || state == ROPE))
			return SpecialMove::FBR_OTHER;

		if (has_cooldown(move.get_id()))
			return SpecialMove::FBR_COOLDOWN;

		int32_t level = skillbook.get_level(move.get_id());
		Weapon::Type weapon = get_weapontype();
		const Job& job = stats.get_job();
		uint16_t hp = stats.get_stat(Maplestat::HP);
		uint16_t mp = stats.get_stat(Maplestat::MP);
		uint16_t bullets = inventory.get_bulletcount();
		return move.can_use(level, weapon, job, hp, mp, bullets);
	}

	Attack Player::prepare_attack(bool skill) const
	{
		Attack::Type attacktype;
		bool degenerate;
		if (state == PRONE)
		{
			degenerate = true;
			attacktype = Attack::CLOSE;
		}
		else
		{
			Weapon::Type weapontype = get_weapontype();
			switch (weapontype)
			{
			case Weapon::BOW:
			case Weapon::CROSSBOW:
			case Weapon::CLAW:
			case Weapon::GUN:
				degenerate = !inventory.has_projectile();
				attacktype = degenerate ? Attack::CLOSE : Attack::RANGED;
				break;
			case Weapon::WAND:
			case Weapon::STAFF:
				degenerate = !skill;
				attacktype = degenerate ? Attack::CLOSE : Attack::MAGIC;
				break;
			default:
				attacktype = Attack::CLOSE;
				degenerate = false;
			}
		}

		Attack attack;
		attack.type = attacktype;
		attack.mindamage = stats.get_mindamage();
		attack.maxdamage = stats.get_maxdamage();
		if (degenerate)
		{
			attack.mindamage /= 10;
			attack.maxdamage /= 10;
		}
		attack.critical = stats.get_critical();
		attack.ignoredef = stats.get_ignoredef();
		attack.accuracy = stats.get_total(Equipstat::ACC);
		attack.playerlevel = stats.get_stat(Maplestat::LEVEL);
		attack.range = stats.get_range();
		attack.bullet = inventory.get_bulletid();
		attack.origin = get_position();
		attack.toleft = !flip;
		attack.speed = get_integer_attackspeed();

		return attack;
	}

	void Player::rush(double targetx)
	{
		if (phobj.onground)
		{
			uint16_t delay = get_attackdelay(1);
			phobj.movexuntil(targetx, delay);
			phobj.set_flag(PhysicsObject::TURNATEDGES);
		}
	}

	bool Player::is_invincible() const
	{
		if (state == DIED)
			return true;

		if (has_buff(Buffstat::DARKSIGHT))
			return true;

		return Char::is_invincible();
	}

	MobAttackResult Player::damage(const MobAttack& attack)
	{
		int32_t damage = stats.calculate_damage(attack.watk);
		show_damage(damage);

		bool fromleft = attack.origin.x() > phobj.get_x();

		bool missed = damage <= 0;
		bool immovable = ladder || state == DIED;
		bool knockback = !missed && !immovable;
		if (knockback && randomizer.above(stats.get_stance()))
		{
			phobj.hspeed = fromleft ? -1.5 : 1.5;
			phobj.vforce -= 3.5;
		}

		uint8_t direction = fromleft ? 0 : 1;
		return{ attack, damage, direction };
	}

	void Player::give_buff(Buff buff)
	{
		buffs[buff.stat] = buff;
	}

	void Player::cancel_buff(Buffstat::Id stat)
	{
		buffs[stat] = {};
	}

	bool Player::has_buff(Buffstat::Id stat) const
	{
		return buffs[stat].value > 0;
	}

	void Player::change_skill(int32_t skill_id, int32_t skill_level,
		int32_t masterlevel, int64_t expiration) {

		int32_t old_level = skillbook.get_level(skill_id);
		skillbook.set_skill(skill_id, skill_level, masterlevel, expiration);

		if (old_level != skill_level)
		{
			recalc_stats(false);
		}
	}

	void Player::add_cooldown(int32_t skill_id, int32_t cooltime)
	{
		cooldowns[skill_id] = cooltime;
	}

	bool Player::has_cooldown(int32_t skill_id) const
	{
		auto iter = cooldowns.find(skill_id);
		if (iter == cooldowns.end())
			return false;

		return iter->second > 0;
	}

	void Player::change_level(uint16_t level)
	{
		uint16_t oldlevel = get_level();
		if (level > oldlevel)
		{
			show_effect_id(CharEffect::LEVELUP);
		}
		stats.set_stat(Maplestat::LEVEL, level);
	}

	uint16_t Player::get_level() const
	{
		return stats.get_stat(Maplestat::LEVEL);
	}

	int32_t Player::get_skilllevel(int32_t skillid) const
	{
		return skillbook.get_level(skillid);
	}

	void Player::change_job(uint16_t jobid)
	{
		show_effect_id(CharEffect::JOBCHANGE);
		stats.change_job(jobid);
	}

	void Player::set_seat(Optional<const Seat> seat)
	{
		if (seat)
		{
			set_position(seat->getpos());
			set_state(Char::SIT);
		}
	}

	void Player::set_ladder(Optional<const Ladder> ldr)
	{
		ladder = ldr;

		if (ladder)
		{
			phobj.set_x(ldr->get_x());
			phobj.hspeed = 0.0;
			phobj.vspeed = 0.0;
			phobj.fhlayer = 7;
			set_state(ldr->is_ladder() ? Char::LADDER : Char::ROPE);
			set_direction(false);
		}
	}
	
	float Player::get_walkforce() const
	{
		return 0.05f + 0.11f * static_cast<float>(stats.get_total(Equipstat::SPEED)) / 100;
	}

	float Player::get_jumpforce() const
	{
		return 1.0f + 3.5f * static_cast<float>(stats.get_total(Equipstat::JUMP)) / 100;
	}

	float Player::get_climbforce() const
	{
		return static_cast<float>(stats.get_total(Equipstat::SPEED)) / 100;
	}

	float Player::get_flyforce() const
	{
		return 0.25f;
	}

	bool Player::is_underwater() const
	{
		return underwater;
	}

	bool Player::is_key_down(KeyAction::Id action) const
	{
		return keysdown.count(action) ? keysdown.at(action) : false;
	}

	CharStats& Player::get_stats()
	{
		return stats;
	}

	const CharStats& Player::get_stats() const
	{
		return stats;
	}

	Inventory& Player::get_inventory()
	{
		return inventory;
	}

	const Inventory& Player::get_inventory() const
	{
		return inventory;
	}

	Skillbook& Player::get_skills()
	{
		return skillbook;
	}

	Questlog& Player::get_quests()
	{
		return questlog;
	}

	Telerock& Player::get_telerock()
	{
		return telerock;
	}

	Monsterbook& Player::get_monsterbook()
	{
		return monsterbook;
	}

	Optional<const Ladder> Player::get_ladder() const
	{
		return ladder;
	}
}
