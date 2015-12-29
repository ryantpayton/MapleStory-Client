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

namespace Character
{
	enum Maplestat
	{
		MS_SKIN = 0x1,
		MS_FACE = 0x2,
		MS_HAIR = 0x4,
		MS_LEVEL = 0x10,
		MS_JOB = 0x20,
		MS_STR = 0x40,
		MS_DEX = 0x80,
		MS_INT = 0x100,
		MS_LUK = 0x200,
		MS_HP = 0x400,
		MS_MAXHP = 0x800,
		MS_MP = 0x1000,
		MS_MAXMP = 0x2000,
		MS_AP = 0x4000,
		MS_SP = 0x8000,
		MS_EXP = 0x10000,
		MS_FAME = 0x20000,
		MS_MESO = 0x40000,
		MS_PET = 0x180008,
		MS_GACHAEXP = 0x200000
	};

	const Maplestat statvalues[20] = 
	{ 
		MS_SKIN, MS_FACE, MS_HAIR, MS_LEVEL, MS_JOB, 
		MS_STR, MS_DEX, MS_INT, MS_LUK, MS_HP, MS_MAXHP,
		MS_MP, MS_MAXMP, MS_AP, MS_SP, MS_EXP, MS_FAME, 
		MS_MESO, MS_PET, MS_GACHAEXP 
	};
}