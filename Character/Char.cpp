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
#include "Char.h"

#include "..\Constants.h"
#include "..\Util\Misc.h"

#include <nlnx\nx.hpp>
#include <nlnx\node.hpp>

namespace jrc
{
	Char::Char(int32_t o, const CharLook& lk, const std::string& name)
		: MapObject(o), look(lk), namelabel({ Text::A13M, Text::CENTER, Text::WHITE, Text::NAMETAG, name }) {}

	void Char::draw(double viewx, double viewy, float alpha) const
	{
		Point<int16_t> absp = phobj.get_absolute(viewx, viewy, alpha);

		effects.drawbelow(absp, alpha);
		look.draw(absp, alpha);
		afterimage.draw(look.get_frame(), { absp, flip }, alpha);

		for (int32_t i = 0; i < 3; i++)
		{
			if (pets[i].get_itemid() > 0)
				pets[i].draw(viewx, viewy, alpha);
		}

		namelabel.draw(absp);
		chatballoon.draw(absp - Point<int16_t>(0, 85));

		effects.drawabove(absp, alpha);
	}

	bool Char::update(const Physics& physics, float speed)
	{
		effects.update();
		chatballoon.update();

		for (int32_t i = 0; i < 3; i++)
		{
			if (pets[i].get_itemid() > 0)
			{
				switch (state)
				{
				case LADDER:
				case ROPE:
					pets[i].set_stance(PetLook::HANG);
					break;
				case SWIM:
					pets[i].set_stance(PetLook::FLY);
					break;
				default:
					if (pets[i].get_stance() == PetLook::HANG || pets[i].get_stance() == PetLook::FLY)
						pets[i].set_stance(PetLook::STAND);
				}
				pets[i].update(physics, get_position());
			}
		}

		uint16_t stancespeed = 0;
		if (speed >= 1.0f / Constants::TIMESTEP)
		{
			stancespeed = static_cast<uint16_t>(Constants::TIMESTEP * speed);
		}
		afterimage.update(look.get_frame(), stancespeed);
		return look.update(stancespeed);
	}

	float Char::get_stancespeed() const
	{
		if (attacking)
			return get_real_attackspeed();

		switch (state)
		{
		case WALK:
			return static_cast<float>(std::abs(phobj.hspeed) / 1.25);
		case LADDER:
		case ROPE:
			return static_cast<float>(std::abs(phobj.vspeed));
		default:
			return 1.0f;
		}
	}

	int8_t Char::get_integer_attackspeed() const
	{
		int8_t base_speed = get_base_attackspeed();
		int8_t weapon_speed = look.get_equips().get_attackspeed();
		return base_speed + weapon_speed;
	}

	float Char::get_real_attackspeed() const
	{
		int8_t speed = get_integer_attackspeed();
		return 1.7f - static_cast<float>(speed) / 10;
	}

	uint16_t Char::get_attackdelay(size_t no) const
	{
		uint8_t first_frame = afterimage.get_first_frame();
		uint16_t delay = look.get_attackdelay(no, first_frame);
		float fspeed = get_real_attackspeed();
		return static_cast<uint16_t>(delay / fspeed);
	}

	int8_t Char::update(const Physics& physics)
	{
		update(physics, 1.0f);
		return get_layer();
	}

	int8_t Char::get_layer() const
	{
		return is_climbing() ? 7 : phobj.fhlayer;
	}

	void Char::show_effect(Animation toshow, int8_t z)
	{
		float attackspeed = get_real_attackspeed();
		effects.add(toshow, flip, z, attackspeed);
	}

	void Char::show_effect_id(Effect toshow)
	{
		Animation effect = effectdata[toshow];
		effects.add(effect);
	}

	void Char::speak(const std::string& line)
	{
		chatballoon.change_text(line);
	}

	void Char::change_look(Maplestat::Value stat, int32_t id)
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

	void Char::set_state(uint8_t statebyte)
	{
		if (statebyte % 2 == 1)
		{
			set_direction(false);
			statebyte -= 1;
		}
		else
		{
			set_direction(true);
		}

		Char::State newstate = byvalue(statebyte);
		set_state(newstate);
	}

	void Char::set_expression(int32_t expid)
	{
		Expression::Value expression = Expression::byaction(expid);
		look.setexpression(expression);
	}

	void Char::attack(const std::string& action)
	{
		look.set_action(action);

		attacking = true;
	}

	void Char::attack(Stance::Value stance)
	{
		look.attack(stance);

		attacking = true;
	}

	void Char::attack(bool degenerate)
	{
		look.attack(degenerate);

		attacking = true;
	}

	void Char::set_afterimage(int32_t skill_id)
	{
		auto weapon = look.get_equips().get_weapon();
		if (!weapon)
			return;

		std::string stance_name = Stance::nameof(look.get_stance());
		int16_t weapon_level = weapon->getreqstat(Maplestat::LEVEL);
		const std::string& ai_name = weapon->get_afterimage();
		afterimage = { skill_id, ai_name, stance_name, weapon_level };
	}

	const Afterimage& Char::get_afterimage() const
	{
		return afterimage;
	}

	void Char::set_direction(bool f)
	{
		flip = f;
		look.set_direction(f);
	}

	void Char::set_state(State st)
	{
		state = st;

		Stance::Value stance = Stance::bystate(state);
		look.set_stance(stance);
	}

	void Char::add_pet(uint8_t index, int32_t iid, const std::string& name,
		int32_t uniqueid, Point<int16_t> pos, uint8_t stance, int32_t fhid) {

		if (index > 2)
			return;

		pets[index] = PetLook(iid, name, uniqueid, pos, stance, fhid);
	}

	void Char::remove_pet(uint8_t index, bool hunger)
	{
		if (index > 2)
			return;

		pets[index] = PetLook();
		if (hunger)
		{

		}
	}

	bool Char::is_sitting() const
	{
		return state == SIT;
	}

	bool Char::is_climbing() const
	{
		return state == LADDER || state == ROPE;
	}

	bool Char::is_twohanded() const
	{
		return look.get_equips().is_twohanded();
	}

	bool Char::getflip() const
	{
		return flip;
	}

	std::string Char::get_name() const
	{
		return namelabel.get_text();
	}

	CharLook& Char::get_look()
	{
		return look;
	}

	const CharLook& Char::get_look() const
	{
		return look;
	}

	void Char::init()
	{
		nl::node src = nl::nx::effect["BasicEff.img"];

		effectdata[LEVELUP] = src["LevelUp"];
		effectdata[JOBCHANGE] = src["JobChanged"];
		effectdata[SCROLL_SUCCESS] = src["Enchant"]["Success"];
		effectdata[SCROLL_FAILURE] = src["Enchant"]["Failure"];
		effectdata[MONSTER_CARD] = src["MonsterBook"]["cardGet"];
	}
	EnumMap<Char::Effect, Animation> Char::effectdata;
}