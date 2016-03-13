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
#include "Constants.h"

#include "nlnx\nx.hpp"
#include "nlnx\node.hpp"

namespace Character
{
	Char::Char(int32_t o, CharLook lk, string name)
		: MapObject(o) {

		look = lk;

		namelabel = Text(Text::A13M, Text::CENTER, Text::WHITE);
		namelabel.setback(Text::NAMETAG);
		namelabel.settext(name);
	}

	void Char::draw(Point<int16_t> viewpos, float inter) const
	{
		Point<int16_t> absp = phobj.getposition(inter) + viewpos;
		look.draw(absp, inter);

		for (int32_t i = 0; i < 3; i++)
		{
			if (pets[i].getiid() > 0)
				pets[i].draw(viewpos, inter);
		}

		namelabel.draw(absp);
		chatballoon.draw(absp);

		if (flip)
			absp.shiftx(8);

		effects.draw(absp, inter);
	}

	bool Char::update(const Physics& physics, float speed)
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
				pets[i].update(physics, getposition());
			}
		}

		uint16_t stancespeed = static_cast<uint16_t>(Constants::TIMESTEP * speed);
		return look.update(stancespeed);
	}

	int8_t Char::update(const Physics& physics)
	{
		update(physics, 1.0f);
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

	void Char::showeffectbyid(Effect toshow)
	{
		if (effectdata.count(toshow))
		{
			Animation effect = effectdata.at(toshow);
			effects.add(effect);
		}
	}

	void Char::speak(string line)
	{
		chatballoon.settext(line);
	}

	void Char::changelook(Maplestat::Value stat, int32_t id)
	{
		switch (stat)
		{
		case Maplestat::SKIN:
			look.setbody(id);
			break;
		case Maplestat::FACE:
			look.setface(id);
			break;
		case Maplestat::HAIR:
			look.sethair(id);
			break;
		}
	}

	void Char::sendface(int32_t expid)
	{
		Expression::Value expression = Expression::byaction(expid);
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

		Stance::Value stance = Stance::bystate(state);
		look.setstance(stance);
	}

	void Char::addpet(uint8_t index, int32_t iid, string name,
		int32_t uniqueid, Point<int16_t> pos, uint8_t stance, int32_t fhid) {

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

	bool Char::istwohanded() const
	{
		return look.getequips().istwohanded();
	}

	bool Char::getflip() const
	{
		return flip;
	}

	string Char::getname() const
	{
		return namelabel.gettext();
	}

	const CharLook& Char::getlook() const
	{
		return look;
	}


	void Char::init()
	{
		using nl::node;
		node src = nl::nx::effect["BasicEff.img"];

		effectdata[LEVELUP] = src["LevelUp"];
		effectdata[JOBCHANGE] = src["JobChanged"];
		effectdata[SCROLL_SUCCESS] = src["Enchant"]["Success"];
		effectdata[SCROLL_FAILURE] = src["Enchant"]["Failure"];
	}

	map<Char::Effect, Animation> Char::effectdata;
}