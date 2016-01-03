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
#include "Slider.h"

namespace IO
{
	Slider::Slider(node src, vector2d<int16_t> ver, int16_t xp)
	{
		node dsrc = src["disabled"];

		dbase = dsrc["base"];
		dnext = dsrc["next"];
		dprev = dsrc["prev"];

		node esrc = src["enabled"];

		base = esrc["base"];
		next0 = esrc["next0"];
		next1 = esrc["next1"];
		prev0 = esrc["prev0"];
		prev1 = esrc["prev1"];
		thump0 = esrc["thump0"];
		thump1 = esrc["thump1"];

		vertical = ver;
		x = xp;
		enabled = true;
		slidery = 0;
	}

	Slider::Slider() {}

	Slider::~Slider() {}

	bool Slider::isenabled() const
	{
		return enabled;
	}

	void Slider::setenabled(bool en)
	{
		enabled = en;
	}

	void Slider::draw(vector2d<int16_t> position) const
	{
		vector2d<int16_t> start = vector2d<int16_t>(
			position.x() + x, 
			position.y() + vertical.x()
			);

		vector2d<int16_t> fill = vector2d<int16_t>(
			0,
			vertical.length() - 48
			);

		vector2d<int16_t> mid = vector2d<int16_t>(
			position.x() + x,
			position.y() + vertical.x() + slidery
			);

		vector2d<int16_t> end = vector2d<int16_t>(
			position.x() + x,
			position.y() + vertical.y()
			);

		using Graphics::DrawArgument;
		if (enabled)
		{
			base.draw(DrawArgument(start, fill));
			prev0.draw(DrawArgument(start));
			thump0.draw(DrawArgument(mid));
			next0.draw(DrawArgument(end));
		}
		else
		{
			dbase.draw(DrawArgument(start, fill));
			dprev.draw(DrawArgument(start));
			dnext.draw(DrawArgument(end));
		}
	}
}