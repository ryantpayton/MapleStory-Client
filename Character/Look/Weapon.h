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
#include "Audio\Audio.h"
#include "Graphics\Animation.h"
#include "Util\Misc.h"
#include "Util\rectangle2d.h"

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

		class AfterImage
		{
		public:
			AfterImage(node src);
			AfterImage();

			void draw(uint8_t stframe, const DrawArgument& args, float alpha) const;
			void update(uint8_t stframe, uint16_t timestep);

			uint8_t getfirstframe() const;
			rectangle2d<int16_t> getrange() const;

		private:
			Animation animation;
			rectangle2d<int16_t> range;
			uint8_t firstframe;
			bool displayed;
		};

		Weapon(int32_t, const BodyDrawinfo&);
		Weapon();

		bool istwohanded() const;
		uint8_t getspeed() const;
		uint8_t getattack() const;
		string getspeedstring() const;
		uint8_t getattackdelay() const;
		rectangle2d<int16_t> getrange(Stance::Value stance) const;
		Type gettype() const;
		Stance::Value getstand() const;
		Stance::Value getwalk() const;
		Sound getusesound(bool degenerate) const;
		AfterImage getafterimage(Stance::Value stance) const;

	private:
		Type type;
		bool twohanded;
		Stance::Value walk;
		Stance::Value stand;
		uint8_t attackspeed;
		uint8_t attack;
		BoolPair<Sound> usesounds;

		struct AfterImageStances
		{
			unordered_map<Stance::Value, AfterImage> afterimages;

			AfterImageStances(node src)
			{
				for (node sub : src)
				{
					Stance::Value stance = Stance::bystring(sub.name());
					afterimages[stance] = sub;
				}
			}

			AfterImageStances() {}

			AfterImage get(Stance::Value stance) const
			{
				if (afterimages.count(stance))
				{
					return afterimages.at(stance);
				}
				else
				{
					return AfterImage();
				}
			}

			rectangle2d<int16_t> range(Stance::Value stance) const
			{
				return get(stance).getrange();
			}
		};
		AfterImageStances afterimage;

		static AfterImageStances getafterimage(string ainame, int16_t lvprefix);
		static unordered_map<string, AfterImageStances> afterimages;
	};
}

