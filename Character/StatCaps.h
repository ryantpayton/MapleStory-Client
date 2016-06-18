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
#include <unordered_map>
#include <cstdint>

namespace jrc
{
	// Externalising this for easier configurability.
	const std::unordered_map<Equipstat::Value, int32_t> EQSTAT_CAPS =
	{
		{ Equipstat::STR, 999 },
		{ Equipstat::DEX, 999 },
		{ Equipstat::INT, 999 },
		{ Equipstat::LUK, 999 },
		{ Equipstat::HP, 30000 },
		{ Equipstat::MP, 30000 },
		{ Equipstat::WATK, 999 },
		{ Equipstat::MAGIC, 2000 },
		{ Equipstat::WDEF, 999 },
		{ Equipstat::MDEF, 999 },
		{ Equipstat::ACC, 999 },
		{ Equipstat::AVOID, 999 },
		{ Equipstat::HANDS, 999 },
		{ Equipstat::SPEED, 140 },
		{ Equipstat::JUMP, 123 },
	};
}