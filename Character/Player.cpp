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
#include "Player.h"
#include "PlayerStates.h"
#include "Constants.h"

#include "Data\DataFactory.h"

#include "Net\Session.h"
#include "Net\Packets\GameplayPackets.h"
#include "Net\Packets\InventoryPackets.h"

namespace Character
{
	using Data::DataFactory;
	using Net::Session;

	const PlayerNullState nullstate;

	const PlayerState* getstate(Char::State state)
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
	{
		oid = entry.cid;
		look = entry.getlook();
		stats = entry.stats;

		namelabel = Text(Text::A13M, Text::CENTER, Text::WHITE);
		namelabel.settext(stats.getname());
		namelabel.setback(Text::NAMETAG);

		sendcd = Constants::TIMESTEP;
		active = true;
		attacking = false;
		ladder = nullptr;

		setstate(STAND);
		setflip(false);
	}

	Player::Player() 
	{
		oid = 0;
	}

	void Player::respawn(Point<int16_t> pos)
	{
		keysdown.clear();
		setposition(pos.x(), pos.y());
		sendcd = Constants::TIMESTEP;
		attacking = false;
		ladder = nullptr;
		nullstate.nextstate(*this);
	}

	void Player::sendaction(Keyboard::Action action, bool down)
	{
		const PlayerState* pst = getstate(state);
		if (pst)
		{
			pst->sendaction(*this, action, down);
		}
		keysdown[action] = down;
	}

	void Player::recalcstats(bool equipchanged)
	{
		stats.inittotalstats();

		Weapon::Type weapontype = look.getequips().getweapontype();
		if (equipchanged)
		{
			inventory.recalcstats(weapontype);
		}

		inventory.addtotalsto(stats);

		for (auto& buff : buffs)
		{
			buff.second.applyto(stats);
		}

		stats.closetotalstats(weapontype);
	}

	void Player::changecloth(int16_t slot)
	{
		Optional<Equip> equip = inventory.getequip(Inventory::EQUIPPED, slot);
		if (equip)
		{
			int32_t itemid = equip.map(&Item::getid);
			look.addequip(itemid);
		}
		else
		{
			Equipslot::Value value = Equipslot::byvalue(slot);
			look.removeequip(value);
		}
	}

	void Player::useitem(int32_t itemid)
	{
		Inventory::Type type = Inventory::typebyid(itemid);
		int16_t slot = inventory.finditem(type, itemid);

		if (slot >= 0)
		{
			switch (type)
			{
			case Inventory::USE:
				using Net::UseItemPacket;
				Session::get().dispatch(UseItemPacket(slot, itemid));
				break;
			}
		}
	}

	int8_t Player::update(const Physics& physics)
	{
		const PlayerState* pst = getstate(state);
		if (pst)
		{
			pst->update(*this);
			physics.moveobject(phobj);
			Char::update(physics);
			pst->nextstate(*this);
		}

		if (sendcd > 0)
		{
			sendcd -= Constants::TIMESTEP;
		}
		else
		{
			uint8_t stancebyte = flip ? state : state + 1;
			Movement newmove = Movement(phobj, stancebyte);
			bool needupdate = lastmove.hasmoved(newmove) || movements.size() > 0;
			if (needupdate)
			{
				movements.push_back(newmove);
				lastmove = newmove;

				if (movements.size() > 4)
				{
					using Net::MovePlayerPacket;
					Session::get().dispatch(MovePlayerPacket(movements));
					movements.clear();
				}
			}
		}
		
		uint16_t stancespeed = static_cast<uint16_t>(Constants::TIMESTEP * getstancespeed());
		bool aniend = look.update(stancespeed);
		if (aniend && attacking)
		{
			attacking = false;
			nullstate.nextstate(*this);
		}

		return getlayer();
	}

	float Player::getstancespeed() const
	{
		if (attacking)
			return getattackspeed();

		switch (state)
		{
		case WALK:
			return static_cast<float>(1.0 + std::abs(phobj.hspeed) / 10);
		case LADDER:
		case ROPE:
			return static_cast<float>(std::abs(phobj.vspeed));
		default:
			return 1.0f;
		}
	}

	float Player::getattackspeed() const
	{
		uint8_t statsspeed = stats.getattackspeed();
		uint8_t weaponspeed = look.getequips().getweapon()
			.mapordefault(&Weapon::getspeed, uint8_t(0));

		float delay = static_cast<float>(weaponspeed + statsspeed);
		return 1.7f - (delay / 10);
	}

	bool Player::isattacking() const
	{
		return attacking;
	}

	bool Player::canattack() const
	{
		return !attacking && !isclimbing() && !issitting() && look.getequips().hasweapon();
	}

	bool Player::canuseskill(int32_t skillid) const
	{
		const Skill& skill = DataFactory::get().getskill(skillid);
		
		int32_t skilllevel = skillbook.getlevel(skillid);
		Optional<const SkillLevel> level = skill.getlevel(skilllevel);
		if (level)
		{
			if (level->hpcost >= stats.getstat(Maplestat::HP))
				return false;

			if (level->mpcost > stats.getstat(Maplestat::MP))
				return false;

			return canattack();
		}
		else
		{
			return false;
		}
	}

	const Skill& Player::useskill(int32_t skillid)
	{
		const Skill& skill = DataFactory::get().getskill(skillid);

		bool twohanded = look.getequips().istwohanded();
		string action = skill.getaction(twohanded);
		if (action == "")
		{
			look.attack(Attack::CLOSE);
			attacking = true;
		}
		else
		{
			look.setaction(action);
			attacking = true;
		}
		showeffect(skill.geteffect(twohanded));
		return skill;
	}

