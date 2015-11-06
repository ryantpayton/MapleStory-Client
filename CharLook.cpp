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
#include "CharLook.h"
#include "WeaponData.h"

namespace Character
{
	CharLook::CharLook()
	{
		setstance("stand");
		frame = 0;
		elapsed = 0;
		flip = true;
		expression = "default";
		fcframe = 0;
		fcelapsed = 0;
		action = 0;
		actionstr = "";
		actframe = 0;

		hair = 0;
		drawinfo = 0;
	}

	void CharLook::init(const BodyDrawinfo* bdi)
	{
		drawinfo = bdi;
	}

	void CharLook::draw(vector2d<int32_t> parentpos) const
	{
		using::Graphics::FlipArgument;
		FlipArgument args = FlipArgument(parentpos, flip);
		if (stance == "ladder" || stance == "rope")
		{

		}
		else
		{
			hair->draw(stance, Data::CL_HAIRBBODY, frame, args);
			equips.draw(EQL_CAPE, stance, Data::CL_CAPE, frame, args);
			equips.draw(EQL_SHIELD, stance, Data::CL_SHIELDBBODY, frame, args);
			body->draw(stance, Data::CL_BODY, frame, args);
			equips.draw(EQL_SHOES, stance, Data::CL_SHOES, frame, args);
			equips.draw(EQL_PANTS, stance, Data::CL_PANTS, frame, args);
			equips.draw(EQL_TOP, stance, Data::CL_TOP, frame, args);
			equips.draw(EQL_TOP, stance, Data::CL_MAIL, frame, args);
			body->draw(stance, Data::CL_LHAND, frame, args);
			equips.draw(EQL_GLOVES, stance, Data::CL_GLOVE, frame, args);
			hair->draw(stance, Data::CL_HAIR, frame, args);
			equips.draw(EQL_SHIELD, stance, Data::CL_SHIELDOHAIR, frame, args);
			equips.draw(EQL_EARRINGS, stance, Data::CL_EARRINGS, frame, args);
			body->draw(stance, Data::CL_HEAD, frame, args);
			hair->draw(stance, Data::CL_HAIRSHADE, frame, args);
			FlipArgument faceargs = FlipArgument(parentpos + drawinfo->getfacepos(stance, frame), flip, parentpos);
			face->draw(expression, Data::CL_FACE, fcframe, faceargs);
			equips.draw(EQL_FACEACC, (expression == "default") ? "blink" : expression, Data::CL_FACEACC, 0, faceargs);
			equips.draw(EQL_EYEACC, stance, Data::CL_EYEACC, frame, args);
			equips.draw(EQL_SHIELD, stance, Data::CL_SHIELD, frame, args);
			if (equips.isvisible(EQL_CAP))
			{
				equips.draw(EQL_CAP, stance, Data::CL_HAT, frame, args);
			}
			else
			{
				hair->draw(stance, Data::CL_HAIROHEAD, frame, args);
			}
			if (equips.istwohanded())
			{
				equips.draw(EQL_TOP, stance, Data::CL_MAILARM, frame, args);
				body->draw(stance, Data::CL_ARM, frame, args);
				equips.draw(EQL_WEAPON, stance, Data::CL_WEAPON, frame, args);
			}
			else
			{
				equips.draw(EQL_WEAPON, stance, Data::CL_WEAPON, frame, args);
				body->draw(stance, Data::CL_ARM, frame, args);
				equips.draw(EQL_TOP, stance, Data::CL_MAILARM, frame, args);
			}
			body->draw(stance, Data::CL_RHAND, frame, args);
			body->draw(stance, Data::CL_ARMOHAIR, frame, args);
			equips.draw(EQL_WEAPON, stance, Data::CL_WEAPONOHAND, frame, args);
			body->draw(stance, Data::CL_HANDOWEP, frame, args);
			equips.draw(EQL_GLOVES, stance, Data::CL_RGLOVE, frame, args);
			equips.draw(EQL_WEAPON, stance, Data::CL_WEAPONOGLOVE, frame, args);
		}
	}

	bool CharLook::update(short dpf)
	{
		bool aniend = false;
		if (dpf > 0)
		{
			elapsed += dpf;

			if (action == nullptr)
			{
				uint16_t delay = drawinfo->getdelay(stance, frame);
				if (elapsed > delay)
				{
					elapsed -= delay;
					frame = drawinfo->nextframe(stance, frame);

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
					actframe = drawinfo->nextacframe(actionstr, actframe);
					if (actframe > 0)
					{
						frame = action->getframe();
						stance = action->getstance();
					}
					else
					{
						aniend = true;
						action = 0;
						frame = 0;
						setstance("stand");
					}
				}
			}

			fcelapsed += dpf;
			uint16_t fcdelay = face->getdelay(expression, fcframe);
			if (fcelapsed > fcdelay)
			{
				fcelapsed -= fcdelay;
				fcframe = face->nextframe(expression, fcframe);
				if (fcframe == 0)
				{
					if (expression == "default")
					{
						expression = "blink";
					}
					else
					{
						expression = "default";
					}
				}
			}
		}
		return aniend;
	}

	void CharLook::setbody(BodyData* bd)
	{
		body = bd;
	}

	void CharLook::sethair(HairData* hd)
	{
		hair = hd;
	}

	void CharLook::setface(FaceData* fd)
	{
		face = fd;
	}

	void CharLook::addequip(const EquipData& eq)
	{
		bool changestance = false;
		if (eq.geteqslot() == EQL_WEAPON)
		{
			using::Data::WeaponData;
			const WeaponData& weapon = reinterpret_cast<const WeaponData&>(eq);
			changestance = weapon.istwohanded() != equips.istwohanded();
		}
		equips.addequip(eq);
		if (changestance)
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
	}

	void CharLook::setstance(string ststr)
	{
		if (ststr != stance && action == 0)
		{
			frame = 0;
			elapsed = 0;
			if (ststr == "stand" || ststr == "walk")
			{
				int8_t thchar = equips.istwohanded() ? '2' : '1';
				ststr.push_back(thchar);
			}
			stance = ststr;
		}
	}

	void CharLook::setaction(string acstr)
	{
		if (acstr != actionstr)
		{
			actframe = 0;
			frame = 0;
			elapsed = 0;
			actionstr = acstr;
			action = drawinfo->getaction(acstr, 0);
		}
	}

	void CharLook::setflip(bool f)
	{
		flip = f;
	}

	CharEquips& CharLook::getequips()
	{
		return equips;
	}
}
