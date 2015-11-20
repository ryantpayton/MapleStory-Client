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
#include "Bodytype.h"
#include "Hairstyle.h"
#include "Facetype.h"
#include "CharEquips.h"
#include "Net\Login\LookEntry.h"

namespace Character
{
	using Net::LookEntry;

	class CharLook
	{
	public:
		static void init();

		CharLook(const LookEntry& entry);
		CharLook();
		~CharLook();

		void reset();
		void draw(vector2d<int16_t> pos, float inter) const;
		bool update(uint16_t timestep);

		void sethair(int32_t hairid);
		void setbody(uint8_t skinid);
		void setface(int32_t faceid);
		void addequip(int32_t equipid);

		void setstance(string stance);
		void setexpression(string expression);
		void setaction(string action);
		void setflip(bool mirrored);

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
		uint16_t lastelapsed;
		string lastexpression;
		uint8_t lastfcframe;
		uint16_t lastfcelapsed;
	};
}

