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
#include "Bar.h"

namespace IO
{
	Bar::Bar(Texture front, Texture mid, Texture end, int16_t max, float percent)
	{
		barfront = front;
		barmid = mid;
		barend = end;
		maximum = max;
		percentage = percent;

		target = percentage;
	}

	Bar::Bar() {}

	Bar::~Bar() {}

	void Bar::draw(vector2d<int16_t> position) const
	{
		int16_t length = static_cast<int16_t>(percentage * maximum);
		if (length > 0)
		{
			using::Graphics::DrawArgument;
			barfront.draw(DrawArgument(position));
			barmid.draw(DrawArgument(position + vector2d<int16_t>(1, 0), vector2d<int16_t>(length, 0)));
			barend.draw(DrawArgument(position + vector2d<int16_t>(length + 1, 0)));
		}
	}

	void Bar::update(float t)
	{
		if (target != t)
		{
			target = t;
			step = (target - percentage) / 24;
		}

		if (percentage != target)
		{
			percentage += step;
			if (step < 0.0f)
			{
				if (target - percentage >= step)
					percentage = target;
			}
			else if (step > 0.0f)
			{
				if (target - percentage <= step)
					percentage = target;
			}
		}
	}
}