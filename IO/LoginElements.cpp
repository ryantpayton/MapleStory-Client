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
#include "LoginElements.h"

#include "IO\UITypes\UILogin.h"

namespace IO
{
	LoginElements::LoginElements()
	{
		focused = Element::NONE;

		add(ElementLogin());
	}

	void LoginElements::draw(float inter, vector2d<int16_t>) const
	{
		for (auto& entry : elements)
		{
			UIElement* element = entry.second.get();
			if (element && element->isactive())
				element->draw(inter);
		}
	}

	void LoginElements::update()
	{
		for (auto& entry : elements)
		{
			UIElement* element = entry.second.get();
			if (element && element->isactive())
				element->update();
		}
	}

	void LoginElements::doubleclick(vector2d<int16_t>) {}

	Cursor::Mousestate LoginElements::sendmouse(Cursor::Mousestate mst, vector2d<int16_t> pos)
	{
		UIElement* focusedelement = get(focused);
		if (focusedelement)
		{
			if (focusedelement->isactive())
			{
				return focusedelement->sendmouse(mst == Cursor::MST_CLICKING, pos);
			}
			else
			{
				focused = Element::NONE;
				return mst;
			}
		}
		else
		{
			UIElement* front = nullptr;
			Element::UIType fronttype = Element::NONE;

			for (auto& elit : elements)
			{
				UIElement* element = elit.second.get();
				if (element && element->isactive() && element->bounds().contains(pos))
				{
					if (front)
						front->sendmouse(false, element->bounds().getlt() - vector2d<int16_t>(1, 1));

					front = element;
					fronttype = elit.first;
				}
			}

			return front ? front->sendmouse(mst == Cursor::MST_CLICKING, pos) : Cursor::MST_IDLE;
		}
	}

	void LoginElements::add(const Element& element)
	{
		Element::UIType type = element.type();
		bool isfocused = element.isfocused();

		if (get(type))
			remove(type);

		elements[type] = unique_ptr<UIElement>(element.instantiate());

		if (isfocused)
			focused = type;
	}

	void LoginElements::remove(Element::UIType type)
	{
		if (focused == type)
			focused = Element::NONE;

		if (get(type))
		{
			elements[type]->deactivate();
			elements[type].release();
			elements.erase(type);
		}
	}

	UIElement* LoginElements::get(Element::UIType type) const
	{
		return elements.count(type) ? elements.at(type).get() : nullptr;
	}

	UIElement* LoginElements::getfront(vector2d<int16_t> pos) const
	{
		UIElement* front = nullptr;
		for (auto& entry : elements)
		{
			UIElement* element = entry.second.get();
			if (element && element->isactive() && element->bounds().contains(pos))
				front = element;
		}
		return front;
	}
}