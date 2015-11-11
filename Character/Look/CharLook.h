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
#include "Character\Look\Bodytype.h"
#include "Character\Look\Hairstyle.h"
#include "Character\Look\Facetype.h"
#include "Character\Look\CharEquips.h"
#include "Net\Login\LookEntry.h"

namespace Character
{
	using::Net::LookEntry;

	class CharLook
	{
	public:
		static void init();

		CharLook(const LookEntry&);
		CharLook();
		~CharLook();
		void reset();
		void draw(vector2d<int32_t>, float) const;
		bool update(uint16_t);
		void sethair(int32_t);
		void setbody(uint8_t);
		void setface(int32_t);
		void addequip(int32_t);
		void setstance(string);
		void setexpression(string);
		void setaction(string);
		void setflip(bool);

		const Bodytype* getbodytype() const;
		const Hairstyle* gethairstyle() const;
		const Facetype* getfacetype() const;
		const CharEquips& getequips() const;

	private:
		string stance;
		uint8_t frame;
		uint16_t elapsed;

		string expression;
		uint8_t fcframe;
		uint16_t fcelapsed;

		const BodyAction* action;
		string actionstr;
		uint8_t actframe;

		bool flip;

		const Bodytype* body;
		const Hairstyle* hair;
		const Facetype* face;
		CharEquips equips;

		string laststance;
		uint8_t lastframe;
		string lastexpression;
		uint8_t lastfcframe;
	};
}

