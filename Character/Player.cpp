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
#include "Player.h"
#include "PlayerStates.h"
#include "Program\Constants.h"
#include "Data\DataFactory.h"
#include "Net\Session.h"
#include "Net\Packets\GameplayPackets.h"
#include "Net\Packets\InventoryPackets.h"

namespace Character
{
	static PlayerNullState nullstate;

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
		look = CharLook(entry.getlook());
		stats = CharStats(entry.stats);

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

	void Player::respawn(vector2d<int16_t> pos)
	{
		setposition(pos.x(), pos.y());
		sendcd = Constants::TIMESTEP;
		attacking = false;
		ladder = nullptr;
		nullstate.nextstate(*this);
	}

	void Player::sendaction(Keyboard::Keyaction ka, bool down)
	{
		const PlayerState* pst = getstate(state);
		if (pst)
		{
			pst->sendaction(*this, ka, down);
		}
		keysdown[ka] = down;
	}

	void Player::recalcstats(bool equipchanged)
	{
		stats.inittotalstats();

		if (equipchanged)
			inventory.recalcstats();

		inventory.addtotalsto(stats);

		for (auto& buff : buffs)
		{
			buff.second.applyto(stats);
		}

		stats.calculatedamage(look.getequips().getweapontype());
	}

	void Player::changecloth(int16_t slot)
	{
		const Equip* equip = inventory.getequip(Inventory::EQUIPPED, slot);

		if (equip)
			look.addequip(equip->getid());
		else
			look.removeequip(Slot::byid(slot));

		recalcstats(true);
	}

