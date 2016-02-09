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
#include "GameElements.h"

#include "UITypes\UIStatusMessenger.h"
#include "UITypes\UIStatusbar.h"
#include "UITypes\UIBuffList.h"
#include "UITypes\UINpcTalk.h"

namespace IO
{
	GameElements::GameElements()
	{
		focused = Element::NONE;
		draggedicon = nullptr;

		add(ElementStatusMessenger());
		add(ElementStatusbar());
		add(ElementBuffList());
		add(ElementNpcTalk());
	}

	void GameElements::draw(float inter, vector2d<int16_t> cursor) const
	{
		for (auto& entry : elementorder)
		{
			UIElement* element = get(entry);
			if (element && element->isactive())
				element->draw(inter);
		}

		if (draggedicon)
			draggedicon->dragdraw(cursor);
	}

	void GameElements::update()
	{
		for (auto& entry : elementorder)
		{
			UIElement* element = get(entry);
			if (element && element->isactive())
				element->update();
		}
	}

	void GameElements::dragicon(Icon* drgic)
	{
		draggedicon = drgic;
	}

	void GameElements::dropicon(vector2d<int16_t> pos, Element::UIType type, int16_t identifier)
	{
		UIElement* front = getfront(pos);

		if (front)
		{

		}
		else
		{
			UIElement* parent = get(type);
			if (parent)
				parent->icondropped(identifier);
		}
	}

	void GameElements::doubleclick(vector2d<int16_t> pos)
	{
		UIElement* front = getfront(pos);
		if (front)
			front->doubleclick(pos);
	}

	Cursor::Mousestate GameElements::sendmouse(Cursor::Mousestate mst, vector2d<int16_t> pos)
	{
		if (draggedicon)
		{
			switch (mst)
			{
			case Cursor::MST_IDLE:
				dropicon(pos, draggedicon->getparent(), draggedicon->getidentifier());
				draggedicon->resetdrag();
				draggedicon = nullptr;
				return mst;
			default:
				return Cursor::MST_GRABBING;
			}
		}
		else
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

				for (auto& elit : elementorder)
				{
					UIElement* element = get(elit);
					if (element && element->isactive() && element->bounds().contains(pos))
					{
						if (front)
							front->sendmouse(false, element->bounds().getlt() - vector2d<int16_t>(1, 1));

						front = element;
						fronttype = elit;
					}
				}

				if (front)
				{
					bool clicked = mst == Cursor::MST_CLICKING;
					if (clicked)
					{
						elementorder.remove(fronttype);
						elementorder.push_back(fronttype);
					}
					return front->sendmouse(clicked, pos);
				}
				else
				{
					return Cursor::MST_IDLE;
				}
			}
		}
	}

	void GameElements::add(const Element& element)
	{
		Element::UIType type = element.type();
		bool isfocused = element.isfocused();
		bool isunique = element.isunique();

		if (get(type))
		{
			if (isunique)
			{
				elementorder.remove(type);
				elementorder.push_back(type);
				elements[type]->togglehide();
				return;
			}
			else
			{
				remove(type);
			}
		}

		elements[type] = unique_ptr<UIElement>(element.instantiate());
		elementorder.push_back(type);

		if (isfocused)
			focused = type;
	}

	void GameElements::remove(Element::UIType type)
	{
		if (focused == type)
			focused = Element::NONE;

		elementorder.remove(type);

		if (get(type))
		{
			elements[type]->deactivate();
			elements[type].release();
			elements.erase(type);
		}
	}

	UIElement* GameElements::get(Element::UIType type) const
	{
		return elements.count(type) ? elements.at(type).get() : nullptr;
	}

	UIElement* GameElements::getfront(vector2d<int16_t> pos) const
	{
		UIElement* front = nullptr;
		for (auto& entry : elementorder)
		{
			UIElement* element = get(entry);
			if (element && element->isactive() && element->bounds().contains(pos))
				front = element;
		}
		return front;
	}
}