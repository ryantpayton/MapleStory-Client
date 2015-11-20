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
#include "UITypes\UIStatsinfo.h"
#include "UITypes\UIEquipInventory.h"
#include "Gameplay\Stage.h"
#include <unordered_map>
#include <memory>

namespace IO
{
	namespace UI
	{
		using std::unique_ptr;

		Keyboard keyboard;
		Cursor cursor;

		std::unordered_map<Element::UIType, unique_ptr<UIElement>> elements;
		std::map<int32_t, bool> keydown;

		Element::UIType focused = Element::NONE;
		Textfield* focusedtextfield = nullptr;
		bool enabled = false;
		bool gamekeysenabled = false;

		void init()
		{
			cursor.init();
			enabled = true;
		}

		void draw(float inter)
		{
			for (auto& elit : elements)
			{
				if (elit.second->isactive())
					elit.second->draw(inter);
			}

			cursor.draw(inter);
		}

		void update()
		{
			for (auto& elit : elements)
			{
				if (elit.second->isactive())
					elit.second->update();
			}

			cursor.update();
		}

		void enable()
		{
			enabled = true;
		}

		void disable()
		{
			enabled = false;
		}

		void sendmouse(vector2d<int16_t> pos)
		{
			sendmouse(cursor.getstate(), pos);
		}

		void sendmouse(Cursor::Mousestate mst, vector2d<int16_t> pos)
		{
			cursor.setposition(pos);

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
				if (enabled)
				{
					for (auto& elit : elements)
					{
						if (elit.second->isactive() && elit.second->bounds().contains(pos))
						{
							if (front)
							{
								front->sendmouse(false, pos);
							}
							front = elit.second.get();
						}
					}
				}

				if (front)
				{
					mst = front->sendmouse(mst == Cursor::MST_CLICKING, pos);
				}
				else
				{
					mst = Cursor::MST_IDLE;
				}
			}

			cursor.setstate(mst);
		}

		void addelementbykey(int32_t key)
		{
			switch (key)
			{
			case Keyboard::KA_CHARSTATS:
				add(ElementStatsinfo(Gameplay::Stage::getplayer().getstats()));
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
			else if (gamekeysenabled)
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

		void addkeymapping(uint8_t no, uint8_t t, int32_t action)
		{
			Keyboard::Keytype type = static_cast<Keyboard::Keytype>(t);
			keyboard.addmapping(no, type, action);
		}

		void enablegamekeys(bool enable)
		{
			gamekeysenabled = enable;
		}

		void add(const Element& element)
		{
			Element::UIType type = element.type();
			if (elements.count(type))
			{
				if (element.isunique())
				{
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

			if (elements.count(type))
			{
				elements[type]->deactivate();
				elements[type].release();
				elements.erase(type);
			}
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