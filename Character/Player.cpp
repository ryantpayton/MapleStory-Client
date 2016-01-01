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
#include "Net\Session.h"
#include "Net\Packets\GameplayPackets83.h"
#include "Net\Packets\InventoryPackets.h"

namespace Character
{
	static PlayerNullState nullstate;

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
		oid = entry.cid;
		look = CharLook(entry.getlook());
		stats = CharStats(entry.stats);

		namelabel = Textlabel(Textlabel::DWF_14MC, Textlabel::TXC_WHITE, stats.getname(), 0);
		namelabel.setback(Textlabel::TXB_NAMETAG);

		sendcd = Constants::TIMESTEP;
		active = true;
		attacking = false;
		ladder = nullptr;

		setstance(STAND);
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
		const PlayerState* pst = getstate(stance);
		if (pst)
		{
			pst->sendaction(*this, ka, down);
		}
		keysdown[ka] = down;
	}

	void Player::recalcstats(bool equipchanged)
	{
		if (equipchanged)
		{
			inventory.recalcstats();
		}

		int32_t maxhp = stats.getstat(MS_MAXHP) + inventory.getstat(ES_HP);
		int32_t maxmp = stats.getstat(MS_MAXMP) + inventory.getstat(ES_MP);

		for (auto& buff : buffs)
		{
			float floatvalue = static_cast<float>(buff.second.value) / 100;

			switch (buff.first)
			{
			case HYPERBODYHP:
				maxhp += static_cast<int32_t>(maxhp * floatvalue);
				break;
			case HYPERBODYMP:
				maxmp += static_cast<int32_t>(maxmp * floatvalue);
				break;
			}
		}

		stats.settotal(ES_HP, maxhp);
		stats.settotal(ES_MP, maxmp);
		stats.settotal(ES_STR, stats.getstat(MS_STR) + inventory.getstat(ES_STR));
		stats.settotal(ES_DEX, stats.getstat(MS_DEX) + inventory.getstat(ES_DEX));
		stats.settotal(ES_LUK, stats.getstat(MS_LUK) + inventory.getstat(ES_LUK));
		stats.settotal(ES_INT, stats.getstat(MS_INT) + inventory.getstat(ES_INT));
		stats.settotal(ES_SPEED, 100 + inventory.getstat(ES_SPEED));
		stats.settotal(ES_JUMP, 100 + inventory.getstat(ES_JUMP));
		stats.setattack(inventory.getstat(ES_WATK));

		stats.calculatedamage(look.getequips().getweapontype());
	}

	void Player::changecloth(int16_t slot)
	{
		const Equip* equip = inventory.getequip(Inventory::EQUIPPED, slot);

		if (equip)
			look.addequip(equip->getid());
		else
			look.removeequip(static_cast<Equipslot>(slot));

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
				using Net::UseItemPacket;
				Net::Session::dispatch(UseItemPacket(slot, itemid));
				break;
			}
		}
	}

	int8_t Player::update(const Physics& physics)
	{
		const PlayerState* pst = getstate(stance);
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
			uint8_t dirstance;
			if (flip)
				dirstance = static_cast<uint8_t>(stance);
			else
				dirstance = static_cast<uint8_t>(stance + 1);

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
					using Net::MovePlayerPacket83;
					Net::Session::dispatch(MovePlayerPacket83(movements));
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

	bool Player::isattacking() const
	{
		return attacking;
	}

	bool Player::canattack() const
	{
		return !attacking && !isclimbing() && !issitting() && look.getequips().hasweapon();
	}

	void Player::useattack(string action)
	{
		if (action == "")
			look.setstance("attack");
		else
			look.setaction(action);

		attacking = true;
	}

	Attack Player::prepareattack()
	{
		Attack attack;
		attack.mindamage = stats.getmindamage();
		attack.maxdamage = stats.getmaxdamage();
		attack.critical = stats.getcritical();
		attack.ignoredef = stats.getignoredef();
		attack.accuracy = stats.gettotal(Character::ES_ACC);
		attack.playerlevel = stats.getstat(Character::MS_LEVEL);
		attack.speed = look.getequips().getweapon()->getspeed();
		attack.origin = getposition();
		attack.direction = flip ? Attack::TORIGHT : Attack::TOLEFT;
		return attack;
	}

	Attack Player::regularattack()
	{
		useattack("");

		Attack attack = prepareattack();
		attack.skill = 0;
		attack.mobcount = 1;
		attack.hitcount = 1;
		attack.delay = look.getequips().getweapon()->getattackdelay();
		attack.range = look.getequips().getweapon()->getrange();
		attack.hiteffect = look.getequips().getweapon()->gethiteffect();
		return attack;
	}

	void Player::givebuff(Buff buff)
	{
		buffs[buff.stat] = buff;
	}

	void Player::cancelbuff(Buffstat buffstat)
	{
		buffs.erase(buffstat);
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
			phobj.fx = static_cast<double>(ldr->x);
			phobj.hspeed = 0.0;
			phobj.vspeed = 0.0;
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
