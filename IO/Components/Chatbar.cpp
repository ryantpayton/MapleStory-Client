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
#include "Chatbar.h"
#include "MapleButton.h"
#include "Net\Session.h"
#include "Net\Packets\MessagingPackets.h"
#include "IO\UI.h"
#include "nlnx\nx.hpp"

namespace IO
{
	Chatbar::Chatbar(vector2d<int16_t> pos)
	{
		position = pos;
		dimension = vector2d<int16_t>(500, 60);
		chatopen = true;

		node mainbar = nl::nx::ui["StatusBar2.img"]["mainBar"];

		buttons[BT_OPENCHAT] = unique_ptr<Button>(new MapleButton(mainbar["chatOpen"]));
		buttons[BT_CLOSECHAT] = unique_ptr<Button>(new MapleButton(mainbar["chatClose"]));
		buttons[BT_SCROLLUP] = unique_ptr<Button>(new MapleButton(mainbar["scrollUp"]));
		buttons[BT_SCROLLDOWN] = unique_ptr<Button>(new MapleButton(mainbar["scrollDown"]));
		buttons[BT_CHATTARGETS] = unique_ptr<Button>(new MapleButton(mainbar["chatTarget"]["base"]));

		buttons[chatopen ? BT_OPENCHAT : BT_CLOSECHAT]->setactive(false);
		buttons[BT_CHATTARGETS]->setactive(chatopen);

		chatspace[false] = mainbar["chatSpace"];
		chatspace[true] = mainbar["chatEnter"];
		chatenter = mainbar["chatSpace2"];
		chatcover = mainbar["chatCover"];

		chattargets[CHT_ALL] = mainbar["chatTarget"]["all"];
		chattargets[CHT_BUDDY] = mainbar["chatTarget"]["friend"];
		chattargets[CHT_GUILD] = mainbar["chatTarget"]["guild"];
		chattargets[CHT_ALLIANCE] = mainbar["chatTarget"]["association"];
		chattargets[CHT_PARTY] = mainbar["chatTarget"]["party"];
		chattargets[CHT_SQUAD] = mainbar["chatTarget"]["expedition"];

		rectangle2d<int16_t> enterarea = rectangle2d<int16_t>(
			vector2d<int16_t>(-435, -59),
			vector2d<int16_t>(-40, -35)
			);
		chatfield = Textfield(Text::A11M, Text::LEFT, Text::BLACK, enterarea, 0);
		chatfield.setstate(chatopen ? Textfield::NORMAL : Textfield::DISABLED);
		chatfield.setonreturn([](string msg) {
			using Net::GeneralChatPacket;
			Net::Session::get().dispatch(GeneralChatPacket(msg, true));
		});

		closedtext = Text(Text::A11M, Text::LEFT, Text::WHITE);
	}

	Chatbar::~Chatbar() {}

	void Chatbar::draw(float inter) const
	{
		chatspace[chatopen].draw(position);
		chatenter.draw(position);

		UIElement::draw(inter);

		if (chatopen)
		{
			chattargets[chattarget].draw(position + vector2d<int16_t>(0, 2));
			chatcover.draw(position);
			chatfield.draw(position);
		}
		else if (lines.size() > 0)
		{
			closedtext.draw(position + vector2d<int16_t>(-500, -60));
		}
	}

	void Chatbar::update()
	{
		UIElement::update();

		chatfield.update();

		if (lines.size() > 0)
			closedtext.settext(lines.back());
	}

	void Chatbar::buttonpressed(uint16_t id)
	{
		switch (id)
		{
		case BT_OPENCHAT:
			chatopen = true;
			buttons[BT_OPENCHAT]->setactive(false);
			buttons[BT_CLOSECHAT]->setactive(true);
			buttons[BT_CHATTARGETS]->setactive(true);
			chatfield.setstate(Textfield::DISABLED);
			//config.getconfig()->chatopen = true;
			break;
		case BT_CLOSECHAT:
			chatopen = false;
			buttons[BT_OPENCHAT]->setactive(true);
			buttons[BT_CLOSECHAT]->setactive(false);
			buttons[BT_CHATTARGETS]->setactive(false);
			chatfield.setstate(Textfield::NORMAL);
			//config.getconfig()->chatopen = false;
			break;
		}

		buttons[id]->setstate(Button::NORMAL);
	}

	rectangle2d<int16_t> Chatbar::bounds() const
	{
		return rectangle2d<int16_t>(
			position - vector2d<int16_t>(512, 90),
			position - vector2d<int16_t>(512, 90) + dimension
			);
	}

	Cursor::State Chatbar::sendmouse(bool down, vector2d<int16_t> pos)
	{
		Cursor::State ret = UIElement::sendmouse(down, pos);

		if (chatfield.getbounds(position).contains(pos))
		{
			if (down)
			{
				UI::get().focustextfield(&chatfield);
				chatfield.setstate(Textfield::FOCUSED);
			}
			else if (chatfield.getstate() == Textfield::NORMAL)
			{
				ret = Cursor::CANCLICK;
			}
		}
		else if (down)
		{
			UI::get().focustextfield(nullptr);
			chatfield.setstate(Textfield::NORMAL);
		}

		return ret;
	}

	void Chatbar::sendline(string line, int8_t)
	{
		lines.push_back(line);
	}
}