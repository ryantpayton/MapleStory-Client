//////////////////////////////////////////////////////////////////////////////
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
#include "UI.h"

#include "UIStateLogin.h"
#include "UIStateGame.h"
#include "Window.h"

#include "../IO/UITypes/UIStatsinfo.h"
#include "../IO/UITypes/UIItemInventory.h"
#include "../IO/UITypes/UIEquipInventory.h"
#include "../IO/UITypes/UISkillbook.h"
#include "../IO/UITypes/UIQuestLog.h"
#include "../IO/UITypes/UIWorldMap.h"
#include "../IO/UITypes/UIUserList.h"
#include "../IO/UITypes/UIChatbar.h"
#include "../IO/UITypes/UIMiniMap.h"
#include "../IO/UITypes/UIStatusbar.h"

namespace jrc
{
	UI::UI()
	{
		state = std::make_unique<UIStateNull>();
		enabled = true;
	}

	void UI::init()
	{
		cursor.init();
		change_state(LOGIN);
	}

	void UI::draw(float alpha) const
	{
		state->draw(alpha, cursor.get_position());

		scrollingnotice.draw(alpha);
		cursor.draw(alpha);
	}

	void UI::update()
	{
		state->update();

		scrollingnotice.update();
		cursor.update();
	}

	void UI::enable()
	{
		enabled = true;
	}

	void UI::disable()
	{
		enabled = false;
	}

	void UI::change_state(State id)
	{
		switch (id)
		{
		case LOGIN:
			state = std::make_unique<UIStateLogin>();
			break;
		case GAME:
			state = std::make_unique<UIStateGame>();
			break;
		}
	}

	void UI::quit()
	{
		quitted = true;
	}

	bool UI::not_quitted() const
	{
		return !quitted;
	}

	void UI::send_cursor(Point<int16_t> cursorpos, Cursor::State cursorstate)
	{
		Cursor::State nextstate = state->send_cursor(cursorstate, cursorpos);
		cursor.set_state(nextstate);
		cursor.set_position(cursorpos);
	}

	void UI::send_focus(int focused)
	{
		if (focused)
		{
			// The window gained input focus
			uint8_t volume = Setting<SFXVolume>::get().load();
			Sound::set_sfxvolume(volume);
			Music::set_bgmvolume(volume);
		}
		else
		{
			// The window lost input focus
			Sound::set_sfxvolume(0);
			Music::set_bgmvolume(0);
		}
	}

	void UI::send_cursor(bool pressed)
	{
		Cursor::State cursorstate = (pressed && enabled) ? Cursor::CLICKING : Cursor::IDLE;
		Point<int16_t> cursorpos = cursor.get_position();
		send_cursor(cursorpos, cursorstate);

		if (focusedtextfield && pressed)
		{
			Cursor::State tstate = focusedtextfield->send_cursor(cursorpos, pressed);

			switch (tstate)
			{
			case Cursor::IDLE:
				focusedtextfield = {};
				break;
			}
		}
	}

	void UI::send_cursor(Point<int16_t> pos)
	{
		send_cursor(pos, cursor.get_state());
	}

	void UI::rightclick()
	{
		Point<int16_t> pos = cursor.get_position();
		state->rightclick(pos);
	}

	void UI::doubleclick()
	{
		Point<int16_t> pos = cursor.get_position();
		state->doubleclick(pos);
	}

