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
#include "UILoginwait.h"
#include "Graphics\Sprite.h"
#include "nlnx\nx.hpp"

namespace IO
{
	UILoginwait::UILoginwait()
	{
		using nl::node;

		node src = nl::nx::ui["Login.img"]["Notice"]["Loading"];
		sprites.push_back(Sprite(src["backgrnd"], vector2d<int32_t>()));
		sprites.push_back(Sprite(src["circle"], vector2d<int32_t>(125, 72)));

		position = vector2d<int32_t>(292, 200);
		dimension = vector2d<int32_t>(282, 144);
		active = true;
	}
}