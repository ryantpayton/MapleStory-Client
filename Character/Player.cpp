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
#include "IO\UI.h"
#include "IO\UITypes\UIStatsinfo.h"
#include "Net\Packets\GameplayPackets.h"
#include "Net\Packets\InventoryPackets.h"

namespace Character
{
	using Data::DataFactory;

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
		: Char(entry.cid, entry.getlook(), entry.stats.name) {

		stats = entry.stats;

		attacking = false;
		underwater = false;

		setstate(STAND);
		setflip(false);
	}

	Player::Player() 
		: Char(0, CharLook(), "") {}

	void Player::respawn(Point<int16_t> pos, bool uw)
	{
		setposition(pos.x(), pos.y());
		underwater = uw;
		keysdown.clear();
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

		using IO::UI;
		using IO::UIElement;
		using IO::UIStatsinfo;
		UI::get().withelement(UIElement::STATSINFO, &UIStatsinfo::updateall);
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
				UseItemPacket(slot, itemid).dispatch();
				break;
			}
		}
	}

	void Player::draw(uint8_t layer, Point<int16_t> viewpos, float alpha) const
	{
		if (layer == getlayer())
		{
			Char::draw(viewpos, alpha);
		}
	}

	int8_t Player::update(const Physics& physics)
	{
		const PlayerState* pst = getstate(state);
		if (pst)
		{
			pst->update(*this);
			physics.moveobject(phobj);

			bool aniend = Char::update(physics, getstancespeed());
			if (aniend && attacking)
			{
				attacking = false;
				nullstate.nextstate(*this);
			}
			else
			{
				pst->nextstate(*this);
			}
		}

		uint8_t stancebyte = flip ? state : state + 1;
		auto newmove = Movement(phobj, stancebyte);
		bool needupdate = lastmove.hasmoved(newmove) || movements.size() > 0;
		if (needupdate)
		{
			movements.push_back(newmove);
			lastmove = newmove;

			if (movements.size() > 4)
			{
				using Net::MovePlayerPacket;
				MovePlayerPacket(movements).dispatch();

				movements.clear();
			}
		}

		return getlayer();
	}

	float Player::getattackspeed() const
	{
		int8_t statsspeed = stats.getattackspeed();
		uint8_t weaponspeed = look.getequips().getweapon()
			.mapordefault(&Weapon::getspeed, uint8_t(0));

		float delay = static_cast<float>(weaponspeed + statsspeed);
		return 1.7f - (delay / 10);
	}

	void Player::setflip(bool flipped)
	{
		if (!attacking)
			Char::setflip(flipped);
	}

	void Player::setstate(State st)
	{
		if (!attacking)
		{
			Char::setstate(st);

			const PlayerState* pst = getstate(st);
			if (pst)
			{
				pst->onentry(*this);
			}
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

	bool Player::canuse(const SpecialMove& move) const
	{
		int32_t level = skillbook.getlevel(move.getid());
		Weapon::Type weapon = look.getequips().getweapontype();
		uint16_t job = stats.getstat(Maplestat::JOB);
		uint16_t hp = stats.getstat(Maplestat::HP);
		uint16_t mp = stats.getstat(Maplestat::MP);
		uint16_t bullets = inventory.getbulletcount();
		return move.canuse(level, weapon, job, hp, mp, bullets) == SpecialMove::FBR_NONE;
	}

	Attack Player::prepareattack(bool skill) const
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
			Weapon::Type weapontype = look.getequips().getweapontype();
			switch (weapontype)
			{
			case Weapon::BOW:
			case Weapon::CROSSBOW:
			case Weapon::CLAW:
			case Weapon::GUN:
				degenerate = !inventory.hasprojectile();
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
		attack.range = stats.getrange();
		attack.bullet = inventory.getbulletid();
		attack.origin = getposition();
		attack.direction = flip ? Attack::TORIGHT : Attack::TOLEFT;
		attack.speed = stats.getattackspeed() + look.getequips().getweapon()
			.mapordefault(&Weapon::getspeed, uint8_t(0));

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

	void Player::changelevel(uint16_t level)
	{
		uint16_t oldlevel = getlevel();
		if (level > oldlevel)
		{
			showeffectbyid(LEVELUP);
		}
		stats.setstat(Maplestat::LEVEL, level);
	}

	uint16_t Player::getlevel() const
	{
		return stats.getstat(Maplestat::LEVEL);
	}

	int32_t Player::getskilllevel(int32_t skillid) const
	{
		return skillbook.getlevel(skillid);
	}

	void Player::changejob(uint16_t jobid)
	{
		showeffectbyid(JOBCHANGE);
		stats.setjob(jobid);
	}

	void Player::setseat(Optional<const Seat> seat)
	{
		if (seat)
		{
			setposition(seat->getpos());
			setstate(Char::SIT);
		}
	}

	void Player::setladder(Optional<const Ladder> ldr)
	{
		ladder = ldr;

		if (ladder)
		{
			phobj.setx(ldr->getx());
			phobj.hspeed = 0.0;
			phobj.vspeed = 0.0;
			setstate(ldr->isladder() ? Char::LADDER : Char::ROPE);
			setflip(false);
		}
	}
	
	float Player::getwforce() const
	{
		return 0.05f + 0.15f * static_cast<float>(stats.gettotal(Equipstat::SPEED)) / 100;
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

	bool Player::isunderwater() const
	{
		return underwater;
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
