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
#include <cstdint>

namespace Constants
{
	using std::uint16_t;
	using std::int32_t;

	// Timestep, e.g. the granularity in which the game advances.
	const uint16_t TIMESTEP = 8;

	// Game width (float).
	const double fVIEWWIDTH = 800.0;
	// Game heihgt (float).
	const double fVIEWHEIGHT = 600.0;
	// Game width (int).
	const int32_t VIEWWIDTH = 800;
	// Game height (int).
	const int32_t VIEWHEIGHT = 600;
}