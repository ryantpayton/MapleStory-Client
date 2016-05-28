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
#include "UIStateLogin.h"

#include "UITypes\UILogin.h"

namespace jrc
{
	UIStateLogin::UIStateLogin()
	{
		focused = UIElement::NONE;

		add(ElementTag<UILogin>());
	}

	void UIStateLogin::draw(float inter, Point<int16_t>) const
	{
		for (auto& entry : elements)
		{
			UIElement* element = entry.second.get();
			if (element && element->isactive())
				element->draw(inter);
		}
	}

	void UIStateLogin::update()
	{
		for (auto& entry : elements)
		{
			UIElement* element = entry.second.get();
			if (element && element->isactive())
				element->update();
		}
	}

	void UIStateLogin::doubleclick(Point<int16_t>) {}

	void UIStateLogin::sendkey(Keyboard::Keytype, int32_t, bool) {}

	Cursor::State UIStateLogin::sendmouse(Cursor::State mst, Point<int16_t> pos)
	{
		UIElement* focusedelement = get(focused);
		if (focusedelement)
		{
			if (focusedelement->isactive())
			{
				return focusedelement->sendmouse(mst == Cursor::CLICKING, pos);
			}
			else
			{
				focused = UIElement::NONE;
				return mst;
			}
		}
		else
		{
			UIElement* front = nullptr;
			UIElement::Type fronttype = UIElement::NONE;

			for (auto& elit : elements)
			{
				UIElement* element = elit.second.get();
				if (element && element->isactive())
				{
					if (element->isinrange(pos))
					{
						if (front)
						{
							element->cursorleave(false, pos);
						}

						front = element;
						fronttype = elit.first;
					}
					else
					{
						element->cursorleave(false, pos);
					}
				}
			}

			return front ? front->sendmouse(mst == Cursor::CLICKING, pos) : Cursor::IDLE;
		}
	}

	void UIStateLogin::dragicon(Icon*) {}

	void UIStateLogin::showequip(UIElement::Type, Equip*, int16_t) {}

	void UIStateLogin::showitem(UIElement::Type, int32_t) {}

	void UIStateLogin::cleartooltip(UIElement::Type) {}

	void UIStateLogin::add(const Element& element)
	{
		UIElement::Type type = element.type();
		bool isfocused = element.focused();

		if (get(type))
			remove(type);

		elements.emplace(type, element.instantiate());

		if (isfocused)
			focused = type;
	}

	void UIStateLogin::remove(UIElement::Type type)
	{
		if (focused == type)
			focused = UIElement::NONE;

		if (get(type))
		{
			elements[type]->deactivate();
			elements[type].release();
			elements.erase(type);
		}
	}

	UIElement* UIStateLogin::get(UIElement::Type type) const
	{
		return elements.count(type) ? elements.at(type).get() : nullptr;
	}

	UIElement* UIStateLogin::getfront(Point<int16_t> pos) const
	{
		UIElement* front = nullptr;
		for (auto& entry : elements)
		{
			UIElement* element = entry.second.get();
			if (element && element->isactive() && element->isinrange(pos))
				front = element;
		}
		return front;
	}
}