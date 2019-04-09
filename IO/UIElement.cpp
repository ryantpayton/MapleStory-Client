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
#include "UIElement.h"
#include "UI.h"

#include "../Constants.h"
#include "../Audio/Audio.h"
#include "../IO/UITypes/UISoftkey.h"

namespace jrc
{
	UIElement::UIElement(Point<int16_t> p, Point<int16_t> d, bool a)
		: position(p), dimension(d), active(a) {}

	UIElement::UIElement(Point<int16_t> p, Point<int16_t> d)
		: UIElement(p, d, true) {}

	UIElement::UIElement()
		: UIElement({}, {}) {}

	void UIElement::draw(float alpha) const
	{
		draw_sprites(alpha);
		draw_buttons(alpha);
	}

	void UIElement::draw_sprites(float alpha) const
	{
		for (const Sprite& sprite : sprites)
		{
			sprite.draw(position, alpha);
		}
	}

	void UIElement::draw_buttons(float) const
	{
		for (auto& iter : buttons)
		{
			if (const Button* button = iter.second.get())
			{
				button->draw(position);
			}
		}
	}

	void UIElement::update()
	{
		for (auto& sprite : sprites)
		{
			sprite.update();
		}
	}

	void UIElement::makeactive()
	{
		active = true;
	}

	void UIElement::deactivate()
	{
		active = false;
	}

	bool UIElement::is_active() const
	{
		return active;
	}

	void UIElement::toggle_active()
	{
		active = !active;
	}

	Button::State UIElement::button_pressed(uint16_t) { return Button::DISABLED; }

	void UIElement::send_icon(const Icon&, Point<int16_t>) {}

	void UIElement::doubleclick(Point<int16_t>) {}

	bool UIElement::is_in_range(Point<int16_t> cursorpos) const
	{
		auto bounds = Rectangle<int16_t>(position, position + dimension);
		return bounds.contains(cursorpos);
	}

	bool UIElement::remove_cursor(bool, Point<int16_t>)
	{
		for (auto& btit : buttons)
		{
			Button* button = btit.second.get();
			switch (button->get_state())
			{
			case Button::MOUSEOVER:
				button->set_state(Button::NORMAL);
				break;
			}
		}
		return false;
	}

	Cursor::State UIElement::send_cursor(bool down, Point<int16_t> pos)
	{
		Cursor::State ret = down ? Cursor::CLICKING : Cursor::IDLE;

		for (auto& btit : buttons)
		{
			if (btit.second->is_active() && btit.second->bounds(position).contains(pos))
			{
				if (btit.second->get_state() == Button::NORMAL)
				{
					Sound(Sound::BUTTONOVER).play();

					btit.second->set_state(Button::MOUSEOVER);
					ret = Cursor::CANCLICK;
				}
				else if (btit.second->get_state() == Button::MOUSEOVER)
				{
					if (down)
					{
						Sound(Sound::BUTTONCLICK).play();

						btit.second->set_state(button_pressed(btit.first));

						ret = Cursor::IDLE;
					}
					else
					{
						ret = Cursor::CANCLICK;
					}
				}
			}
			else if (btit.second->get_state() == Button::MOUSEOVER)
			{
				btit.second->set_state(Button::NORMAL);
			}
		}

		return ret;
	}

	void UIElement::send_key(int32_t action, bool pressed)
	{
		if (pressed && action == KeyAction::ESCAPE)
			if (auto softkey = UI::get().get_element<UISoftkey>())
				if (softkey->is_active())
					softkey->deactivate();
	}
}