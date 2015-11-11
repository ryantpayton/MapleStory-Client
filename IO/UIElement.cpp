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

#define button_ptr unique_ptr<Button>

namespace IO
{
	void UIElement::draw(float inter) const
	{
		if (active)
		{
			using::Graphics::DrawArgument;
			for (vector<Sprite>::const_iterator sprit = sprites.begin(); sprit != sprites.end(); ++sprit)
			{
				sprit->draw(DrawArgument(position), inter);
			}

			for (map<uint16_t, button_ptr>::const_iterator btit = buttons.begin(); btit != buttons.end(); ++btit)
			{
				btit->second->draw(position);
			}
		}
	}

	void UIElement::update()
	{
		if (active)
		{
			for (vector<Sprite>::iterator sprit = sprites.begin(); sprit != sprites.end(); ++sprit)
			{
				sprit->update();
			}
		}
	}

	Mousestate UIElement::sendmouse(bool down, vector2d<int32_t> pos)
	{
		Mousestate ret = down ? MST_CLICKING : MST_IDLE;

		for (map<uint16_t, button_ptr>::iterator bmit = buttons.begin(); bmit != buttons.end(); ++bmit)
		{
			Button* btit = bmit->second.get();
			if (btit->isactive() && btit->bounds(position).contains(pos))
			{
				if (btit->getstate() == Button::NORMAL)
				{
					btit->setstate(Button::MOUSEOVER);
					ret = MST_CANCLICK;
				}
				else if (btit->getstate() == Button::MOUSEOVER)
				{
					if (down)
					{
						btit->setstate(Button::PRESSED);
						buttonpressed(bmit->first);
					}
					else
					{
						ret = MST_CANCLICK;
					}
				}
			}
			else if (btit->getstate() == Button::MOUSEOVER)
			{
				btit->setstate(Button::NORMAL);
			}
		}

		return ret;
	}

	rectangle2d<int32_t> UIElement::bounds() const
	{
		return rectangle2d<int32_t>(position, position + dimension);
	}
}