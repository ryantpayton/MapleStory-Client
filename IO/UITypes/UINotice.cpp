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
#include "UINotice.h"

#include "..\Components\MapleButton.h"

#include "nlnx\nx.hpp"

namespace jrc
{
	UINotice::UINotice(std::string q)
	{
		nl::node src = nl::nx::ui["Basic.img"]["Notice6"];

		top = src["t"];
		center = src["c"];
		centerbox = src["c_box"];
		box = src["box"];
		box2 = src["box2"];
		bottom = src["s"];
		bottombox = src["s_box"];

		question = { Text::A11M, Text::CENTER, Text::DARKGREY, q, 200 };

		height = question.height();
		dimension = Point<int16_t>(top.width(), top.height() + height + bottom.height());
		position = Point<int16_t>(400 - dimension.x() / 2, 240 - dimension.y() / 2);
	}

	void UINotice::draw(bool textfield) const
	{
		Point<int16_t> start = position;

		top.draw(start);
		start.shift_y(top.height());
		centerbox.draw(start);
		start.shift_y(centerbox.height());

		Point<int16_t> textpos = start;
		box.draw(DrawArgument(textpos, Point<int16_t>(0, height)));
		start.shift_y(box.height() * (height / box.height()));
		box.draw(start);
		start.shift_y(box.height());
		question.draw(textpos + Point<int16_t>(130, -3));

		if (textfield)
		{
			box2.draw(start);
			start.shift_y(box2.height());
		}
		box.draw(start);
		start.shift_y(box.height());
		bottombox.draw(start);
	}

	int16_t UINotice::box2offset() const
	{
		return top.height() + centerbox.height() + box.height() * (1 + height / box.height());
	}


	UIYesNo::UIYesNo(std::string q, std::function<void(bool)> yh)
		: UINotice(q) {

		yesnohandler = yh;

		int16_t belowtext = UINotice::box2offset();

		nl::node src = nl::nx::ui["Basic.img"];

		buttons[YES] = std::make_unique<MapleButton>(src["BtOK4"], 90, belowtext + 1);
		buttons[NO] = std::make_unique<MapleButton>(src["BtCancel4"], 132, belowtext + 1);
	}

	void UIYesNo::draw(float alpha) const
	{
		UINotice::draw(false);
		UIElement::draw(alpha);
	}

	Button::State UIYesNo::button_pressed(uint16_t buttonid)
	{
		switch (buttonid)
		{
		case YES:
			yesnohandler(true);
			break;
		case NO:
			yesnohandler(false);
			break;
		}

		active = false;

		return Button::PRESSED;
	}


	UIEnterNumber::UIEnterNumber(std::string q, std::function<void(int32_t)> nh, int32_t mi, int32_t ma, int32_t de)
		: UINotice(q) {

		numhandler = nh;
		min = mi;
		max = ma;

		int16_t belowtext = UINotice::box2offset();

		nl::node src = nl::nx::ui["Basic.img"];

		buttons[OK] = std::make_unique<MapleButton>(src["BtOK4"], 90, belowtext + 21);
		buttons[CANCEL] = std::make_unique<MapleButton>(src["BtCancel4"], 132, belowtext + 21);

		Rectangle<int16_t> area(26, 232, belowtext, belowtext + 20);
		numfield = Textfield(Text::A11M, Text::LEFT, Text::LIGHTGREY, area, 9);
		numfield.set_state(Textfield::FOCUSED);
		numfield.change_text(std::to_string(de));
		numfield.set_enter_callback([&](std::string numstr){
			handlestring(numstr);
		});
	}

	void UIEnterNumber::draw(float alpha) const
	{
		UINotice::draw(true);
		UIElement::draw(alpha);

		numfield.draw(position);
	}

	void UIEnterNumber::update()
	{
		UIElement::update();

		numfield.update(position);
	}

	Cursor::State UIEnterNumber::send_cursor(bool clicked, Point<int16_t> cursorpos)
	{
		if (numfield.get_state() == Textfield::NORMAL)
		{
			Cursor::State nstate = numfield.send_cursor(cursorpos, clicked);
			if (nstate != Cursor::IDLE)
			{
				return nstate;
			}
		}
		return UIElement::send_cursor(clicked, cursorpos);
	}

	Button::State UIEnterNumber::button_pressed(uint16_t buttonid)
	{
		switch (buttonid)
		{
		case OK:
			handlestring(numfield.get_text());
			break;
		}

		active = false;

		return Button::PRESSED;
	}

	void UIEnterNumber::handlestring(std::string numstr)
	{
		if (numstr.size() > 0)
		{
			int32_t num;
			try
			{
				num = std::stoi(numstr);
				if (num >= min && num <= max)
				{
					numhandler(num);
					active = false;
				}
			}
			catch (const std::exception&) {}
		}

		buttons[OK]->set_state(Button::NORMAL);
	}
}