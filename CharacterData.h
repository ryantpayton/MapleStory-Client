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
#include "Texture.h"
#include "ptrmap.h"

using namespace Graphics;

namespace Data
{
	enum CharacterLayer
	{
		CL_BASE,
		CL_HAIRBBODY,
		CL_CAPE,
		CL_SHIELDBBODY,
		CL_BODY,
		CL_SHOES,
		CL_PANTS,
		CL_TOP,
		CL_MAIL,
		CL_LHAND,
		CL_GLOVE,
		CL_HAIR,
		CL_BACKHAIRBCAP,
		CL_BACKHAIR,
		CL_SHIELDOHAIR,
		CL_EARRINGS,
		CL_HEAD,
		CL_FACE,
		CL_HAIRSHADE,
		CL_FACEACC,
		CL_EYEACC,
		CL_PENDANT,
		CL_BELT,
		CL_MEDAL,
		CL_RING,
		CL_SHIELD,
		CL_BACKSHIELD,
		CL_WEAPON,
		CL_BACKWEAPON,
		CL_ARM,
		CL_MAILARM,
		CL_RHAND,
		CL_HAIROHEAD,
		CL_HAT,
		CL_ARMOHAIR,
		CL_WEAPONOHAND,
		CL_RGLOVE,
		CL_WEAPONOGLOVE,
		CL_HANDOWEP
	};

	class CharacterData
	{
	public:
		virtual ~CharacterData() {}
		void draw(string, CharacterLayer, uint8_t, DrawArgument*);
	protected:
		map<string, map<CharacterLayer, ptrmap<uint8_t, Texture*>>> stances;
	};
}

