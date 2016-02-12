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
#include "UIStateGame.h"

#include "UITypes\UIStatusMessenger.h"
#include "UITypes\UIStatusbar.h"
#include "UITypes\UIBuffList.h"
#include "UITypes\UINpcTalk.h"
#include "UITypes\UIStatsinfo.h"
#include "UITypes\UIItemInventory.h"
#include "UITypes\UIEquipInventory.h"

#include "Gameplay\Stage.h"

namespace IO
{
	UIStateGame::UIStateGame()
	{
		focused = Element::NONE;
		draggedicon = nullptr;

		add(ElementStatusMessenger());
		add(ElementStatusbar());
		add(ElementBuffList());
		add(ElementNpcTalk());
	}

	void UIStateGame::draw(float inter, vector2d<int16_t> cursor) const
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

	void UIStateGame::update()
	{
		for (auto& entry : elementorder)
		{
			UIElement* element = get(entry);
			if (element && element->isactive())
				element->update();
		}
	}

	void UIStateGame::dropicon(vector2d<int16_t> pos, Element::UIType type, int16_t identifier)
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

	void UIStateGame::doubleclick(vector2d<int16_t> pos)
	{
		UIElement* front = getfront(pos);
		if (front)
			front->doubleclick(pos);
	}

	void UIStateGame::dragicon(Icon* drgic)
	{
		draggedicon = drgic;
	}

	void UIStateGame::sendkey(Keyboard::Keytype type, int32_t action, bool pressed)
	{
		switch (type)
		{
		case Keyboard::MENU:
			if (pressed)
				switch (action)
				{
				case Keyboard::CHARSTATS:
					add(ElementStatsinfo());
					break;
				case Keyboard::INVENTORY:
					add(ElementItemInventory());
					break;
				case Keyboard::EQUIPS:
					add(ElementEquipInventory());
					break;
				}
			break;
		case Keyboard::ACTION:
		case Keyboard::FACE:
		case Keyboard::ITEM:
		case Keyboard::SKILL:
			using Gameplay::Stage;
			Stage::get().sendkey(type, action, pressed);
			break;
		}
	}

	Cursor::State UIStateGame::sendmouse(Cursor::State mst, vector2d<int16_t> pos)
	{
		if (draggedicon)
		{
			switch (mst)
			{
			case Cursor::IDLE:
				dropicon(pos, draggedicon->getparent(), draggedicon->getidentifier());
				draggedicon->resetdrag();
				draggedicon = nullptr;
				return mst;
			default:
				return Cursor::GRABBING;
			}
		}
		else
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
					bool clicked = mst == Cursor::CLICKING;
					if (clicked)
					{
						elementorder.remove(fronttype);
						elementorder.push_back(fronttype);
					}
					return front->sendmouse(clicked, pos);
				}
				else
				{
					return Cursor::IDLE;
				}
			}
		}
	}

	void UIStateGame::add(const Element& element)
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

	void UIStateGame::remove(Element::UIType type)
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

	UIElement* UIStateGame::get(Element::UIType type) const
	{
		return elements.count(type) ? elements.at(type).get() : nullptr;
	}

	UIElement* UIStateGame::getfront(vector2d<int16_t> pos) const
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