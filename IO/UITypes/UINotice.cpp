//////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2016 Daniel Allendorf                                        //
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

#include "IO\Components\MapleButton.h"

#include "nlnx\nx.hpp"

namespace IO
{
	UINotice::UINotice(string q)
	{
		node src = nl::nx::ui["Basic.img"]["Notice6"];

		top = src["t"];
		center = src["c"];
		centerbox = src["c_box"];
		box = src["box"];
		box2 = src["box2"];
		bottom = src["s"];
		bottombox = src["s_box"];

		question = Text(Text::A11M, Text::CENTER, Text::DARKGREY);
		question.settext(q, 200);

		height = question.height();
		dimension = Point<int16_t>(top.width(), top.height() + height + bottom.height());
		position = Point<int16_t>(400 - dimension.x() / 2, 240 - dimension.y() / 2);
	}

	void UINotice::draw(bool textfield) const
	{
		Point<int16_t> start = position;

		top.draw(start);
		start.shifty(top.height());
		centerbox.draw(start);
		start.shifty(centerbox.height());

		Point<int16_t> textpos = start;
		using Graphics::DrawArgument;
		box.draw(DrawArgument(textpos, Point<int16_t>(0, height)));
		start.shifty(box.height() * (height / box.height()));
		box.draw(start);
		start.shifty(box.height());
		question.draw(textpos + Point<int16_t>(130, -3));

		if (textfield)
		{
			box2.draw(start);
			start.shifty(box2.height());
		}
		box.draw(start);
		start.shifty(box.height());
		bottombox.draw(start);
	}

	int16_t UINotice::box2offset() const
	{
		return top.height() + centerbox.height() + box.height() * (1 + height / box.height());
	}


	UIYesNo::UIYesNo(string q, function<void(bool)> yh)
		: UINotice(q) {

		yesnohandler = yh;

		int16_t belowtext = UINotice::box2offset();

		node src = nl::nx::ui["Basic.img"];

		buttons[YES] = unique_ptr<Button>(new MapleButton(src["BtOK4"], 90, belowtext + 1));
		buttons[NO] = unique_ptr<Button>(new MapleButton(src["BtCancel4"], 132, belowtext + 1));
	}

	void UIYesNo::draw(float alpha) const
	{
		UINotice::draw(false);
		UIElement::draw(alpha);
	}

	void UIYesNo::buttonpressed(uint16_t buttonid)
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
	}


	UIEnterNumber::UIEnterNumber(string q, function<void(int32_t)> nh, int32_t mi, int32_t ma, int32_t de)
		: UINotice(q) {

		numhandler = nh;
		min = mi;
		max = ma;

		int16_t belowtext = UINotice::box2offset();

		node src = nl::nx::ui["Basic.img"];

		buttons[OK] = unique_ptr<Button>(new MapleButton(src["BtOK4"], 90, belowtext + 21));
		buttons[CANCEL] = unique_ptr<Button>(new MapleButton(src["BtCancel4"], 132, belowtext + 21));

		auto area = rectangle2d<int16_t>(26, 232, belowtext, belowtext + 20);
		numfield = Textfield(Text::A11M, Text::LEFT, Text::LIGHTGREY, area, 9);
		numfield.setstate(Textfield::FOCUSED);
		numfield.settext(std::to_string(de));
		numfield.setonreturn([&](string numstr){
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

	Cursor::State UIEnterNumber::sendmouse(bool clicked, Point<int16_t> cursorpos)
	{
		if (numfield.getstate() == Textfield::NORMAL)
		{
			Cursor::State nstate = numfield.sendcursor(cursorpos, clicked);
			if (nstate != Cursor::IDLE)
			{
				return nstate;
			}
		}
		return UIElement::sendmouse(clicked, cursorpos);
	}

	void UIEnterNumber::buttonpressed(uint16_t buttonid)
	{
		if (buttonid == OK)
		{
			string numstr = numfield.gettext();
			handlestring(numstr);
		}
		else
		{
			active = false;
		}
	}

	void UIEnterNumber::handlestring(string numstr)
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

		buttons[OK]->setstate(Button::NORMAL);
	}
}