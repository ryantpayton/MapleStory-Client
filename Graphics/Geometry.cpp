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
#include "GraphicsGL.h"

namespace jrc
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

		GraphicsGL::get().drawrectangle(x, y, w, h, color[0], color[1], color[2], opacity);
	}


	ColorBox::ColorBox(int16_t w, int16_t h, Geometry::Color c, float o)
	{
		width = w;
		height = h;
		color = c;
		opacity = o;
	}

	ColorBox::ColorBox()
		: ColorBox(0, 0, Geometry::BLACK, 0.0f) {}

	void ColorBox::setwidth(int16_t w)
	{
		width = w;
	}

	void ColorBox::setheight(int16_t h)
	{
		height = h;
	}

	void ColorBox::setcolor(Geometry::Color c)
	{
		color = c;
	}

	void ColorBox::setopacity(float o)
	{
		opacity = o;
	}

	void ColorBox::draw(const DrawArgument& args) const
	{
		Point<int16_t> absp = args.getpos();
		int16_t absw = args.getstretch().x();
		if (absw == 0)
			absw = width;
		int16_t absh = args.getstretch().y();
		if (absh == 0)
			absh = height;
		absw = static_cast<int16_t>(absw * args.getxscale());
		absh = static_cast<int16_t>(absh * args.getyscale());
		float absopc = opacity * args.getopacity();
		Geometry::draw(absp.x(), absp.y(), absw, absh, color, absopc);
	}


	void MobHpBar::draw(Point<int16_t> position, int16_t hppercent) const
	{
		int16_t fillw = static_cast<int16_t>((WIDTH - 6) * static_cast<float>(hppercent) / 100);
		int16_t x = position.x() - WIDTH / 2;
		int16_t y = position.y() - HEIGHT * 3;
		Geometry::draw(x, y, WIDTH, HEIGHT, BLACK, 1.0f);
		Geometry::draw(x + 1, y + 1, WIDTH - 2, 1, WHITE, 1.0f);
		Geometry::draw(x + 1, y + HEIGHT - 2, WIDTH - 2, 1, WHITE, 1.0f);
		Geometry::draw(x + 1, y + 2, 1, HEIGHT - 4, WHITE, 1.0f);
		Geometry::draw(x + WIDTH - 2, y + 2, 1, HEIGHT - 4, WHITE, 1.0f);
		Geometry::draw(x + 3, y + 3, fillw, 3, HPBAR_LIGHTGREEN, 1.0f);
		Geometry::draw(x + 3, y + 6, fillw, 1, HPBAR_DARKGREEN, 1.0f);
	}
}