	void Player::useitem(int32_t itemid)
	{
		Inventory::InvType type = inventory.gettypebyid(itemid);
		int16_t slot = inventory.finditem(type, itemid);

		if (slot >= 0)
		{
			switch (type)
			{
			case Inventory::USE:
				using Net::Session;
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
			uint8_t dirstance = flip ? state : state + 1;
			int16_t shortx = static_cast<int16_t>(phobj.fx);
			int16_t shorty = static_cast<int16_t>(phobj.fy);
			if (dirstance != lastmove.newstate || shortx != lastmove.xpos || shorty != lastmove.ypos || movements.size() > 0)
			{
				lastmove.type = MovementFragment::MVT_ABSOLUTE;
				lastmove.command = 0;
				lastmove.xpos = static_cast<int16_t>(phobj.fx);
				lastmove.ypos = static_cast<int16_t>(phobj.fy);
				lastmove.lastx = static_cast<int16_t>(phobj.lastx);
				lastmove.lasty = static_cast<int16_t>(phobj.lasty);
				lastmove.fh = phobj.fhid;
				lastmove.newstate = dirstance;
				lastmove.duration = Constants::TIMESTEP;
				movements.push_back(lastmove);

				if (movements.size() > 4)
				{
					using Net::Session;
					using Net::MovePlayerPacket;
					Session::get().dispatch(MovePlayerPacket(movements));
					movements.clear();
				}
			}
		}
		
		bool aniend = look.update(getstancespeed());
		if (aniend && attacking)
		{
			attacking = false;
			nullstate.nextstate(*this);
		}

		return getlayer();
	}

	uint16_t Player::getstancespeed() const
	{
		if (attacking)
			return static_cast<uint16_t>(Constants::TIMESTEP * getattackspeed());

		switch (state)
		{
		case WALK:
			return static_cast<uint16_t>(Constants::TIMESTEP * (1.0f + abs(phobj.hspeed) / 25));
		case LADDER:
		case ROPE:
			return static_cast<uint16_t>(Constants::TIMESTEP * abs(phobj.vspeed));
		default:
			return Constants::TIMESTEP;
		}
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
		const SkillLevel* level = skillbook.getlevelof(skillid);
		if (level == nullptr)
			return false;

		if (level->hpcost >= stats.getstat(Maplestat::HP))
			return false;

		if (level->mpcost > stats.getstat(Maplestat::MP))
			return false;

		return canattack();
	}

	const Skill& Player::useskill(int32_t skillid)
	{
		using Data::DataFactory;
		const Skill& skill = DataFactory::get().getskill(skillid);
		bool twohanded = look.getequips().istwohanded();
		string action = skill.getaction(twohanded);
		if (action == "")
		{
			useattack();
		}
		else
		{
			look.setaction(action);
			attacking = true;
		}
		showeffect(skill.geteffect(twohanded));
		return skill;
	}

	void Player::useattack()
	{
		look.attack();
		look.getequips().getweapon()->playsound();
		attacking = true;
	}

	Attack Player::prepareattack() const
	{
		Attack attack;
		attack.mindamage = stats.getmindamage();
		attack.maxdamage = stats.getmaxdamage();
		attack.critical = stats.getcritical();
		attack.ignoredef = stats.getignoredef();
		attack.accuracy = stats.gettotal(Equipstat::ACC);
		attack.playerlevel = stats.getstat(Maplestat::LEVEL);
		attack.speed = look.getequips().getweapon()->getspeed();
		attack.origin = getposition();
		attack.direction = flip ? Attack::TORIGHT : Attack::TOLEFT;
		return attack;
	}

	Attack Player::regularattack()
	{
		useattack();

		Attack attack = prepareattack();
		attack.skill = 0;
		attack.mobcount = 1;
		attack.hitcount = 1;
		attack.delay = look.getequips().getweapon()->getattackdelay();
		attack.range = look.getequips().getweapon()->getrange();
		attack.hiteffect = look.getequips().getweapon()->gethiteffect();
		return attack;
	}

	Attack Player::prepareskillattack(int32_t skillid) const
	{
		const SkillLevel* level = skillbook.getlevelof(skillid);

		Attack attack = prepareattack();
		attack.skill = skillid;
		attack.hitcount = level->attackcount;
		attack.mobcount = level->mobcount;
		attack.critical += level->critical;
		attack.ignoredef += level->ignoredef;
		attack.range = level->range;
		if (attack.range.empty())
			attack.range = look.getequips().getweapon()->getrange();
		attack.mindamage = static_cast<int32_t>(attack.mindamage * level->damage);
		attack.maxdamage = static_cast<int32_t>(attack.maxdamage * level->damage);
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

	float Player::getattackspeed() const
	{
		const Weapon* weapon = look.getequips().getweapon();
		if (weapon)
		{
			float delay = static_cast<float>(weapon->getspeed() + stats.getattackspeed());
			return 1.7f - (delay / 10);
		}
		else
		{
			return 0.0f;
		}
	}

	void Player::setseat(const Seat* seat)
	{
		if (!seat)
			return;

		setposition(seat->pos.x(), seat->pos.y());
		setstate(Char::SIT);
	}

	void Player::setladder(const Ladder* ldr)
	{
		ladder = ldr;

		if (ladder)
		{
			phobj.fx = static_cast<double>(ldr->x);
			phobj.hspeed = 0.0;
			phobj.vspeed = 0.0;
			phobj.type = PhysicsObject::CLIMBING;
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
		return 0.5f * static_cast<float>(stats.gettotal(Equipstat::SPEED)) / 100;
	}

	float Player::getjforce() const
	{
		return 5.0f * static_cast<float>(stats.gettotal(Equipstat::JUMP)) / 100;
	}

	float Player::getclimbforce() const
	{
		return 1.0f * static_cast<float>(stats.gettotal(Equipstat::SPEED)) / 100;
	}

	float Player::getflyforce() const
	{
		return 0.25f;
	}

	bool Player::keydown(Keyboard::Keyaction ka) const
	{
		return keysdown.count(ka) ? keysdown.at(ka) : false;
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

	const Ladder* Player::getladder() const
	{
		return ladder;
	}
}
