//////////////////////////////////////////////////////////////////////////////
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
#include "Char.h"
#include "Net\Login.h"
#include "Gameplay\MovementInfo.h"

namespace Character
{
	using std::vector;
	using Net::LookEntry;
	using Gameplay::Physics;
	using Gameplay::MovementFragment;

	class OtherChar : public Char
	{
	public:
		OtherChar(int32_t charid, const LookEntry& look, uint8_t level, 
			int16_t job, string name, int8_t stance, vector2d<int16_t> position);

		int8_t update(const Physics& physics);
		void sendmovement(const vector<MovementFragment>& movements);

	private:
		uint8_t level;
		int16_t job;
		string name;
		vector<MovementFragment> movements;
		MovementFragment lastmove;
	};
}