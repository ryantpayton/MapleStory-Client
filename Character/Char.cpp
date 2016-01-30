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
#include "Char.h"

namespace Character
{
	void Char::draw(const Camera& camera, float inter) const
	{
		vector2d<int16_t> absp = phobj.getposition(inter) + camera.getposition(inter);
		look.draw(absp, inter);

		for (int32_t i = 0; i < 3; i++)
		{
			if (pets[i].getiid() > 0)
				pets[i].draw(camera, inter);
		}

		namelabel.draw(absp);
		chatballoon.draw(absp);

		if (!flip)
			absp.shiftx(-8);

		effects.draw(absp, inter);
	}

	int8_t Char::update(const Physics& physics)
	{
		effects.update();
		chatballoon.update();

		for (int32_t i = 0; i < 3; i++)
		{
			if (pets[i].getiid() > 0)
			{
				switch (state)
				{
				case LADDER:
				case ROPE:
					pets[i].setstance(PetLook::HANG);
					break;
				case SWIM:
					pets[i].setstance(PetLook::FLY);
					break;
				default:
					if (pets[i].getstance() == PetLook::HANG || pets[i].getstance() == PetLook::FLY)
						pets[i].setstance(PetLook::STAND);
				}
				pets[i].update(physics, phobj.getposition(1.0f));
			}
		}

		return getlayer();
	}

	int8_t Char::getlayer() const
	{
		return isclimbing() ? 7 : phobj.fhlayer;
	}

	void Char::showeffect(Animation toshow)
	{
		effects.add(toshow);
	}

	void Char::speak(string line)
	{
		chatballoon.settext(line);
	}

	void Char::sendface(int32_t expid)
	{
		Expression::Value expression = Expression::byid(expid);
		look.setexpression(expression);
	}

	void Char::setflip(bool f)
	{
		flip = f;
		look.setflip(f);
	}

	void Char::setstate(State st)
	{
		state = st;

		// Names of the character stances used in the game's files.
		static const Stance::Value stancevalues[10] =
		{
			Stance::WALK1, Stance::STAND1, Stance::JUMP, Stance::ALERT, 
			Stance::PRONE, Stance::FLY, Stance::LADDER, Stance::ROPE,
			Stance::DEAD, Stance::SIT
		};

		int8_t index = (st / 2) - 1;
		if (index >= 0 && index < 10)
			look.setstance(stancevalues[index]);
	}

	void Char::addpet(uint8_t index, int32_t iid, string name,
		int32_t uniqueid, vector2d<int16_t> pos, uint8_t stance, int32_t fhid) {

		if (index > 2)
			return;

		pets[index] = PetLook(iid, name, uniqueid, pos, stance, fhid);
	}

	void Char::removepet(uint8_t index, bool hunger)
	{
		if (index > 2)
			return;

		pets[index] = PetLook();
		if (hunger)
		{

		}
	}

	bool Char::issitting() const
	{
		return state == SIT;
	}

	bool Char::isclimbing() const
	{
		return state == LADDER || state == ROPE;
	}

	bool Char::getflip() const
	{
		return flip;
	}

	string Char::getname() const
	{
		return namelabel.gettext();
	}

	rectangle2d<int16_t> Char::getbounds() const
	{
		return rectangle2d<int16_t>(
			getposition() - vector2d<int16_t>(30, 70),
			getposition() + vector2d<int16_t>(30, 10)
			);
	}

	CharLook& Char::getlook()
	{
		return look;
	}
}