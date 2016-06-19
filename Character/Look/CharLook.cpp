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
#include "CharLook.h"

#include "..\..\Constants.h"
#include "..\..\Data\DataFactory.h"

namespace jrc
{
	CharLook::CharLook(const LookEntry& entry)
	{
		reset();

		setbody(entry.skin);
		sethair(entry.hairid);
		setface(entry.faceid);

		for (auto& equip : entry.equips)
		{
			int32_t equipid = equip.second;
			if (equipid > 0)
			{
				add_equip(equipid);
			}
		}
	}

	CharLook::CharLook()
	{
		reset();

		body = nullptr;
		hair = nullptr;
		face = nullptr;
	}

	CharLook::~CharLook() {}

	void CharLook::reset()
	{
		flip = true;

		action = nullptr;
		actionstr = "";
		actframe = 0;

		set_stance(Stance::STAND1);
		stframe.set(0);
		stelapsed = 0;

		setexpression(Expression::DEFAULT);
		expframe.set(0);
		expelapsed = 0;
	}

	void CharLook::draw(Point<int16_t> absp, bool flipped, Stance::Value interstance,
		Expression::Value interexpression, uint8_t interframe, uint8_t interexpframe) const {

		DrawArgument args = DrawArgument(absp, flipped);
		Point<int16_t> faceshift = DataFactory::get().getdrawinfo().getfacepos(interstance, interframe);
		DrawArgument faceargs = DrawArgument(absp + faceshift, flipped, absp);

		if (Stance::is_climbing(interstance))
		{
			body->draw(interstance, Body::BODY, interframe, args);
			equips.draw(Equipslot::GLOVES, interstance, Clothing::GLOVE, interframe, args);
			equips.draw(Equipslot::SHOES, interstance, Clothing::SHOES, interframe, args);
			equips.draw(Equipslot::PANTS, interstance, Clothing::PANTS, interframe, args);
			equips.draw(Equipslot::TOP, interstance, Clothing::TOP, interframe, args);
			equips.draw(Equipslot::TOP, interstance, Clothing::MAIL, interframe, args);
			equips.draw(Equipslot::CAPE, interstance, Clothing::CAPE, interframe, args);
			body->draw(interstance, Body::HEAD, interframe, args);
			equips.draw(Equipslot::EARRINGS, interstance, Clothing::EARRINGS, interframe, args);

			switch (equips.getcaptype())
			{
			case CharEquips::NONE:
				hair->draw(interstance, Hair::BACK, interframe, args);
				break;
			case CharEquips::HEADBAND:
				equips.draw(Equipslot::CAP, interstance, Clothing::CAP, interframe, args);
				hair->draw(interstance, Hair::BACK, interframe, args);
				break;
			case CharEquips::HALFCOVER:
				hair->draw(interstance, Hair::BELOWCAP, interframe, args);
				equips.draw(Equipslot::CAP, interstance, Clothing::CAP, interframe, args);
				break;
			case CharEquips::FULLCOVER:
				equips.draw(Equipslot::CAP, interstance, Clothing::CAP, interframe, args);
				break;
			}

			equips.draw(Equipslot::SHIELD, interstance, Clothing::BACKSHIELD, interframe, args);
			equips.draw(Equipslot::WEAPON, interstance, Clothing::BACKWEAPON, interframe, args);
		}
		else
		{
			hair->draw(interstance, Hair::BELOWBODY, interframe, args);
			equips.draw(Equipslot::CAPE, interstance, Clothing::CAPE, interframe, args);
			equips.draw(Equipslot::SHIELD, interstance, Clothing::SHIELDBBODY, interframe, args);
			equips.draw(Equipslot::WEAPON, interstance, Clothing::WEAPONBBODY, interframe, args);
			equips.draw(Equipslot::CAP, interstance, Clothing::CAPBBODY, interframe, args);
			body->draw(interstance, Body::BODY, interframe, args);
			equips.draw(Equipslot::GLOVES, interstance, Clothing::WRISTOBODY, interframe, args);
			equips.draw(Equipslot::GLOVES, interstance, Clothing::GLOVEOBODY, interframe, args);
			equips.draw(Equipslot::SHOES, interstance, Clothing::SHOES, interframe, args);

			if (equips.has_overall())
			{
				equips.draw(Equipslot::TOP, interstance, Clothing::MAIL, interframe, args);
			}
			else
			{
				equips.draw(Equipslot::PANTS, interstance, Clothing::PANTS, interframe, args);
				equips.draw(Equipslot::TOP, interstance, Clothing::TOP, interframe, args);
			}

			hair->draw(interstance, Hair::DEFAULT, interframe, args);
			equips.draw(Equipslot::SHIELD, interstance, Clothing::SHIELDOHAIR, interframe, args);
			equips.draw(Equipslot::EARRINGS, interstance, Clothing::EARRINGS, interframe, args);
			body->draw(interstance, Body::HEAD, interframe, args);
			hair->draw(interstance, Hair::SHADE, interframe, args);
			face->draw(interexpression, interexpframe, faceargs);
			equips.draw(Equipslot::FACEACC, interstance, Clothing::FACEACC, 0, faceargs);
			equips.draw(Equipslot::EYEACC, interstance, Clothing::EYEACC, interframe, args);
			equips.draw(Equipslot::SHIELD, interstance, Clothing::SHIELD, interframe, args);

			switch (equips.getcaptype())
			{
			case CharEquips::NONE:
				hair->draw(interstance, Hair::OVERHEAD, interframe, args);
				break;
			case CharEquips::HEADBAND:
				equips.draw(Equipslot::CAP, interstance, Clothing::CAP, interframe, args);
				hair->draw(interstance, Hair::DEFAULT, interframe, args);
				hair->draw(interstance, Hair::OVERHEAD, interframe, args);
				equips.draw(Equipslot::CAP, interstance, Clothing::CAPOHAIR, interframe, args);
				break;
			case CharEquips::HALFCOVER:
				hair->draw(interstance, Hair::DEFAULT, interframe, args);
				equips.draw(Equipslot::CAP, interstance, Clothing::CAP, interframe, args);
				break;
			case CharEquips::FULLCOVER:
				equips.draw(Equipslot::CAP, interstance, Clothing::CAP, interframe, args);
				break;
			}

			equips.draw(Equipslot::WEAPON, interstance, Clothing::WEAPONBARM, interframe, args);
			bool twohanded = is_twohanded(interstance);
			if (twohanded)
			{
				equips.draw(Equipslot::TOP, interstance, Clothing::MAILARM, interframe, args);
				body->draw(interstance, Body::ARM, interframe, args);
				equips.draw(Equipslot::WEAPON, interstance, Clothing::WEAPON, interframe, args);
			}
			else
			{
				equips.draw(Equipslot::WEAPON, interstance, Clothing::WEAPON, interframe, args);
				body->draw(interstance, Body::ARM, interframe, args);
				equips.draw(Equipslot::TOP, interstance, Clothing::MAILARM, interframe, args);
			}
			equips.draw(Equipslot::GLOVES, interstance, Clothing::WRIST, interframe, args);
			equips.draw(Equipslot::GLOVES, interstance, Clothing::GLOVE, interframe, args);
			equips.draw(Equipslot::WEAPON, interstance, Clothing::WEAPONOGLOVE, interframe, args);

			body->draw(interstance, Body::LEFTHAND, interframe, args);
			body->draw(interstance, Body::RIGHTHAND, interframe, args);

			body->draw(interstance, Body::ARMOVERHAIR, interframe, args);
			equips.draw(Equipslot::WEAPON, interstance, Clothing::WEAPONOHAND, interframe, args);
			equips.draw(Equipslot::WEAPON, interstance, Clothing::WEAPONOBODY, interframe, args);
			body->draw(interstance, Body::HANDOVER, interframe, args);

			equips.draw(Equipslot::GLOVES, interstance, Clothing::WRISTOHAIR, interframe, args);
			equips.draw(Equipslot::GLOVES, interstance, Clothing::GLOVEOHAIR, interframe, args);
		}
	}

