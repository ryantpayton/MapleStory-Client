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
#include "UI.h"
#include "Window.h"
#include "UITypes\UIBuffList.h"
#include "UITypes\UIStatusbar.h"
#include "UITypes\UIStatsinfo.h"
#include "UITypes\UIItemInventory.h"
#include "UITypes\UIEquipInventory.h"
#include "UITypes\UINpcTalk.h"
#include "Gameplay\Stage.h"
#include "Net\Session.h"
#include "Net\Packets\InventoryPackets.h"

namespace IO
{
	using Gameplay::Stage;

	UI::UI()
	{
		focusedtextfield = nullptr;
		draggedicon = nullptr;
		focused = Element::NONE;
		mode = MD_LOGIN;
		enabled = true;
	}

	void UI::draw(float inter) const
	{
		messenger.draw(vector2d<int16_t>(790, 510), inter);

		for (auto& elit : elementorder)
		{
			UIElement* element = getelement(elit);

			if (element && element->isactive())
				element->draw(inter);
		}

		if (draggedicon)
			draggedicon->dragdraw(cursor.getposition());

		cursor.draw(inter);
	}

	void UI::update()
	{
		messenger.update();

		for (auto& elit : elements)
		{
			if (elit.second->isactive())
				elit.second->update();
		}

		cursor.update();
	}

	void UI::shownpctalk(int32_t npcid, int8_t type, int16_t style, int8_t speaker, string text)
	{
		UINpcTalk* npctalk = getelement<UINpcTalk>(Element::NPCTALK);
		if (npctalk)
			npctalk->settext(npcid, type, style, speaker, text);
	}

	void UI::showstatus(Text::Color color, string message)
	{
		messenger.showstatus(color, message);
	}

	void UI::enable()
	{
		enabled = true;
	}

	void UI::disable()
	{
		enabled = false;
	}

	void UI::changemode(Mode md)
	{
		if (md == MD_GAME)
		{
			add(ElementStatusbar(
				Stage::get().getplayer().getstats(),
				Stage::get().getplayer().getinvent()
				));
			add(ElementBuffList());
			add(ElementNpcTalk());
		}

		mode = md;
	}

	UIElement* UI::getfront(vector2d<int16_t> pos)
	{
		UIElement* front = nullptr;
		for (auto& elit : elementorder)
		{
			UIElement* element = elements[elit].get();

			if (element == nullptr)
				continue;

			if (element->isactive() && element->bounds().contains(pos))
				front = element;
		}
		return front;
	}

	void UI::dropicon(vector2d<int16_t> pos)
	{
		UIElement* front = getfront(pos);

		if (front)
		{

		}
		else
		{
			UIElement* parent = getelement(draggedicon->getparent());
			if (parent)
				parent->icondropped(draggedicon->getidentifier());
		}
	}

