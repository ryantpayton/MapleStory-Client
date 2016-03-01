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
#include "CharLook.h"
#include "Data\DataFactory.h"
#include "Constants.h"

namespace Character
{
	using Data::DataFactory;

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
				addequip(equipid);
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
		setstance(Stance::STAND1);
		lastframe = 0;
		frame = 0;
		lastelapsed = 0;
		elapsed = 0;

		setexpression(Expression::DEFAULT);
		lastfcframe = 0;
		fcframe = 0;
		lastfcelapsed = 0;
		fcelapsed = 0;

		action = nullptr;
		actionstr = "";
		actframe = 0;

		flip = true;
	}

	void CharLook::draw(Point<int16_t> pos, float inter) const
	{
		if (!body || !hair || !face)
			return;

		uint16_t delay;
		if (action)
			delay = action->getdelay();
		else
			delay = DataFactory::get().getdrawinfo().getdelay(laststance, lastframe);
		uint8_t interframe;
		if (lastelapsed + Constants::TIMESTEP * inter > delay)
			interframe = frame;
		else
			interframe = lastframe;

		uint16_t fcdelay = face->getdelay(lastexpression, lastfcframe);
		uint8_t fcinterframe;
		if (lastfcelapsed + Constants::TIMESTEP * inter > fcdelay)
			fcinterframe = fcframe;
		else
			fcinterframe = lastfcframe;

		Point<int16_t> absp = pos;
		if (action)
			absp.shift(action->getmove());

		using Graphics::DrawArgument;
		DrawArgument args = DrawArgument(absp, flip);
		Point<int16_t> faceshift = DataFactory::get().getdrawinfo().getfacepos(laststance, interframe);
		DrawArgument faceargs = DrawArgument(absp + faceshift, flip, absp);

		if (Stance::isclimbing(laststance))
		{
			body->draw(laststance, Body::BODY, lastframe, args);
			equips.draw(Equipslot::GLOVES, laststance, Clothing::GLOVE, interframe, args);
			equips.draw(Equipslot::SHOES, laststance, Clothing::SHOES, interframe, args);
			equips.draw(Equipslot::PANTS, laststance, Clothing::PANTS, interframe, args);
			equips.draw(Equipslot::TOP, laststance, Clothing::TOP, interframe, args);
			equips.draw(Equipslot::TOP, laststance, Clothing::MAIL, interframe, args);
			equips.draw(Equipslot::CAPE, laststance, Clothing::CAPE, interframe, args);
			body->draw(laststance, Body::HEAD, interframe, args);
			equips.draw(Equipslot::EARRINGS, laststance, Clothing::EARRINGS, interframe, args);

			if (equips.isvisible(Equipslot::CAP))
			{
				hair->draw(laststance, Hair::BELOWCAP, interframe, args);
				equips.draw(Equipslot::CAP, laststance, Clothing::HAT, interframe, args);
			}
			else
			{
				hair->draw(laststance, Hair::BACK, interframe, args);
			}

			equips.draw(Equipslot::SHIELD, laststance, Clothing::BACKSHIELD, interframe, args);
			equips.draw(Equipslot::WEAPON, laststance, Clothing::BACKWEAPON, interframe, args);
		}
		else
		{
			hair->draw(laststance, Hair::BELOWBODY, interframe, args);
			equips.draw(Equipslot::CAPE, laststance, Clothing::CAPE, interframe, args);
			equips.draw(Equipslot::SHIELD, laststance, Clothing::SHIELDBBODY, interframe, args);
			body->draw(laststance, Body::BODY, interframe, args);
			equips.draw(Equipslot::SHOES, laststance, Clothing::SHOES, interframe, args);

			if (equips.hasoverall())
			{
				equips.draw(Equipslot::TOP, laststance, Clothing::MAIL, interframe, args);
			}
			else
			{
				equips.draw(Equipslot::PANTS, laststance, Clothing::PANTS, interframe, args);
				equips.draw(Equipslot::TOP, laststance, Clothing::TOP, interframe, args);
			}

			body->draw(laststance, Body::LEFTHAND, interframe, args);
			equips.draw(Equipslot::GLOVES, laststance, Clothing::GLOVE, interframe, args);
			hair->draw(laststance, Hair::DEFAULT, interframe, args);
			equips.draw(Equipslot::SHIELD, laststance, Clothing::SHIELDOHAIR, interframe, args);
			equips.draw(Equipslot::EARRINGS, laststance, Clothing::EARRINGS, interframe, args);
			body->draw(laststance, Body::HEAD, interframe, args);
			hair->draw(laststance, Hair::SHADE, interframe, args);
			face->draw(lastexpression, fcinterframe, faceargs);
			equips.draw(Equipslot::FACEACC, laststance, Clothing::FACEACC, 0, faceargs);
			equips.draw(Equipslot::EYEACC, laststance, Clothing::EYEACC, interframe, args);
			equips.draw(Equipslot::SHIELD, laststance, Clothing::SHIELD, interframe, args);

			if (equips.isvisible(Equipslot::CAP))
			{
				if (equips.comparelayer(Equipslot::CAP, laststance, Clothing::HATOVERHAIR))
				{
					hair->draw(laststance, Hair::OVERHEAD, interframe, args);
					equips.draw(Equipslot::CAP, laststance, Clothing::HATOVERHAIR, interframe, args);
				}
				else
				{
					equips.draw(Equipslot::CAP, laststance, Clothing::HAT, interframe, args);
				}
			}
			else
			{
				hair->draw(laststance, Hair::OVERHEAD, interframe, args);
			}

			if (equips.istwohanded())
			{
				equips.draw(Equipslot::TOP, laststance, Clothing::MAILARM, interframe, args);
				body->draw(laststance, Body::ARM, interframe, args);
				equips.draw(Equipslot::WEAPON, laststance, Clothing::WEAPON, interframe, args);
			}
			else
			{
				equips.draw(Equipslot::WEAPON, laststance, Clothing::WEAPON, interframe, args);
				body->draw(laststance, Body::ARM, interframe, args);
				equips.draw(Equipslot::TOP, laststance, Clothing::MAILARM, interframe, args);
			}

			body->draw(laststance, Body::RIGHTHAND, interframe, args);
			body->draw(laststance, Body::ARMOVERHAIR, interframe, args);
			equips.draw(Equipslot::WEAPON, laststance, Clothing::WEAPONOHAND, interframe, args);
			body->draw(laststance, Body::HANDOVER, interframe, args);
			equips.draw(Equipslot::GLOVES, laststance, Clothing::RGLOVE, interframe, args);
			equips.draw(Equipslot::WEAPON, laststance, Clothing::WEAPONOGLOVE, interframe, args);
		}
	}

	bool CharLook::update(uint16_t timestep)
	{
		if (timestep == 0)
			return false;

		lastframe = frame;
		laststance = stance;
		lastelapsed = elapsed;
		lastfcframe = fcframe;
		lastexpression = expression;
		lastfcelapsed = fcelapsed;

		elapsed += timestep;

		bool aniend = false;
		if (action == nullptr)
		{
			uint16_t delay = DataFactory::get().getdrawinfo().getdelay(stance, frame);
			if (elapsed > delay)
			{
				elapsed -= delay;
				frame = DataFactory::get().getdrawinfo().nextframe(stance, frame);

				if (frame == 0)
				{
					aniend = true;
				}
			}
		}
		else
		{
			uint16_t delay = action->getdelay();
			if (elapsed > delay)
			{
				elapsed -= delay;

				actframe = DataFactory::get().getdrawinfo().nextacframe(actionstr, actframe);
				if (actframe > 0)
				{
					action = DataFactory::get().getdrawinfo().getaction(actionstr, actframe);
					frame = action->getframe();
					stance = action->getstance();
				}
				else
				{
					aniend = true;
					action = nullptr;
					actionstr = "";
					setstance(Stance::STAND1);
				}
			}
		}

		fcelapsed += timestep;

		uint16_t fcdelay = face->getdelay(expression, fcframe);
		if (fcelapsed > fcdelay)
		{
			fcelapsed -= fcdelay;
			fcframe = face->nextframe(expression, fcframe);

			if (fcframe == 0)
			{
				if (expression == Expression::DEFAULT)
				{
					setexpression(Expression::BLINK);
				}
				else
				{
					setexpression(Expression::DEFAULT);
				}
			}
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
		Stance::Value basestance = Stance::baseof(stance);
		setstance(basestance);
	}

	void CharLook::addequip(int32_t eq)
	{
		const Clothing& equip = DataFactory::get().getclothing(eq);
		bool changestance = false;
		if (equip.geteqslot() == Equipslot::WEAPON)
		{
			const Weapon& weapon = reinterpret_cast<const Weapon&>(equip);
			changestance = weapon.istwohanded() != equips.istwohanded();
		}
		equips.addequip(equip);
		if (changestance)
			updatetwohanded();
	}

	void CharLook::removeequip(Equipslot::Value slot)
	{
		equips.removeequip(slot);
		if (slot == Equipslot::WEAPON)
			updatetwohanded();
	}

	uint16_t CharLook::attack(bool degenerate)
	{
		Optional<const Weapon> weapon = equips.getweapon();
		uint8_t attack = weapon.mapordefault(&Weapon::getattack, uint8_t(0));
		bool isaction = attack == 9 && !degenerate;
		weapon.transform(&Weapon::getsound, degenerate)
			.ifpresent(&Sound::play);
		if (isaction)
		{
			switch (attack)
			{
			case 9:
				setaction("handgun");
				break;
			}
			return action->getdelay();
		}
		else
		{
			Stance::Value newstance = getattackstance(attack, degenerate);
			if (stance != newstance)
			{
				frame = 0;
				elapsed = 0;
				stance = newstance;
			}
			return DataFactory::get().getdrawinfo().getdelay(newstance, 0);
		}
	}

	void CharLook::setstance(Stance::Value newstance)
	{
		if (action)
			return;

		if (equips.istwohanded())
			newstance = Stance::secondof(newstance);

		if (stance != newstance)
		{
			frame = 0;
			elapsed = 0;
			stance = newstance;
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
			return randomizer.nextbool() ? Stance::STABT1 : Stance::SWINGT1;
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

	void CharLook::setexpression(Expression::Value newexpression)
	{
		if (expression != newexpression)
		{
			fcframe = 0;
			fcelapsed = 0;
			expression = newexpression;
		}
	}

	void CharLook::setaction(string acstr)
	{
		if (acstr == actionstr || acstr == "")
			return;

		actframe = 0;
		elapsed = 0;
		actionstr = acstr;
		action = DataFactory::get().getdrawinfo().getaction(acstr, 0);
		if (action)
		{
			stance = action->getstance();
			frame = action->getframe();
		}
	}

	void CharLook::setflip(bool f)
	{
		flip = f;
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

	const CharEquips& CharLook::getequips() const
	{
		return equips;
	}
}