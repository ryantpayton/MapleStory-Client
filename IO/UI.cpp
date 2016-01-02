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
#include "Cursor.h"
#include "Keyboard.h"
#include "Window.h"

#include "Components\StatusMessenger.h"
#include "UITypes\UIBuffList.h"
#include "UITypes\UIStatusbar.h"
#include "UITypes\UIStatsinfo.h"
#include "UITypes\UIItemInventory.h"
#include "UITypes\UIEquipInventory.h"
#include "Gameplay\Stage.h"
#include "Net\Session.h"
#include "Net\Packets\InventoryPackets.h"

#include <unordered_map>
#include <memory>

namespace IO
{
	namespace UI
	{
		using std::unique_ptr;
		using std::unordered_map;

		Keyboard keyboard;
		Cursor cursor;
		StatusMessenger messenger;

		unordered_map<Element::UIType, unique_ptr<UIElement>> elements;
		unordered_map<Element::UIType, Element::UIType> elementorder;
		unordered_map<int32_t, bool> keydown;

		Textfield* focusedtextfield = nullptr;
		Icon* draggedicon = nullptr;

		Element::UIType focused = Element::NONE;
		Mode mode = MD_LOGIN;
		bool enabled = true;

		void draw(float inter)
		{
			messenger.draw(vector2d<int16_t>(790, 510), inter);

			for (auto& elit : elementorder)
			{
				UIElement* element = elements[elit.first].get();

				if (element == nullptr)
					continue;

				if (element->isactive())
					element->draw(inter);
			}

			if (draggedicon)
				draggedicon->dragdraw(cursor.getposition());

			cursor.draw(inter);
		}

		void update()
		{
			messenger.update();

			for (auto& elit : elements)
			{
				if (elit.second->isactive())
					elit.second->update();
			}

			cursor.update();
		}

		void showstatus(Textlabel::Textcolor color, string message)
		{
			messenger.showstatus(color, message);
		}

		void enable()
		{
			enabled = true;
		}

		void disable()
		{
			enabled = false;
		}

		void changemode(Mode md)
		{
			if (md == MD_GAME)
			{
				add(ElementStatusbar(
					Gameplay::Stage::getplayer().getstats(),
					Gameplay::Stage::getplayer().getinvent()
					));
				add(ElementBuffList());
			}

			mode = md;
		}

		UIElement* getfront(vector2d<int16_t> pos)
		{
			UIElement* front = nullptr;
			for (auto& elit : elementorder)
			{
				UIElement* element = elements[elit.first].get();

				if (element == nullptr)
					continue;

				if (element->isactive() && element->bounds().contains(pos))
					front = element;
			}
			return front;
		}

		void dropicon(vector2d<int16_t> pos)
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

		void sendmouse(Cursor::Mousestate mst, vector2d<int16_t> pos)
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
							UIElement* element = elements[elit.first].get();

							if (element == nullptr)
								continue;

							if (element->isactive() && element->bounds().contains(pos))
							{
								if (front)
									front->sendmouse(false, element->bounds().getlt() - vector2d<int16_t>(1, 1));

								front = element;
								fronttype = elit.first;
							}
						}
					}

					if (front)
					{
						bool clicked = mst == Cursor::MST_CLICKING;
						if (clicked)
						{
							elementorder.erase(fronttype);
							elementorder[fronttype] = fronttype;
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

		void sendmouse(bool pressed, vector2d<int16_t> pos)
		{
			sendmouse(pressed ? Cursor::MST_CLICKING : Cursor::MST_IDLE, pos);
		}

		void sendmouse(vector2d<int16_t> pos)
		{
			sendmouse(cursor.getstate(), pos);
		}

		void doubleclick(vector2d<int16_t> pos)
		{
			UIElement* front = getfront(pos);
			if (front)
				front->doubleclick(pos);
		}

		void addelementbykey(int32_t key)
		{
			switch (key)
			{
			case Keyboard::KA_CHARSTATS:
				add(ElementStatsinfo(Gameplay::Stage::getplayer().getstats()));
				break;
			case Keyboard::KA_INVENTORY:
				add(ElementItemInventory(Gameplay::Stage::getplayer().getinvent()));
				break;
			case Keyboard::KA_EQUIPS:
				add(ElementEquipInventory(Gameplay::Stage::getplayer().getinvent()));
				break;
			}
		}

		void sendkey(int32_t keycode, bool pressed)
		{
			if (focusedtextfield)
			{
				Keyboard::Keymapping mapping = keyboard.gettextmapping(keycode);

				switch (mapping.type)
				{
				case Keyboard::KT_ACTION:
					focusedtextfield->sendkey(Keyboard::KT_ACTION, mapping.action, pressed);
					break;
				case Keyboard::KT_LETTER:
					if (!pressed)
					{
						int8_t letter;
						if (keydown[keyboard.getshiftkeycode()])
							letter = static_cast<int8_t>(mapping.action);
						else
							letter = static_cast<int8_t>(mapping.action + 32);

						focusedtextfield->sendkey(Keyboard::KT_LETTER, letter, pressed);
					}
					break;
				case Keyboard::KT_NUMBER:
					focusedtextfield->sendkey(Keyboard::KT_NUMBER, mapping.action, pressed);
					break;
				}
			}
			else if (mode == MD_GAME)
			{
				const Keyboard::Keymapping* mapping = keyboard.getmapping(keycode);

				if (!mapping)
					return;

				Keyboard::Keytype type = mapping->type;
				int32_t action = mapping->action;

				switch (type)
				{
				case Keyboard::KT_MENU:
					if (pressed)
					{
						addelementbykey(action);
					}
					break;
				case Keyboard::KT_ACTION:
				case Keyboard::KT_FACE:
				case Keyboard::KT_ITEM:
				case Keyboard::KT_SKILL:
					Gameplay::Stage::sendkey(type, action, pressed);
					break;
				}
			}

			keydown[keycode] = pressed;
		}

		void focustextfield(Textfield* tofocus)
		{
			focusedtextfield = tofocus;
		}

		void dragicon(Icon* todrag)
		{
			draggedicon = todrag;
		}

		void addkeymapping(uint8_t no, uint8_t t, int32_t action)
		{
			Keyboard::Keytype type = static_cast<Keyboard::Keytype>(t);
			keyboard.addmapping(no, type, action);
		}

		void add(const Element& element)
		{
			Element::UIType type = element.type();
			if (elements.count(type))
			{
				if (element.isunique())
				{
					elementorder.erase(type);
					elementorder[type] = type;
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
			elementorder[type] = type;

			if (element.isfocused())
			{
				focused = type;
			}
		}

		void remove(Element::UIType type)
		{
			focusedtextfield = nullptr;

			if (type == focused)
			{
				focused = Element::NONE;
			}

			elementorder.erase(type);

			if (elements.count(type))
			{
				elements[type]->deactivate();
				elements[type].release();
				elements.erase(type);
			}
		}

		bool haselement(Element::UIType type)
		{
			return getelement(type) != nullptr;
		}

		UIElement* getelement(Element::UIType type)
		{
			if (elements.count(type))
				return elements.at(type).get();
			else
				return nullptr;
		}
	}
}