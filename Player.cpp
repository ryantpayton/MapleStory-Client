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

namespace Character
{
	const float WALKFORCE = 2.0f;
	const float JUMPFORCE = 50.f;

	Player::Player(int32_t id, CharLook lk, Charstats st)
	{
		cid = id;
		look = lk;
		stats = st;
		name = Textlabel(IO::DWF_14MC, IO::TXC_WHITE, stats.getname(), 0);
		name.gettext().setback(IO::TXB_NAMETAG);
		setstance(PST_STAND);
		setflip(false);
		recalcstats(true);
	}

	Player::Player() {}

	void Player::initcontrol()
	{
		plstates.add(PST_STAND, new PlayerStandState());
		plstates.add(PST_WALK, new PlayerWalkState());
		plstates.add(PST_FALL, new PlayerFallState());
		plstates.add(PST_PRONE, new PlayerProneState());
		plstates.add(PST_SIT, new PlayerSitState());
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

	void Player::setflip(bool f)
	{
		flip = f;
		look.setflip(f);
	}

	void Player::setstance(CharStance st)
	{
		if (stance != st)
		{
			stance = st;
			look.setstance(stances[st / 2]);
		}
	}

	void Player::recalcstats(bool equipchanged)
	{
		int32_t speed = 100;
		int32_t jump = 100;

		stats.settotal(ES_SPEED, speed);
		stats.settotal(ES_JUMP, jump);
	}

	void Player::draw(vector2d<int32_t> viewpos) const
	{
		vector2d<int32_t> absp = getposition() + viewpos;
		look.draw(absp);
		name.gettext().draw(absp);
	}

	int8_t Player::update(const Physics& physics, uint16_t dpf)
	{
		const PlayerState* pst = plstates.get(stance);
		if (pst)
		{
			pst->update(*this);
			physics.moveobject(phobj, dpf);
			pst->nextstate(*this);
		}

		uint16_t lookdpf = dpf;
		switch (stance)
		{
		case PST_WALK:
			lookdpf = static_cast<short>(dpf * (1.0f + abs(phobj.hspeed) / 25));
			break;
		}
		look.update(lookdpf);

		return phobj.fhlayer;
	}

	void Player::setposition(vector2d<int32_t> pos)
	{
		phobj.fx = static_cast<float>(pos.x());
		phobj.fy = static_cast<float>(pos.y());
	}

	float Player::getwforce() const
	{
		return WALKFORCE * static_cast<float>(stats.gettotal(ES_SPEED)) / 100;
	}

	float Player::getjforce() const
	{
		return JUMPFORCE * static_cast<float>(stats.gettotal(ES_JUMP)) / 100;
	}

	bool Player::getflip() const
	{
		return flip;
	}

	CharStance Player::getstance() const
	{
		return stance;
	}

	int32_t Player::getoid() const
	{
		return cid;
	}

	int8_t Player::getlayer() const
	{
		return phobj.fhlayer;
	}

	bool Player::keydown(Keyaction ka) const
	{
		return keysdown.count(ka) ? keysdown.at(ka) : false;
	}

	vector2d<int32_t> Player::getposition() const
	{
		return vector2d<int32_t>(static_cast<int32_t>(phobj.fx), static_cast<int32_t>(phobj.fy));
	}

	rectangle2d<int32_t> Player::bounds() const
	{
		return rectangle2d<int32_t>(getposition() - vector2d<int32_t>(30, 70), getposition() + vector2d<int32_t>(30, 10));
	}

	CharLook& Player::getlook()
	{
		return look;
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

	PhysicsObject& Player::getphobj()
	{
		return phobj;
	}
}
