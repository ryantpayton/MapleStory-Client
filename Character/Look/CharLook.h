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
#include "Body.h"
#include "Hair.h"
#include "Face.h"
#include "CharEquips.h"

#include "Gameplay\Attack.h"
#include "Net\Login.h"
#include "Util\Randomizer.h"

namespace Character
{
	using Gameplay::Attack;
	using Net::LookEntry;

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
		void addequip(int32_t equipid);
		void removeequip(Equipslot::Value slot);

		uint16_t attack(bool degenerate);
		void setstance(Stance::Value stance);
		void setexpression(Expression::Value expression);
		void setaction(string action);
		void setflip(bool mirrored);

		bool istwohanded(Stance::Value stance) const;

		const Body* getbodytype() const;
		const Hair* gethairstyle() const;
		const Face* getfacetype() const;
		const CharEquips& getequips() const;

	private:
		void updatetwohanded();
		void draw(Point<int16_t> position, bool flip, Stance::Value interstance, 
			Expression::Value interexp, uint8_t interframe, uint8_t interfcframe) const;
		Stance::Value getattackstance(uint8_t attack, bool degenerate) const;

		Stance::Value stance;
		uint8_t frame;
		uint16_t elapsed;

		Expression::Value expression;
		uint8_t fcframe;
		uint16_t fcelapsed;

		const BodyAction* action;
		string actionstr;
		uint8_t actframe;

		bool flip;

		const Body* body;
		const Hair* hair;
		const Face* face;
		CharEquips equips;

		Randomizer randomizer;

		Stance::Value laststance;
		uint8_t lastframe;
		uint16_t lastelapsed;
		Expression::Value lastexpression;
		uint8_t lastfcframe;
		uint16_t lastfcelapsed;
	};
}

