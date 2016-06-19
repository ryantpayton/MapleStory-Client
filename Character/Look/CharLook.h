/////////////////////////////////////////////////////////////////////////////
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
#include "Body.h"
#include "Hair.h"
#include "Face.h"
#include "CharEquips.h"

#include "..\..\Net\Login.h"
#include "..\..\Util\Interpolated.h"
#include "..\..\Util\Randomizer.h"

namespace jrc
{
	class CharLook
	{
	public:
		CharLook(const LookEntry& entry);
		CharLook();
		~CharLook();

		void reset();
		void draw(Point<int16_t> pos, float inter) const;
		void drawstanding(Point<int16_t> pos, bool flip) const;
		bool update(uint16_t timestep);

		void sethair(int32_t hairid);
		void setbody(int32_t skinid);
		void setface(int32_t faceid);
		void add_equip(int32_t equipid);
		void remove_equip(Equipslot::Value slot);

		void attack(bool degenerate);
		void attack(Stance::Value stance);
		void set_stance(Stance::Value stance);
		void setexpression(Expression::Value expression);
		void set_action(const std::string& action);
		void set_direction(bool mirrored);

		bool is_twohanded(Stance::Value stance) const;
		uint16_t get_attackdelay(size_t no, uint8_t first_frame) const;
		uint8_t get_frame() const;
		Stance::Value get_stance() const;

		const Body* getbodytype() const;
		const Hair* gethairstyle() const;
		const Face* getfacetype() const;
		const CharEquips& get_equips() const;

	private:
		void updatetwohanded();
		void draw(Point<int16_t> position, bool flip, Stance::Value interstance, 
			Expression::Value interexp, uint8_t interframe, uint8_t interfcframe) const;
		uint16_t get_delay(Stance::Value stance, uint8_t frame) const;
		uint8_t getnextframe(Stance::Value stance, uint8_t frame) const;
		Stance::Value getattackstance(uint8_t attack, bool degenerate) const;

		Nominal<Stance::Value> stance;
		Nominal<uint8_t> stframe;
		uint16_t stelapsed;

		Nominal<Expression::Value> expression;
		Nominal<uint8_t> expframe;
		uint16_t expelapsed;

		bool flip;

		const BodyAction* action;
		std::string actionstr;
		uint8_t actframe;

		const Body* body;
		const Hair* hair;
		const Face* face;
		CharEquips equips;

		Randomizer randomizer;
	};
}

