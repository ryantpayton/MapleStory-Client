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
#include "MobHpBar.h"

namespace Graphics
{
	void MobHpBar::draw(Point<int16_t> position, int16_t hppercent) const
	{
		int16_t x = position.x() - WIDTH / 2;
		int16_t y = position.y() - HEIGHT * 3;
		int16_t fillw = static_cast<int16_t>((WIDTH - 6) * static_cast<float>(hppercent) / 100);
		Geometry::draw(x, y, WIDTH, HEIGHT, BLACK, 1.0f);
		Geometry::draw(x + 1, y + 1, WIDTH - 2, 1, WHITE, 1.0f);
		Geometry::draw(x + 1, y + HEIGHT - 2, WIDTH - 2, 1, WHITE, 1.0f);
		Geometry::draw(x + 1, y + 2, 1, HEIGHT - 4, WHITE, 1.0f);
		Geometry::draw(x + WIDTH - 2, y + 2, 1, HEIGHT - 4, WHITE, 1.0f);
		Geometry::draw(x + 3, y + 3, fillw, 3, HPBAR_LIGHTGREEN, 1.0f);
		Geometry::draw(x + 3, y + 6, fillw, 1, HPBAR_DARKGREEN, 1.0f);
	}
}