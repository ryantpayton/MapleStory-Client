/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015 SYJourney                                               //
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
	CharLook::CharLook(LookEntry* le)
	{
		lookentry = le;

		setstance("stand");
		frame = 0;
		elapsed = 0;
		flip = false;
		expression = "default";
		fcframe = 0;
		fcelapsed = 0;
		action = 0;
		actionstr = "";
		actframe = 0;

		hair = 0;
		drawinfo = 0;
	}

	void CharLook::init(BodyDrawinfo* bdi)
	{
		drawinfo = bdi;
	}

	void CharLook::draw(vector2d<int> parentpos)
	{
		FlipArgument args = FlipArgument(parentpos, flip);
		if (stance == "ladder" || stance == "rope")
		{

		}
		else
		{
			hair->draw(stance, CL_HAIRBBODY, frame, &args);
			equips.draw(EQL_CAPE, stance, CL_CAPE, frame, &args);
			equips.draw(EQL_SHIELD, stance, CL_SHIELDBBODY, frame, &args);
			body->draw(stance, CL_BODY, frame, &args);
			equips.draw(EQL_SHOES, stance, CL_SHOES, frame, &args);
			equips.draw(EQL_PANTS, stance, CL_PANTS, frame, &args);
			equips.draw(EQL_TOP, stance, CL_TOP, frame, &args);
			equips.draw(EQL_TOP, stance, CL_MAIL, frame, &args);
			body->draw(stance, CL_LHAND, frame, &args);
			equips.draw(EQL_GLOVES, stance, CL_GLOVE, frame, &args);
			hair->draw(stance, CL_HAIR, frame, &args);
			equips.draw(EQL_SHIELD, stance, CL_SHIELDOHAIR, frame, &args);
			equips.draw(EQL_EARRINGS, stance, CL_EARRINGS, frame, &args);
			body->draw(stance, CL_HEAD, frame, &args);
			hair->draw(stance, CL_HAIRSHADE, frame, &args);
			FlipArgument faceargs = FlipArgument(parentpos + drawinfo->getfacepos(stance, frame), flip, parentpos);
			face->draw(expression, CL_FACE, fcframe, &faceargs);
			equips.draw(EQL_FACEACC, (expression == "default") ? "blink" : expression, CL_FACEACC, 0, &faceargs);
			equips.draw(EQL_EYEACC, stance, CL_EYEACC, frame, &args);
			equips.draw(EQL_SHIELD, stance, CL_SHIELD, frame, &args);
			if (equips.isvisible(EQL_CAP))
			{
				equips.draw(EQL_CAP, stance, CL_HAT, frame, &args);
			}
			else
			{
				hair->draw(stance, CL_HAIROHEAD, frame, &args);
			}
			if (equips.istwohanded())
			{
				equips.draw(EQL_TOP, stance, CL_MAILARM, frame, &args);
				body->draw(stance, CL_ARM, frame, &args);
				equips.draw(EQL_WEAPON, stance, CL_WEAPON, frame, &args);
			}
			else
			{
				equips.draw(EQL_WEAPON, stance, CL_WEAPON, frame, &args);
				body->draw(stance, CL_ARM, frame, &args);
				equips.draw(EQL_TOP, stance, CL_MAILARM, frame, &args);
			}
			body->draw(stance, CL_RHAND, frame, &args);
			body->draw(stance, CL_ARMOHAIR, frame, &args);
			equips.draw(EQL_WEAPON, stance, CL_WEAPONOHAND, frame, &args);
			body->draw(stance, CL_HANDOWEP, frame, &args);
			equips.draw(EQL_GLOVES, stance, CL_RGLOVE, frame, &args);
			equips.draw(EQL_WEAPON, stance, CL_WEAPONOGLOVE, frame, &args);
		}
	}

	bool CharLook::update(short dpf)
	{
		bool aniend = false;
		if (dpf > 0)
		{
			elapsed += dpf;

			if (action == 0)
			{
				short delay = drawinfo->getdelay(stance, frame);
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
				short delay = action->getdelay();
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
			short fcdelay = face->getdelay(expression, fcframe);
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

	void CharLook::addequip(EquipData* eq)
	{
		bool changestance = false;
		if (eq->geteqslot() == EQL_WEAPON)
		{
			WeaponData* weapon = reinterpret_cast<WeaponData*>(eq);
			changestance = weapon->istwohanded() != equips.istwohanded();
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
				char thchar = equips.istwohanded() ? '2' : '1';
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
}
