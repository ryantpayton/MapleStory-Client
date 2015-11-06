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
#include "BodyDrawinfo.h"
#include "BodyData.h"
#include "HairData.h"
#include "FaceData.h"
#include "CharEquips.h"

namespace Character
{
	using::std::string;
	using::Util::vector2d;
	using::Data::BodyDrawinfo;
	using::Data::BodyAction;
	using::Data::BodyData;
	using::Data::HairData;
	using::Data::FaceData;

	class CharLook
	{
	public:
		CharLook();
		~CharLook(){}
		void init(const BodyDrawinfo*);
		void draw(vector2d<int>) const;
		bool update(short);
		void sethair(HairData*);
		void setbody(BodyData*);
		void setface(FaceData*);
		void addequip(const EquipData&);
		void setstance(string);
		void setaction(string);
		void setflip(bool);
		CharEquips& getequips();
	private:
		string stance;
		uint8_t frame;
		short elapsed;
		bool flip;
		string expression;
		uint8_t fcframe;
		short fcelapsed;
		const BodyAction* action;
		string actionstr;
		uint8_t actframe;
		const BodyData* body;
		const HairData* hair;
		const FaceData* face;
		CharEquips equips;
		const BodyDrawinfo* drawinfo;
	};
}

