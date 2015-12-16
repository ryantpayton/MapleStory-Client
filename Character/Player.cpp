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

namespace Character
{
	const PlayerState* getstate(Char::Stance stance)
	{
		static PlayerStandState standing;
		static PlayerWalkState walking;
		static PlayerFallState falling;
		static PlayerProneState lying;
		static PlayerClimbState climbing;
		static PlayerSitState sitting;
		static PlayerFlyState flying;

		switch (stance)
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
		cid = entry.cid;
		look = CharLook(entry.getlook());
		stats = CharStats(entry.stats);

		namelabel = Textlabel(Textlabel::DWF_14MC, Textlabel::TXC_WHITE, stats.getname(), 0);
		namelabel.setback(Textlabel::TXB_NAMETAG);

		attacking = false;
		setstance(STAND);
		setflip(false);
	}

	Player::Player() {}

	void Player::respawn(vector2d<int16_t> pos)
	{
		setposition(pos.x(), pos.y());
		movementinfo.clear();
		ladder = nullptr;
		attacking = false;
	}

	void Player::sendaction(IO::Keyboard::Keyaction ka, bool down)
	{
		const PlayerState* pst = getstate(stance);
		if (pst)
		{
			pst->sendaction(*this, ka, down);
		}
		keysdown[ka] = down;
	}

	void Player::clearmovement()
	{
		movementinfo.clear();
	}

	void Player::recalcstats(bool equipchanged)
	{
		if (equipchanged)
		{
			inventory.recalcstats();
		}

		int32_t speed = 100;
		int32_t jump = 100;

		stats.settotal(ES_HP, stats.getstat(MS_MAXHP) + inventory.getstat(ES_HP));
		stats.settotal(ES_MP, stats.getstat(MS_MAXMP) + inventory.getstat(ES_MP));
		stats.settotal(ES_STR, stats.getstat(MS_STR) + inventory.getstat(ES_STR));
		stats.settotal(ES_DEX, stats.getstat(MS_DEX) + inventory.getstat(ES_DEX));
		stats.settotal(ES_LUK, stats.getstat(MS_LUK) + inventory.getstat(ES_LUK));
		stats.settotal(ES_INT, stats.getstat(MS_INT) + inventory.getstat(ES_INT));
		stats.settotal(ES_SPEED, speed + inventory.getstat(ES_SPEED));
		stats.settotal(ES_JUMP, jump + inventory.getstat(ES_JUMP));
		stats.setattack(inventory.getstat(ES_WATK));

		stats.calculatedamage(look.getequips().getweapontype());
	}

	int8_t Player::update(const Physics& physics)
	{
		updatestate(physics);
		writemovement();
		updatelook();
		return isclimbing() ? 7 : phobj.fhlayer;
	}

	void Player::updatestate(const Physics& physics)
	{
		const PlayerState* pst = getstate(stance);
		if (pst)
		{
			pst->update(*this);
			physics.moveobject(phobj);
			Char::update(physics);
			pst->nextstate(*this);
		}
	}

	void Player::writemovement()
	{
		uint8_t dirstance;
		if (flip)
			dirstance = static_cast<uint8_t>(stance);
		else
			dirstance = static_cast<uint8_t>(stance + 1);

		int16_t shortx = static_cast<int16_t>(phobj.fx);
		int16_t shorty = static_cast<int16_t>(phobj.fy);
		using Gameplay::MovementFragment;
		if (dirstance != lastmove.newstate || shortx != lastmove.xpos || shorty != lastmove.ypos)
		{
			movementinfo.addmovement(phobj, 0, dirstance, Constants::TIMESTEP);
			lastmove = movementinfo.gettop();
		}
	}

	void Player::updatelook()
	{
		bool aniend = look.update(getstancespeed());
		if (aniend && attacking)
		{
			attacking = false;

			static PlayerNullState nullstate;
			nullstate.nextstate(*this);
		}
	}

	uint16_t Player::getstancespeed() const
	{
		if (attacking)
			return static_cast<uint16_t>(Constants::TIMESTEP * getattackspeed());

		switch (stance)
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

	bool Player::tryattack()
	{
		if (attacking || isclimbing() || issitting())
			return false;

		look.setstance("attack");
		attacking = true;
		return true;
	}

	float Player::getattackspeed() const
	{
		const Weapon* weapon = look.getequips().getweapon();
		if (weapon)
			return 1.7f - static_cast<float>(weapon->getspeed()) / 10;
		else
			return 0.0f;
	}

	void Player::setseat(const Seat* seat)
	{
		if (!seat)
			return;

		setposition(seat->pos.x(), seat->pos.y());
		setstance(Char::SIT);
	}

	void Player::setladder(const Ladder* ldr)
	{
		ladder = ldr;
		if (ladder)
		{
			phobj.fx = static_cast<float>(ldr->x);
			phobj.hspeed = 0.0f;
			phobj.vspeed = 0.0f;
			phobj.type = PhysicsObject::CLIMBING;
			setstance(ldr->ladder ? Char::LADDER : Char::ROPE);
			setflip(false);
		}
	}

	void Player::setflip(bool flipped)
	{
		if (!attacking)
			Char::setflip(flipped);
	}

	void Player::setstance(Stance st)
	{
		if (!attacking)
			Char::setstance(st);
	}
	
	float Player::getwforce() const
	{
		return 0.5f * static_cast<float>(stats.gettotal(ES_SPEED)) / 100;
	}

	float Player::getjforce() const
	{
		return 5.0f * static_cast<float>(stats.gettotal(ES_JUMP)) / 100;
	}

	float Player::getclimbforce() const
	{
		return 1.0f * static_cast<float>(stats.gettotal(ES_SPEED)) / 100;
	}

	float Player::getflyforce() const
	{
		return 0.25f;
	}

	bool Player::isattacking() const
	{
		return attacking;
	}

	bool Player::keydown(IO::Keyboard::Keyaction ka) const
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

	const MovementInfo& Player::getmovement() const
	{
		return movementinfo;
	}

	const Ladder* Player::getladder() const
	{
		return ladder;
	}
}