	void UI::send_key(int32_t keycode, bool pressed)
	{
		if (is_key_down[keyboard.capslockcode()])
			caps_lock_enabled = !caps_lock_enabled;

		if (focusedtextfield)
		{
			bool ctrl = is_key_down[keyboard.leftctrlcode()] || is_key_down[keyboard.rightctrlcode()];

			if (ctrl)
			{
				if (!pressed)
				{
					KeyAction::Id action = keyboard.get_ctrl_action(keycode);

					switch (action)
					{
					case KeyAction::COPY:
						Window::get().setclipboard(focusedtextfield->get_text());
						break;
					case KeyAction::PASTE:
						focusedtextfield->add_string(Window::get().getclipboard());
						break;
					}
				}
			}
			else
			{
				bool shift = is_key_down[keyboard.leftshiftcode()] || is_key_down[keyboard.rightshiftcode()] || caps_lock_enabled;
				Keyboard::Mapping mapping = keyboard.get_text_mapping(keycode, shift);
				focusedtextfield->send_key(mapping.type, mapping.action, pressed);
			}
		}
		else
		{
			Keyboard::Mapping mapping = keyboard.get_mapping(keycode);

			if (mapping.type)
			{
				auto chatbar = UI::get().get_element<UIChatbar>();

				if (pressed && (keycode == GLFW_KEY_ESCAPE || keycode == GLFW_KEY_TAB))
				{
					auto statsinfo = UI::get().get_element<UIStatsinfo>();
					auto iteminventory = UI::get().get_element<UIItemInventory>();
					auto equipinventory = UI::get().get_element<UIEquipInventory>();
					auto skillbook = UI::get().get_element<UISkillbook>();
					auto questlog = UI::get().get_element<UIQuestLog>();
					auto worldmap = UI::get().get_element<UIWorldMap>();
					auto userlist = UI::get().get_element<UIUserList>();
					auto statusbar = UI::get().get_element<UIStatusbar>();

					if (statsinfo && statsinfo->is_active())
						statsinfo->send_key(mapping.action, pressed);
					else if (iteminventory && iteminventory->is_active())
						iteminventory->send_key(mapping.action, pressed);
					else if (equipinventory && equipinventory->is_active())
						equipinventory->send_key(mapping.action, pressed);
					else if (skillbook && skillbook->is_active())
						skillbook->send_key(mapping.action, pressed);
					else if (questlog && questlog->is_active())
						questlog->send_key(mapping.action, pressed);
					else if (worldmap && worldmap->is_active())
						worldmap->send_key(mapping.action, pressed);
					else if (userlist && userlist->is_active())
						userlist->send_key(mapping.action, pressed);
					else if (statusbar)
						statusbar->send_key(mapping.action, pressed);
					else if (chatbar)
						chatbar->send_key(mapping.action, pressed);
					else
						state->send_key(mapping.type, mapping.action, pressed);
				}
				else if (pressed && (keycode == GLFW_KEY_ENTER || keycode == GLFW_KEY_KP_ENTER))
				{
					if (chatbar)
						chatbar->send_key(mapping.action, pressed);
				}
				else if (pressed && keycode == GLFW_KEY_M)
				{
					if (auto ninimap = UI::get().get_element<UIMiniMap>())
						ninimap->send_key(mapping.action, pressed);
				}
				else
				{
					state->send_key(mapping.type, mapping.action, pressed);
				}
			}
		}

		is_key_down[keycode] = pressed;
	}

	void UI::send_menu(KeyAction::Id action)
	{
		state->send_key(KeyType::MENU, action, true);
	}

	void UI::set_scrollnotice(const std::string& notice)
	{
		scrollingnotice.setnotice(notice);
	}

	void UI::focus_textfield(Textfield* tofocus)
	{
		if (focusedtextfield)
		{
			focusedtextfield->set_state(Textfield::NORMAL);
		}

		focusedtextfield = tofocus;
	}

	void UI::remove_textfield()
	{
		focusedtextfield = {};
	}

	void UI::drag_icon(Icon* icon)
	{
		state->drag_icon(icon);
	}

	void UI::add_keymapping(uint8_t no, uint8_t type, int32_t action)
	{
		keyboard.assign(no, type, action);
	}

	void UI::clear_tooltip(Tooltip::Parent parent)
	{
		state->clear_tooltip(parent);
	}

	void UI::show_equip(Tooltip::Parent parent, int16_t slot)
	{
		state->show_equip(parent, slot);
	}

	void UI::show_item(Tooltip::Parent parent, int32_t item_id)
	{
		state->show_item(parent, item_id);
	}

	void UI::show_skill(Tooltip::Parent parent, int32_t skill_id,
		int32_t level, int32_t masterlevel, int64_t expiration) {

		state->show_skill(parent, skill_id, level, masterlevel, expiration);
	}

	void UI::remove(UIElement::Type type)
	{
		focusedtextfield = {};
		state->remove(type);
	}
}