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
#include "Program\TimeConstants.h"

namespace Character
{
	const float WALKFORCE = 0.5f;
	const float JUMPFORCE = 5.0f;
	const float FLYFORCE = 0.25f;

	Player::Player(int32_t id, const LookEntry& lookentry, const StatsEntry& statsentry)
	{
		cid = id;
		look = CharLook(lookentry);
		stats = Charstats(statsentry);

		namelabel = Textlabel(Textlabel::DWF_14MC, Textlabel::TXC_WHITE, stats.getname(), 0);
		namelabel.setback(Textlabel::TXB_NAMETAG);

		setstance(STAND);
		setflip(false);
	}

	Player::Player() {}

	void Player::initcontrol()
	{
		plstates.add(STAND, new PlayerStandState());
		plstates.add(WALK, new PlayerWalkState());
		plstates.add(FALL, new PlayerFallState());
		plstates.add(PRONE, new PlayerProneState());
		plstates.add(SIT, new PlayerSitState());
		plstates.add(SWIM, new PlayerFlyState());
	}

	void Player::sendaction(Keyaction ka, bool down)
	{
		const PlayerState* pst = plstates.get(stance);
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
			//inventory.recalcstats();
		}

		int32_t speed = 100;
		int32_t jump = 100;

		stats.settotal(ES_SPEED, speed);
		stats.settotal(ES_JUMP, jump);
	}

	void Player::draw(vector2d<int32_t> viewpos, float inter) const
	{
		Char::draw(phobj.getposition(inter) + viewpos, inter);
	}

	int8_t Player::update(const Physics& physics)
	{
		const PlayerState* pst = plstates.get(stance);
		if (pst)
		{
			pst->update(*this);
			physics.moveobject(phobj);
			pst->nextstate(*this);
		}

		uint8_t dirstance = flip ? stance : stance + 1;
		using::Gameplay::MovementFragment;
		if (dirstance != lastmove.newstate || static_cast<int16_t>(phobj.fx) != lastmove.xpos || static_cast<int16_t>(phobj.fy) != lastmove.ypos)
		{
			movementinfo.addmovement(phobj, 0, dirstance, Constants::TIMESTEP);
			lastmove = movementinfo.gettop();
		}

		uint16_t lookts = Constants::TIMESTEP;
		switch (stance)
		{
		case WALK:
			lookts = static_cast<uint16_t>(Constants::TIMESTEP * (1.0f + abs(phobj.hspeed) / 25));
			break;
		}
		look.update(lookts);

		return phobj.fhlayer;
	}

	float Player::getwforce() const
	{
		return WALKFORCE * static_cast<float>(stats.gettotal(ES_SPEED)) / 100;
	}

	float Player::getjforce() const
	{
		return JUMPFORCE * static_cast<float>(stats.gettotal(ES_JUMP)) / 100;
	}

	float Player::getflyforce() const
	{
		return FLYFORCE;
	}

	bool Player::keydown(Keyaction ka) const
	{
		return keysdown.count(ka) ? keysdown.at(ka) : false;
	}

	rectangle2d<int32_t> Player::bounds() const
	{
		return rectangle2d<int32_t>(getposition() - vector2d<int32_t>(30, 70), getposition() + vector2d<int32_t>(30, 10));
	}

	Charstats& Player::getstats()
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
}
