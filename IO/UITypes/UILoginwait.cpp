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
#include "UILoginwait.h"

#include "..\..\Graphics\Sprite.h"

#include <nlnx\nx.hpp>

namespace jrc
{
	UILoginwait::UILoginwait()
	{
		nl::node src = nl::nx::ui["Login.img"]["Notice"]["Loading"];

		background = src["backgrnd"];
		circle = { src["circle"], { 125, 72 } };

		position = { 292, 200 };
		dimension = { 282, 144 };
		active = true;
	}

	void UILoginwait::draw(float alpha) const
	{
		background.draw({ position });
		circle.draw(position, alpha);
	}

	void UILoginwait::update()
	{
		circle.update();
	}
}