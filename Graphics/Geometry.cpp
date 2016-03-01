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
#include "Geometry.h"
#include "GraphicsEngine.h"

namespace Graphics
{
	void Geometry::draw(int16_t x, int16_t y, int16_t w, int16_t h, Geometry::Color cid, float opacity) const
	{
		static const float colors[NUM_COLORS][3] =
		{
			{ 0.0f, 0.0f, 0.0f }, // Black
			{ 1.0f, 1.0f, 1.0f }, // White
			{ 0.0f, 1.0f, 0.0f }, // Mob-hp-bar lightgreen
			{ 0.0f, 0.75f, 0.0f }, // Mob-hp-bar green
			{ 0.0f, 0.5f, 0.0f } // Mob-hp-bar darkgreen
		};
		if (opacity < 0.0f)
			opacity = 0.0f;
		const float* color = colors[cid];

		GraphicsEngine::getengine().drawrectangle(x, y, w, h, color[0], color[1], color[2], opacity);
	}
}