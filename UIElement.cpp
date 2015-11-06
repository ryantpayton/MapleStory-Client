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
#include "UIElement.h"

namespace IO
{
	void UIElement::draw() const
	{
		if (active)
		{
			using::Graphics::PosArgument;
			for (size_t i = 0; i < graphics.getend(); i++)
			{
				graphics.get(i)->draw(PosArgument(position));
			}

			using::std::map;
			for (map<uint16_t, Button*>::const_iterator btit = buttons.getbegin(); btit != buttons.getend(); ++btit)
			{
				btit->second->draw(position);
			}
		}
	}

	void UIElement::update(uint16_t dpf)
	{
		if (active)
		{
			for (size_t i = 0; i < graphics.getend(); i++)
			{
				graphics.get(i)->update(dpf);
			}
		}
	}

	Mousestate UIElement::sendmouse(bool down, vector2d<int32_t> pos)
	{
		Mousestate ret = down ? MST_CLICKING : MST_IDLE;

		using::std::map;
		for (map<uint16_t, Button*>::iterator bmit = buttons.getbegin(); bmit != buttons.getend(); ++bmit)
		{
			Button* btit = bmit->second;
			if (btit->isactive() && btit->bounds(position).contains(pos))
			{
				if (btit->getstate() == BTS_NORMAL)
				{
					btit->setstate(BTS_MOUSEOVER);
					ret = MST_CANCLICK;
				}
				else if (btit->getstate() == BTS_MOUSEOVER)
				{
					if (down)
					{
						btit->setstate(BTS_PRESSED);
						buttonpressed(bmit->first);
					}
					else
					{
						ret = MST_CANCLICK;
					}
				}
			}
			else if (btit->getstate() == BTS_MOUSEOVER)
			{
				btit->setstate(BTS_NORMAL);
			}
		}

		return ret;
	}

	rectangle2d<int32_t> UIElement::bounds() const
	{
		return rectangle2d<int32_t>(position, position + dimension);
	}
}