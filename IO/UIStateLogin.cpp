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
#include "UIStateLogin.h"

#include "UITypes\UILogin.h"

namespace jrc
{
	UIStateLogin::UIStateLogin()
	{
		focused = UIElement::NONE;

		add(Element<UILogin>());
	}

	void UIStateLogin::draw(float inter, Point<int16_t>) const
	{
		for (auto& entry : elements)
		{
			UIElement* element = entry.second.get();
			if (element && element->is_active())
				element->draw(inter);
		}
	}

	void UIStateLogin::update()
	{
		for (auto& entry : elements)
		{
			UIElement* element = entry.second.get();
			if (element && element->is_active())
				element->update();
		}
	}

	void UIStateLogin::doubleclick(Point<int16_t>) {}

	void UIStateLogin::send_key(Keyboard::Keytype, int32_t, bool) {}

	Cursor::State UIStateLogin::send_cursor(Cursor::State mst, Point<int16_t> pos)
	{
		UIElement* focusedelement = get(focused);
		if (focusedelement)
		{
			if (focusedelement->is_active())
			{
				return focusedelement->send_cursor(mst == Cursor::CLICKING, pos);
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
				if (element && element->is_active())
				{
					if (element->is_in_range(pos))
					{
						if (front)
						{
							element->remove_cursor(false, pos);
						}

						front = element;
						fronttype = elit.first;
					}
					else
					{
						element->remove_cursor(false, pos);
					}
				}
			}

			return front ? front->send_cursor(mst == Cursor::CLICKING, pos) : Cursor::IDLE;
		}
	}

	void UIStateLogin::drag_icon(Icon*) {}

	void UIStateLogin::show_equip(UIElement::Type, Equip*, int16_t) {}

	void UIStateLogin::show_item(UIElement::Type, int32_t) {}

	void UIStateLogin::show_skill(UIElement::Type, int32_t, int32_t, int32_t, int64_t) {}

	void UIStateLogin::clear_tooltip(UIElement::Type) {}

	void UIStateLogin::add(const IElement& element)
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

	UIElement* UIStateLogin::get_front(Point<int16_t> pos) const
	{
		UIElement* front = nullptr;
		for (auto& entry : elements)
		{
			UIElement* element = entry.second.get();
			if (element && element->is_active() && element->is_in_range(pos))
				front = element;
		}
		return front;
	}
}