	void CharLook::draw(Point<int16_t> pos, float alpha) const
	{
		if (!body || !hair || !face)
			return;

		Point<int16_t> absp = pos;
		if (action)
			absp.shift(action->get_move());

		draw(absp, flip, stance.get(alpha), expression.get(alpha), stframe.get(alpha), expframe.get(alpha));
	}

	void CharLook::drawstanding(Point<int16_t> position, bool flipped) const
	{
		Stance::Value standing = equips.get_weapon()
			.mapordefault(&Weapon::getstand, Stance::STAND1);

		draw(position, flipped, standing, Expression::DEFAULT, 0, 0);
	}

	bool CharLook::update(uint16_t timestep)
	{
		if (timestep == 0)
		{
			stance.normalize();
			stframe.normalize();
			expression.normalize();
			expframe.normalize();
			return false;
		}

		bool aniend = false;
		if (action == nullptr)
		{
			uint16_t delay = get_delay(stance.get(), stframe.get());
			uint16_t delta = delay - stelapsed;
			if (timestep >= delta)
			{
				stelapsed = timestep - delta;

				uint8_t nextframe = getnextframe(stance.get(), stframe.get());
				float threshold = static_cast<float>(delta) / timestep;
				stframe.next(nextframe, threshold);

				if (stframe == 0)
				{
					aniend = true;
				}
			}
			else
			{
				stance.normalize();
				stframe.normalize();

				stelapsed += timestep;
			}
		}
		else
		{
			uint16_t delay = action->get_delay();
			uint16_t delta = delay - stelapsed;
			if (timestep >= delta)
			{
				stelapsed = timestep - delta;

				actframe = DataFactory::get().getdrawinfo().next_actionframe(actionstr, actframe);
				if (actframe > 0)
				{
					action = DataFactory::get().getdrawinfo().get_action(actionstr, actframe);
					
					float threshold = static_cast<float>(delta) / timestep;
					stance.next(action->get_stance(), threshold);
					stframe.next(action->get_frame(), threshold);
				}
				else
				{
					aniend = true;
					action = nullptr;
					actionstr = "";
					set_stance(Stance::STAND1);
				}
			}
			else
			{
				stance.normalize();
				stframe.normalize();

				stelapsed += timestep;
			}
		}

		uint16_t expdelay = face->get_delay(expression.get(), expframe.get());
		uint16_t expdelta = expdelay - expelapsed;
		if (timestep >= expdelta)
		{
			expelapsed = timestep - expdelta;

			uint8_t nextexpframe = face->nextframe(expression.get(), expframe.get());
			float fcthreshold = static_cast<float>(expdelta) / timestep;
			expframe.next(nextexpframe, fcthreshold);

			if (expframe == 0)
			{
				if (expression == Expression::DEFAULT)
				{
					expression.next(Expression::BLINK, fcthreshold);
				}
				else
				{
					expression.next(Expression::DEFAULT, fcthreshold);
				}
			}
		}
		else
		{
			expression.normalize();
			expframe.normalize();

			expelapsed += timestep;
		}

		return aniend;
	}

