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
#include "Clothing.h"
#include "Util\rectangle2d.h"
#include "Graphics\Animation.h"
#include "Audio\Sound.h"

namespace Character
{
	using Audio::Sound;
	using Graphics::Animation;

	class Weapon : public Clothing
	{
	public:
		enum Type
		{
			NONE = 0,
			SWORD_1H = 130,
			AXE_1H = 131,
			MACE_1H = 132,
			DAGGER = 133,
			WAND = 137,
			STAFF = 138,
			SWORD_2H = 140,
			AXE_2H = 141,
			MACE_2H = 142,
			SPEAR = 143,
			POLEARM = 144,
			BOW = 145,
			CROSSBOW = 146,
			CLAW = 147,
			KNUCKLE = 148,
			GUN = 149,
			CASH = 170
		};

		static Type typebyvalue(int32_t value)
		{
			if (value < 130 || (value > 133 && value < 137) || value == 139 || (value > 149 && value < 170) || value > 170)
				return NONE;

			return static_cast<Type>(value);
		}

		Weapon(int32_t, const BodyDrawinfo&);
		Weapon();

		void playsound() const;
		Type gettype() const;
		bool istwohanded() const;
		string getafterimage() const;
		uint8_t getspeed() const;
		uint8_t getattack() const;
		string getspeedstring() const;
		uint8_t getattackdelay() const;
		rectangle2d<int16_t> getrange() const;
		Animation gethiteffect() const;

	private:
		Type type;
		bool twohanded;
		string afterimage;
		uint8_t attackspeed;
		uint8_t attack;
		Sound firstattack;
		Sound secondattack;
	};
}

