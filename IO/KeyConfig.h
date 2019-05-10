//////////////////////////////////////////////////////////////////////////////
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
#pragma once
#include <cstdint>

namespace jrc
{
	namespace KeyConfig
	{
		enum Key : uint8_t
		{
			ESC, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, SLK,
			TILDA, NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8, NUM9, NUM0, MINUS, EQUAL, INS, HM, PUP,
			Q, W, E, R, T, Y, U, I, O, P, BRACKETL, BRACKETR, BACKSLASH, DEL, END, PDN,
			A, S, D, F, G, H, J, K, L, SEMICOLON, SINGLEQUOTE,
			SHIFTL, Z, X, C, V, B, N, M, COMMA, PERIOD, SHIFTR,
			CTRLL, ALTL, SPACE, ALTR, CTRLR,
			LENGTH
		};

		inline Key actionbyid(int32_t id)
		{
			return static_cast<Key>(id);
		}
	}
}