	void CharLook::setbody(int32_t bd)
	{
		body = &DataFactory::get().getbodytype(bd);
	}

	void CharLook::sethair(int32_t hd)
	{
		hair = &DataFactory::get().gethairstyle(hd);
	}

	void CharLook::setface(int32_t fd)
	{
		face = &DataFactory::get().getfacetype(fd);
	}

	void CharLook::updatetwohanded()
	{
		Stance::Value basestance = Stance::baseof(stance.get());
		set_stance(basestance);
	}

	void CharLook::add_equip(int32_t eq)
	{
		const Clothing& equip = DataFactory::get().get_equip(eq);
		bool changestance = false;
		if (equip.geteqslot() == Equipslot::WEAPON)
		{
			const Weapon& weapon = reinterpret_cast<const Weapon&>(equip);
			changestance = weapon.is_twohanded() != equips.is_twohanded();
		}
		equips.add_equip(equip);
		if (changestance)
			updatetwohanded();
	}

	void CharLook::remove_equip(Equipslot::Value slot)
	{
		equips.remove_equip(slot);
		if (slot == Equipslot::WEAPON)
			updatetwohanded();
	}

	void CharLook::attack(bool degenerate)
	{
		Optional<const Weapon> weapon = equips.get_weapon();
		uint8_t attacktype = weapon.mapordefault(&Weapon::getattack, uint8_t(0));

		if (attacktype == 9 && !degenerate)
		{
			stance.set(Stance::SHOT);
			set_action("handgun");
		}
		else
		{
			stance.set(getattackstance(attacktype, degenerate));
			stframe.set(0);
			stelapsed = 0;
		}

		weapon.map(&Weapon::getusesound, degenerate).play();
	}

	void CharLook::attack(Stance::Value newstance)
	{
		if (action || newstance == Stance::NONE)
			return;

		switch (newstance)
		{
		case Stance::SHOT:
			set_action("handgun");
			break;
		default:
			set_stance(newstance);
		}
	}