	void UI::sendmouse(Cursor::Mousestate mst, vector2d<int16_t> pos)
	{
		if (draggedicon)
		{
			switch (mst)
			{
			case Cursor::MST_IDLE:
				dropicon(pos);
				draggedicon->resetdrag();
				draggedicon = nullptr;
				break;
			default:
				mst = Cursor::MST_GRABBING;
			}
		}
		else
		{
			if (focused != Element::NONE)
			{
				if (elements[focused]->isactive())
					elements[focused]->sendmouse(mst == Cursor::MST_CLICKING, pos);
				else
					focused = Element::NONE;
			}

			if (focused == Element::NONE)
			{
				UIElement* front = nullptr;
				Element::UIType fronttype = Element::NONE;

				if (enabled)
				{
					for (auto& elit : elementorder)
					{
						UIElement* element = elements[elit].get();

						if (element == nullptr)
							continue;

						if (element->isactive() && element->bounds().contains(pos))
						{
							if (front)
								front->sendmouse(false, element->bounds().getlt() - vector2d<int16_t>(1, 1));

							front = element;
							fronttype = elit;
						}
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
					mst = front->sendmouse(clicked, pos);
				}
				else
				{
					mst = Cursor::MST_IDLE;
				}
			}
		}

		cursor.setposition(pos);
		cursor.setstate(mst);
	}

	void UI::sendmouse(bool pressed, vector2d<int16_t> pos)
	{
		sendmouse(pressed ? Cursor::MST_CLICKING : Cursor::MST_IDLE, pos);
	}

	void UI::sendmouse(vector2d<int16_t> pos)
	{
		sendmouse(cursor.getstate(), pos);
	}

	void UI::doubleclick(vector2d<int16_t> pos)
	{
		UIElement* front = getfront(pos);
		if (front)
			front->doubleclick(pos);
	}

	void UI::addelementbykey(int32_t key)
	{
		switch (key)
		{
		case Keyboard::KA_CHARSTATS:
			add(ElementStatsinfo(Stage::get().getplayer().getstats()));
			break;
		case Keyboard::KA_INVENTORY:
			add(ElementItemInventory(Stage::get().getplayer().getinvent()));
			break;
		case Keyboard::KA_EQUIPS:
			add(ElementEquipInventory(Stage::get().getplayer().getinvent()));
			break;
		}
	}

	void UI::sendkey(int32_t keycode, bool pressed)
	{
		if (focusedtextfield)
		{
			Keyboard::Keymapping mapping = keyboard.gettextmapping(keycode, keydown[keyboard.shiftcode()]);

			switch (mapping.type)
			{
			case Keyboard::KT_ACTION:
				focusedtextfield->sendkey(Keyboard::KT_ACTION, mapping.action, pressed);
				break;
			case Keyboard::KT_LETTER:
				focusedtextfield->sendkey(Keyboard::KT_LETTER, static_cast<int8_t>(mapping.action), pressed);
				break;
			case Keyboard::KT_NUMBER:
				focusedtextfield->sendkey(Keyboard::KT_NUMBER, static_cast<int8_t>(mapping.action), pressed);
				break;
			}
		}
		else if (mode == MD_GAME)
		{
			const Keyboard::Keymapping* mapping = keyboard.getmapping(keycode);

			if (mapping)
			{
				Keyboard::Keytype type = mapping->type;
				int32_t action = mapping->action;

				switch (type)
				{
				case Keyboard::KT_MENU:
					if (pressed)
						addelementbykey(action);
					break;
				case Keyboard::KT_ACTION:
				case Keyboard::KT_FACE:
				case Keyboard::KT_ITEM:
				case Keyboard::KT_SKILL:
					Stage::get().sendkey(type, action, pressed);
					break;
				}
			}
		}

		keydown[keycode] = pressed;
	}

	void UI::focustextfield(Textfield* tofocus)
	{
		focusedtextfield = tofocus;
	}

	void UI::dragicon(Icon* todrag)
	{
		draggedicon = todrag;
	}

	void UI::addkeymapping(uint8_t no, uint8_t t, int32_t action)
	{
		Keyboard::Keytype type = static_cast<Keyboard::Keytype>(t);
		keyboard.assign(no, type, action);
	}

	void UI::add(const Element& element)
	{
		Element::UIType type = element.type();
		if (elements.count(type))
		{
			if (element.isunique())
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

		UIElement* toadd = element.instantiate();
		elements[type] = unique_ptr<UIElement>(toadd);
		elementorder.push_back(type);

		if (element.isfocused())
		{
			focused = type;
		}
	}

	void UI::remove(Element::UIType type)
	{
		focusedtextfield = nullptr;

		if (type == focused)
		{
			focused = Element::NONE;
		}

		elementorder.remove(type);

		if (elements.count(type))
		{
			elements[type]->deactivate();
			elements[type].release();
			elements.erase(type);
		}
	}

	bool UI::haselement(Element::UIType type) const
	{
		return getelement(type) != nullptr;
	}

	UIElement* UI::getelement(Element::UIType type) const
	{
		return elements.count(type) ? elements.at(type).get() : nullptr;
	}
}