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
#include "Program\Constants.h"

namespace Character
{
	CharLook::CharLook(const LookEntry& entry)
	{
		reset();

		setbody(entry.skin);
		sethair(entry.hairid);
		setface(entry.faceid);

		for (Equipslot e = EQL_CAP; e <= EQL_WEAPON; e = static_cast<Equipslot>(e + 1))
		{
			int8_t val = static_cast<int8_t>(e);
			if (entry.equips.count(val))
			{
				int32_t equipid = entry.equips.at(val);
				if (equipid > 0)
				{
					addequip(equipid);
				}
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
		setstance("stand");
		lastframe = 0;
		frame = 0;
		lastelapsed = 0;
		elapsed = 0;

		setexpression("default");
		lastfcframe = 0;
		fcframe = 0;
		lastfcelapsed = 0;
		fcelapsed = 0;

		action = nullptr;
		actionstr = "";
		actframe = 0;

		flip = true;
	}

	void CharLook::draw(vector2d<int16_t> pos, float inter) const
	{
		if (!body || !hair || !face)
			return;

		uint16_t delay;
		if (action)
			delay = action->getdelay();
		else
			delay = Data::getdrawinfo().getdelay(laststance, lastframe);
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

		using Graphics::DrawArgument;
		DrawArgument args = DrawArgument(pos, flip);
		vector2d<int16_t> faceshift = Data::getdrawinfo().getfacepos(laststance, interframe);
		DrawArgument faceargs = DrawArgument(pos + faceshift, flip, pos);

		if (laststance == "ladder" || laststance == "rope")
		{
			body->draw(laststance, CL_BODY, lastframe, args);
			equips.draw(EQL_GLOVES, laststance, CL_GLOVE, interframe, args);
			equips.draw(EQL_SHOES, laststance, CL_SHOES, interframe, args);
			equips.draw(EQL_PANTS, laststance, CL_PANTS, interframe, args);
			equips.draw(EQL_TOP, laststance, CL_TOP, interframe, args);
			equips.draw(EQL_TOP, laststance, CL_MAIL, interframe, args);
			equips.draw(EQL_CAPE, laststance, CL_CAPE, interframe, args);
			body->draw(laststance, CL_HEAD, interframe, args);
			equips.draw(EQL_EARRINGS, laststance, CL_EARRINGS, interframe, args);

			if (equips.isvisible(EQL_CAP))
			{
				hair->draw(laststance, CL_BACKHAIRBCAP, interframe, args);
				equips.draw(EQL_CAP, laststance, CL_HAT, interframe, args);
			}
			else
			{
				hair->draw(laststance, CL_BACKHAIR, interframe, args);
			}

			equips.draw(EQL_SHIELD, laststance, CL_BACKSHIELD, interframe, args);
			equips.draw(EQL_WEAPON, laststance, CL_BACKWEAPON, interframe, args);
		}
		else
		{
			hair->draw(laststance, CL_HAIRBBODY, interframe, args);
			equips.draw(EQL_CAPE, laststance, CL_CAPE, interframe, args);
			equips.draw(EQL_SHIELD, laststance, CL_SHIELDBBODY, interframe, args);
			body->draw(laststance, CL_BODY, interframe, args);
			equips.draw(EQL_SHOES, laststance, CL_SHOES, interframe, args);

			if (equips.hasoverall())
			{
				equips.draw(EQL_TOP, laststance, CL_MAIL, interframe, args);
			}
			else
			{
				equips.draw(EQL_PANTS, laststance, CL_PANTS, interframe, args);
				equips.draw(EQL_TOP, laststance, CL_TOP, interframe, args);
			}

			body->draw(laststance, CL_LHAND, interframe, args);
			equips.draw(EQL_GLOVES, laststance, CL_GLOVE, interframe, args);
			hair->draw(laststance, CL_HAIR, interframe, args);
			equips.draw(EQL_SHIELD, laststance, CL_SHIELDOHAIR, interframe, args);
			equips.draw(EQL_EARRINGS, laststance, CL_EARRINGS, interframe, args);
			body->draw(laststance, CL_HEAD, interframe, args);
			hair->draw(laststance, CL_HAIRSHADE, interframe, args);
			face->draw(lastexpression, CL_FACE, fcinterframe, faceargs);
			equips.draw(EQL_FACEACC, "blink", CL_FACEACC, 0, faceargs);
			equips.draw(EQL_EYEACC, laststance, CL_EYEACC, interframe, args);
			equips.draw(EQL_SHIELD, laststance, CL_SHIELD, interframe, args);

			if (equips.isvisible(EQL_CAP))
			{
				if (equips.getequip(EQL_CAP).islayer(laststance, CL_HATOVERHAIR))
				{
					hair->draw(laststance, CL_HAIROHEAD, interframe, args);
					equips.draw(EQL_CAP, laststance, CL_HATOVERHAIR, interframe, args);
				}
				else
				{
					equips.draw(EQL_CAP, laststance, CL_HAT, interframe, args);
				}
			}
			else
			{
				hair->draw(laststance, CL_HAIROHEAD, interframe, args);
			}

			if (equips.istwohanded())
			{
				equips.draw(EQL_TOP, laststance, CL_MAILARM, interframe, args);
				body->draw(laststance, CL_ARM, interframe, args);
				equips.draw(EQL_WEAPON, laststance, CL_WEAPON, interframe, args);
			}
			else
			{
				equips.draw(EQL_WEAPON, laststance, CL_WEAPON, interframe, args);
				body->draw(laststance, CL_ARM, interframe, args);
				equips.draw(EQL_TOP, laststance, CL_MAILARM, interframe, args);
			}

			body->draw(laststance, CL_RHAND, interframe, args);
			body->draw(laststance, CL_ARMOHAIR, interframe, args);
			equips.draw(EQL_WEAPON, laststance, CL_WEAPONOHAND, interframe, args);
			body->draw(laststance, CL_HANDOWEP, interframe, args);
			equips.draw(EQL_GLOVES, laststance, CL_RGLOVE, interframe, args);
			equips.draw(EQL_WEAPON, laststance, CL_WEAPONOGLOVE, interframe, args);
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
			uint16_t delay = Data::getdrawinfo().getdelay(stance, frame);
			if (elapsed > delay)
			{
				elapsed -= delay;
				frame = Data::getdrawinfo().nextframe(stance, frame);

				if (frame == 0)
				{
					aniend = true;

					if (stance == "attack")
					{
						setstance("stand");
					}
				}
			}
		}
		else
		{
			uint16_t delay = action->getdelay();
			if (elapsed > delay)
			{
				elapsed -= delay;

				actframe = Data::getdrawinfo().nextacframe(actionstr, actframe);
				if (actframe > 0)
				{
					action = Data::getdrawinfo().getaction(actionstr, actframe);
					frame = action->getframe();
					stance = action->getstance();
				}
				else
				{
					aniend = true;
					action = nullptr;
					actionstr = "";
					setstance("stand");
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
				if (expression == "default")
				{
					setexpression("blink");
				}
				else
				{
					setexpression("default");
				}
			}
		}

		return aniend;
	}

	void CharLook::setbody(uint8_t bd)
	{
		body = &Data::getbodytype(bd);
	}

	void CharLook::sethair(int32_t hd)
	{
		hair = &Data::gethairstyle(hd);
	}

	void CharLook::setface(int32_t fd)
	{
		face = &Data::getfacetype(fd);
	}

	void CharLook::updatetwohanded()
	{
		if (stance == "stand1" || stance == "stand2")
		{
			setstance("stand");
		}
		else if (stance == "walk1" || stance == "walk2")
		{
			setstance("walk");
		}
	}

	void CharLook::addequip(int32_t eq)
	{
		const Clothing& equip = Data::getclothing(eq);
		bool changestance = false;
		if (equip.geteqslot() == EQL_WEAPON)
		{
			const Weapon& weapon = reinterpret_cast<const Weapon&>(equip);
			changestance = weapon.istwohanded() != equips.istwohanded();
		}
		equips.addequip(equip);
		if (changestance)
			updatetwohanded();
	}

	void CharLook::removeequip(Equipslot slot)
	{
		equips.removeequip(slot);
		if (slot == EQL_WEAPON)
			updatetwohanded();
	}

	void CharLook::setstance(string newstance)
	{
		if (action)
			return;

		if (newstance == "attack")
			newstance = getattackstance();
		else if (newstance == "stand" || newstance == "walk")
			newstance.push_back(equips.istwohanded() ? '2' : '1');

		if (newstance == stance)
			return;

		if (newstance != "")
		{
			frame = 0;
			elapsed = 0;
			stance = newstance;
		}
	}

	string CharLook::getattackstance() const
	{
		const Weapon* weapon = equips.getweapon();
		if (weapon == nullptr)
			return "";

		if (stance == "prone")
			return "proneStab";

		uint8_t attack = weapon->getattack();
		switch (attack)
		{
		case 1:
			return randomizer.nextbool() ? "stabO1" : "swingO1";
		case 2:
			return randomizer.nextbool() ? "stabT1" : "swingP1";
		case 5:
			return randomizer.nextbool() ? "stabT1" : "swingT1";
		default:
			return "";
		}
	}

	void CharLook::setexpression(string exstr)
	{
		if (exstr == expression)
			return;
		
		fcframe = 0;
		fcelapsed = 0;
		expression = exstr;
	}

	void CharLook::setaction(string acstr)
	{
		if (acstr == actionstr || acstr == "")
			return;

		actframe = 0;
		elapsed = 0;
		actionstr = acstr;
		action = Data::getdrawinfo().getaction(acstr, 0);
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

	const Bodytype* CharLook::getbodytype() const
	{
		return body;
	}

	const Hairstyle* CharLook::gethairstyle() const
	{
		return hair;
	}

	const Facetype* CharLook::getfacetype() const
	{
		return face;
	}

	const CharEquips& CharLook::getequips() const
	{
		return equips;
	}
}