	void CharLook::set_stance(Stance::Value newstance)
	{
		if (action || newstance == Stance::NONE)
			return;

		Optional<const Weapon> weapon = equips.get_weapon();
		if (weapon)
		{
			switch (newstance)
			{
			case Stance::STAND1:
			case Stance::STAND2:
				newstance = weapon->getstand();
				break;
			case Stance::WALK1:
			case Stance::WALK2:
				newstance = weapon->getwalk();
				break;
			}
		}

		if (stance != newstance)
		{
			stance.set(newstance);
			stframe.set(0);

			stelapsed = 0;
		}
	}

	Stance::Value CharLook::getattackstance(uint8_t attack, bool degenerate) const
	{
		if (stance == Stance::PRONE)
			return Stance::PRONESTAB;

		bool random = randomizer.nextbool();
		switch (attack)
		{
		case 1:
			return random ? Stance::STABO1 : Stance::SWINGO1;
		case 2:
			return random ? Stance::STABT1 : Stance::SWINGP1;
		case 3:
			if (degenerate)
				return random ? Stance::SWINGT1 : Stance::SWINGT3;
			else
				return Stance::SHOOT1;
		case 4:
			if (degenerate)
				return random ? Stance::SWINGT1 : Stance::STABT1;
			else
				return Stance::SHOOT2;
		case 5:
			return randomizer.nextbool() ? Stance::STABO1 : Stance::SWINGT1;
		case 6:
			return randomizer.nextbool() ? Stance::SWINGO1 : Stance::SWINGO2;
		case 7:
			if (degenerate)
				return random ? Stance::SWINGT1 : Stance::STABT1;
			else
				return random ? Stance::SWINGO1 : Stance::SWINGO2;
		case 9:
			if (degenerate)
				return random ? Stance::SWINGP1 : Stance::STABT2;
			else
				return Stance::STAND1;
		default:
			return Stance::STAND1;
		}
	}

	uint16_t CharLook::get_delay(Stance::Value st, uint8_t fr) const
	{
		return DataFactory::get().getdrawinfo().get_delay(st, fr);
	}

	uint8_t CharLook::getnextframe(Stance::Value st, uint8_t fr) const
	{
		return DataFactory::get().getdrawinfo().nextframe(st, fr);
	}

	void CharLook::setexpression(Expression::Value newexpression)
	{
		if (expression != newexpression)
		{
			expression.set(newexpression);
			expframe.set(0);

			expelapsed = 0;
		}
	}

	void CharLook::set_action(const std::string& acstr)
	{
		if (acstr == actionstr || acstr == "")
			return;

		if (Stance::Value ac_stance = Stance::bystring(acstr))
		{
			set_stance(ac_stance);
		}
		else
		{
			action = DataFactory::get().getdrawinfo().get_action(acstr, 0);
			if (action)
			{
				actframe = 0;
				stelapsed = 0;
				actionstr = acstr;

				stance.set(action->get_stance());
				stframe.set(action->get_frame());
			}
		}
	}

	void CharLook::set_direction(bool f)
	{
		flip = f;
	}

	bool CharLook::is_twohanded(Stance::Value st) const
	{
		switch (st)
		{
		case Stance::STAND1:
		case Stance::WALK1:
			return false;
		case Stance::STAND2:
		case Stance::WALK2:
			return true;
		default:
			return equips.is_twohanded();
		}
	}

	uint16_t CharLook::get_attackdelay(size_t no, uint8_t first_frame) const
	{
		if (action)
		{
			return DataFactory::get()
				.getdrawinfo()
				.get_attackdelay(actionstr, no);
		}
		else
		{
			uint16_t delay = 0;
			for (uint8_t frame = 0; frame < first_frame; frame++)
			{
				delay += get_delay(stance.get(), frame);
			}
			return delay;
		}
	}

	uint8_t CharLook::get_frame() const
	{
		return stframe.get();
	}

	Stance::Value CharLook::get_stance() const
	{
		return stance.get();
	}

	const Body* CharLook::getbodytype() const
	{
		return body;
	}

	const Hair* CharLook::gethairstyle() const
	{
		return hair;
	}

	const Face* CharLook::getfacetype() const
	{
		return face;
	}

	const CharEquips& CharLook::get_equips() const
	{
		return equips;
	}
}