	Attack Player::prepareattack(bool degenerate) const
	{
		Attack attack;
		attack.mindamage = stats.getmindamage();
		attack.maxdamage = stats.getmaxdamage();
		if (degenerate)
		{
			attack.mindamage /= 10;
			attack.maxdamage /= 10;
		}
		attack.critical = stats.getcritical();
		attack.ignoredef = stats.getignoredef();
		attack.accuracy = stats.gettotal(Equipstat::ACC);
		attack.playerlevel = stats.getstat(Maplestat::LEVEL);
		attack.speed = look.getequips().getweapon()
			.mapordefault(&Weapon::getspeed, uint8_t(0));
		attack.origin = getposition();
		attack.direction = flip ? Attack::TORIGHT : Attack::TOLEFT;
		return attack;
	}

	Attack Player::prepareregularattack()
	{
		Weapon::Type weapontype = look.getequips().getweapontype();
		Attack::Type attacktype;
		bool degenerate;
		switch (weapontype)
		{
		case Weapon::BOW:
		case Weapon::CROSSBOW:
		case Weapon::CLAW:
		case Weapon::GUN:
			degenerate = !inventory.hasprojectile();
			attacktype = degenerate ? Attack::CLOSE : Attack::RANGED;
			break;
		default:
			attacktype = Attack::CLOSE;
			degenerate = false;
		}

		uint16_t delay = look.attack(degenerate);
		attacking = true;

		Attack attack = prepareattack(degenerate);
		attack.skill = 0;
		attack.mobcount = 1;
		attack.hitcount = 1;

		attack.delay = delay;
		attack.type = attacktype;

		if (attacktype == Attack::RANGED)
		{
			int16_t projectile = inventory.getprojectile();
			attack.bullet = inventory.getitem(Inventory::USE, projectile)
				.transform(DataFactory::get(), &DataFactory::getbulletdata, &Item::getid)
				.map(&BulletData::getbullet);
			attack.range = stats.getrange();
		}
		else
		{
			Optional<const Weapon> weapon = look.getequips().getweapon();
			attack.range = weapon.map(&Weapon::getrange);
			attack.hiteffect = weapon.map(&Weapon::gethiteffect);
		}

		return attack;
	}

	Attack Player::prepareskillattack(int32_t skillid) const
	{
		Attack attack = prepareattack(false);
		attack.skill = skillid;

		const Skill& skill = DataFactory::get().getskill(skillid);

		int32_t skilllevel = skillbook.getlevel(skillid);
		Optional<const SkillLevel> level = skill.getlevel(skilllevel);
		if (level)
		{
			attack.mindamage *= level->damage;
			attack.maxdamage *= level->damage;
			attack.hitcount = level->attackcount;
			attack.mobcount = level->mobcount;
			attack.critical += level->critical;
			attack.ignoredef += level->ignoredef;
			attack.range = level->range;
		}

		if (attack.range.empty())
			attack.range = look.getequips().getweapon()
			.map(&Weapon::getrange);

		attack.hiteffect = skill.gethitanimation(look.getequips().istwohanded());
		return attack;
	}

	void Player::givebuff(Buff buff)
	{
		buffs[buff.getstat()] = buff;
	}

	void Player::cancelbuff(Buffstat::Value buffstat)
	{
		buffs.erase(buffstat);
	}

	bool Player::hasbuff(Buffstat::Value buff) const
	{
		return buffs.count(buff) > 0;
	}

	void Player::setseat(const Seat* seat)
	{
		if (seat)
		{
			setposition(seat->pos.x(), seat->pos.y());
			setstate(Char::SIT);
		}
	}

	void Player::setladder(const Ladder* ldr)
	{
		ladder = ldr;

		if (ladder)
		{
			phobj.fx = ldr->x;
			phobj.hspeed = 0.0;
			phobj.vspeed = 0.0;
			phobj.setflag(PhysicsObject::NOGRAVITY);
			setstate(ldr->ladder ? Char::LADDER : Char::ROPE);
			setflip(false);
		}
	}

	void Player::setflip(bool flipped)
	{
		if (!attacking)
			Char::setflip(flipped);
	}

	void Player::setstate(State st)
	{
		if (!attacking)
			Char::setstate(st);
	}
	
	float Player::getwforce() const
	{
		return 0.05f + 0.5f * static_cast<float>(stats.gettotal(Equipstat::SPEED)) / 100;
	}

	float Player::getjforce() const
	{
		return 0.5f + 5.0f * static_cast<float>(stats.gettotal(Equipstat::JUMP)) / 100;
	}

	float Player::getclimbforce() const
	{
		return static_cast<float>(stats.gettotal(Equipstat::SPEED)) / 100;
	}

	float Player::getflyforce() const
	{
		return 0.25f;
	}

	bool Player::keydown(Keyboard::Action action) const
	{
		return keysdown.count(action) ? keysdown.at(action) : false;
	}

	CharStats& Player::getstats()
	{
		return stats;
	}

	Inventory& Player::getinvent()
	{
		return inventory;
	}

	Skillbook& Player::getskills()
	{
		return skillbook;
	}

	Questlog& Player::getquests()
	{
		return questlog;
	}

	Telerock& Player::gettrock()
	{
		return telerock;
	}

	Monsterbook& Player::getmonsterbook()
	{
		return monsterbook;
	}

	Optional<const Ladder> Player::getladder() const
	{
		return ladder;
	}
}
