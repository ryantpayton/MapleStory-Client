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
#include "UIStateGame.h"
#include "UI.h"

#include "UITypes\UIStatusMessenger.h"
#include "UITypes\UIStatusbar.h"
#include "UITypes\UIBuffList.h"
#include "UITypes\UINpcTalk.h"
#include "UITypes\UIShop.h"
#include "UITypes\UIStatsinfo.h"
#include "UITypes\UIItemInventory.h"
#include "UITypes\UIEquipInventory.h"
#include "UITypes\UISkillbook.h"

#include "..\Gameplay\Stage.h"

namespace jrc
{
	UIStateGame::UIStateGame()
	{
		focused = UIElement::NONE;
		tooltipparent = UIElement::NONE;

		add(Element<UIStatusMessenger>());
		add(Element<UIStatusbar>());
		add(Element<UIBuffList>());
		add(Element<UINpcTalk>());
		add(Element<UIShop>());
	}

	void UIStateGame::draw(float inter, Point<int16_t> cursor) const
	{
		for (auto& entry : elementorder)
		{
			UIElement* element = get(entry);
			if (element && element->is_active())
				element->draw(inter);
		}

		draggedicon.if_present(&Icon::dragdraw, cursor);
		tooltip.if_present(&Tooltip::draw, cursor);
	}

	void UIStateGame::update()
	{
		for (auto& entry : elementorder)
		{
			UIElement* element = get(entry);
			if (element && element->is_active())
				element->update();
		}
	}

	void UIStateGame::drop_icon(const Icon& icon, Point<int16_t> pos)
	{
		UIElement* front = get_front(pos);
		if (front)
		{
			front->send_icon(icon, pos);
		}
		else
		{
			icon.drop_on_stage();
		}
	}

	void UIStateGame::doubleclick(Point<int16_t> pos)
	{
		UIElement* front = get_front(pos);
		if (front)
			front->doubleclick(pos);
	}

	void UIStateGame::send_key(Keyboard::Keytype type, int32_t action, bool pressed)
	{
		switch (type)
		{
		case Keyboard::MENU:
			if (pressed)
				switch (action)
				{
				case Keyboard::CHARSTATS:
					add(Element<UIStatsinfo>());
					break;
				case Keyboard::INVENTORY:
					add(Element<UIItemInventory>());
					break;
				case Keyboard::EQUIPS:
					add(Element<UIEquipInventory>());
					break;
				case Keyboard::SKILLBOOK:
					add(Element<UISkillbook>());
					break;
				}
			break;
		case Keyboard::ACTION:
		case Keyboard::FACE:
		case Keyboard::ITEM:
		case Keyboard::SKILL:
			Stage::get().send_key(type, action, pressed);
			break;
		}
	}

	Cursor::State UIStateGame::send_cursor(Cursor::State mst, Point<int16_t> pos)
	{
		if (draggedicon)
		{
			switch (mst)
			{
			case Cursor::IDLE:
				drop_icon(*draggedicon, pos);
				draggedicon->reset();
				draggedicon = Optional<Icon>();
				return mst;
			default:
				return Cursor::GRABBING;
			}
		}
		else
		{
			bool clicked = mst == Cursor::CLICKING;
			UIElement* focusedelement = get(focused);
			if (focusedelement)
			{
				if (focusedelement->is_active())
				{
					return focusedelement->send_cursor(clicked, pos);
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

				for (auto& elit : elementorder)
				{
					UIElement* element = get(elit);
					bool found = false;
					if (element && element->is_active())
					{
						if (element->is_in_range(pos))
						{
							found = true;
						}
						else
						{
							found = element->remove_cursor(clicked, pos);
						}

						if (found)
						{
							if (front)
							{
								element->remove_cursor(false, pos);
							}

							front = element;
							fronttype = elit;
						}
					}
				}

				if (fronttype != tooltipparent)
					clear_tooltip(tooltipparent);

				if (front)
				{
					if (clicked)
					{
						elementorder.remove(fronttype);
						elementorder.push_back(fronttype);
					}
					return front->send_cursor(clicked, pos);
				}
				else
				{
					return Stage::get().send_cursor(clicked, pos);
				}
			}
		}
	}

	void UIStateGame::drag_icon(Icon* drgic)
	{
		draggedicon = drgic;
	}

	void UIStateGame::show_equip(UIElement::Type parent, Equip* equip, int16_t slot)
	{
		eqtooltip.set_equip(equip, slot);

		if (equip)
		{
			tooltip = eqtooltip;
			tooltipparent = parent;
		}
	}

	void UIStateGame::show_item(UIElement::Type parent, int32_t itemid)
	{
		ittooltip.set_item(itemid);

		if (itemid)
		{
			tooltip = ittooltip;
			tooltipparent = parent;
		}
	}

	void UIStateGame::show_skill(UIElement::Type parent, int32_t skill_id,
		int32_t level, int32_t masterlevel, int64_t expiration) {

		sktooltip.set_skill(skill_id, level, masterlevel, expiration);

		if (skill_id)
		{
			tooltip = sktooltip;
			tooltipparent = parent;
		}
	}

	void UIStateGame::clear_tooltip(UIElement::Type parent)
	{
		if (parent == tooltipparent)
		{
			eqtooltip.set_equip(nullptr, 0);
			ittooltip.set_item(0);
			tooltip = Optional<Tooltip>();
			tooltipparent = UIElement::NONE;
		}
	}

	void UIStateGame::add(const IElement& element)
	{
		UIElement::Type type = element.type();
		bool isfocused = element.focused();
		bool isunique = element.unique();

		if (get(type))
		{
			if (isunique)
			{
				elementorder.remove(type);
				elementorder.push_back(type);
				elements[type]->toggle_active();
				return;
			}
			else
			{
				remove(type);
			}
		}

		elements.emplace(type, element.instantiate());
		elementorder.push_back(type);

		if (isfocused)
			focused = type;
	}

	void UIStateGame::remove(UIElement::Type type)
	{
		if (type == focused)
			focused = UIElement::NONE; 
		if (type == tooltipparent)
			clear_tooltip(tooltipparent);

		elementorder.remove(type);

		if (get(type))
		{
			elements[type]->deactivate();
			elements[type].release();
			elements.erase(type);
		}
	}

	UIElement* UIStateGame::get(UIElement::Type type) const
	{
		return elements.count(type) ? elements.at(type).get() : nullptr;
	}

	UIElement* UIStateGame::get_front(Point<int16_t> pos) const
	{
		UIElement* front = nullptr;
		for (auto& entry : elementorder)
		{
			UIElement* element = get(entry);
			if (element && element->is_active() && element->is_in_range(pos))
				front = element;
		}
		return front;
	}
}