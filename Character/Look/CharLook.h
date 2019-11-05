//////////////////////////////////////////////////////////////////////////////////
//	This file is part of the continued Journey MMORPG client					//
//	Copyright (C) 2015-2019  Daniel Allendorf, Ryan Payton						//
//																				//
//	This program is free software: you can redistribute it and/or modify		//
//	it under the terms of the GNU Affero General Public License as published by	//
//	the Free Software Foundation, either version 3 of the License, or			//
//	(at your option) any later version.											//
//																				//
//	This program is distributed in the hope that it will be useful,				//
//	but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				//
//	GNU Affero General Public License for more details.							//
//																				//
//	You should have received a copy of the GNU Affero General Public License	//
//	along with this program.  If not, see <https://www.gnu.org/licenses/>.		//
//////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "BodyDrawinfo.h"
#include "Body.h"
#include "Hair.h"
#include "Face.h"
#include "CharEquips.h"

#include "../../Net/Login.h"
#include "../../Template/Interpolated.h"
#include "../../Util/Randomizer.h"
#include "../../Util/TimedBool.h"

namespace ms
{
	class CharLook
	{
	public:
		CharLook(const LookEntry& entry);
		CharLook();

		void reset();
		void draw(const DrawArgument& args, float alpha) const;
		void draw(Point<int16_t> pos, bool flip, Stance::Id stance, Expression::Id expression) const;
		bool update(std::uint16_t timestep);

		void set_hair(std::int32_t hairid);
		void set_body(std::int32_t skinid);
		void set_face(std::int32_t faceid);
		void add_equip(std::int32_t equipid);
		void remove_equip(Equipslot::Id slot);

		void attack(bool degenerate);
		void attack(Stance::Id stance);
		void set_stance(Stance::Id stance);
		void set_expression(Expression::Id expression);
		void set_action(const std::string& action);
		void set_direction(bool mirrored);
		void set_alerted(int64_t millis);
		bool get_alerted() const;

		bool is_twohanded(Stance::Id stance) const;
		std::uint16_t get_attackdelay(std::size_t no, std::uint8_t first_frame) const;
		std::uint8_t get_frame() const;
		Stance::Id get_stance() const;

		const Body* get_body() const;
		const Hair* get_hair() const;
		const Face* get_face() const;
		const CharEquips& get_equips() const;

		// Initialize drawinfo.
		static void init();

	private:
		void updatetwohanded();
		void draw(const DrawArgument& args, Stance::Id interstance, Expression::Id interexp, std::uint8_t interframe, std::uint8_t interfcframe) const;
		std::uint16_t get_delay(Stance::Id stance, std::uint8_t frame) const;
		std::uint8_t getnextframe(Stance::Id stance, std::uint8_t frame) const;
		Stance::Id getattackstance(std::uint8_t attack, bool degenerate) const;

		Nominal<Stance::Id> stance;
		Nominal<std::uint8_t> stframe;
		std::uint16_t stelapsed;

		Nominal<Expression::Id> expression;
		Nominal<std::uint8_t> expframe;
		std::uint16_t expelapsed;

		bool flip;

		const BodyAction* action;
		std::string actionstr;
		std::uint8_t actframe;

		const Body* body;
		const Hair* hair;
		const Face* face;
		CharEquips equips;

		Randomizer randomizer;
		TimedBool alerted;

		static BodyDrawinfo drawinfo;
		static std::unordered_map<std::int32_t, Hair> hairstyles;
		static std::unordered_map<std::int32_t, Face> facetypes;
		static std::unordered_map<std::int32_t, Body> bodytypes;
	};
}