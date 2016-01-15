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
		buttons[chatopen ? BT_OPENCHAT : BT_CLOSECHAT]->setactive(false);
		buttons[BT_SCROLLUP] = unique_ptr<Button>(new MapleButton(mainbar["scrollUp"]));
		buttons[BT_SCROLLDOWN] = unique_ptr<Button>(new MapleButton(mainbar["scrollDown"]));
		buttons[BT_CHATTARGETS] = unique_ptr<Button>(new MapleButton(mainbar["chatTarget"]["base"]));
		buttons[BT_CHATTARGETS]->setactive(chatopen);

		chatspace[false] = Texture(mainbar["chatSpace"]);
		chatspace[true] = Texture(mainbar["chatEnter"]);
		chatenter = Texture(mainbar["chatSpace2"]);
		chatcover = Texture(mainbar["chatCover"]);

		chattargets[CHT_ALL] = Texture(mainbar["chatTarget"]["all"]);
		chattargets[CHT_BUDDY] = Texture(mainbar["chatTarget"]["friend"]);
		chattargets[CHT_GUILD] = Texture(mainbar["chatTarget"]["guild"]);
		chattargets[CHT_ALLIANCE] = Texture(mainbar["chatTarget"]["association"]);
		chattargets[CHT_PARTY] = Texture(mainbar["chatTarget"]["party"]);
		chattargets[CHT_SQUAD] = Texture(mainbar["chatTarget"]["expedition"]);

		rectangle2d<int16_t> enterarea = rectangle2d<int16_t>(
			vector2d<int16_t>(-435, -59),
			vector2d<int16_t>(-40, -35)
			);
		chatfield = Textfield(Text::A11M, Text::LEFT, Text::BLACK, enterarea, 0);
		chatfield.setstate(chatopen ? Textfield::NORMAL : Textfield::DISABLED);
		chatfield.setonreturn(Consumer<string>([](string msg) {
			using Net::GeneralChatPacket;
			Net::Session::dispatch(GeneralChatPacket(msg, true));
		}));

		closedtext = Text(Text::A11M, Text::LEFT, Text::WHITE);
	}

	Chatbar::~Chatbar() {}

	void Chatbar::draw(float inter) const
	{
		using Graphics::DrawArgument;
		chatspace.at(chatopen).draw(DrawArgument(position));
		chatenter.draw(DrawArgument(position));

		UIElement::draw(inter);

		if (chatopen)
		{
			chattargets.at(chattarget).draw(DrawArgument(position + vector2d<int16_t>(0, 2)));
			chatcover.draw(DrawArgument(position));
			chatfield.draw(position);
		}
		else if (lines.size() > 0)
		{
			closedtext.drawline(lines.back(), position + vector2d<int16_t>(-500, -60));
		}
	}

	void Chatbar::update()
	{
		UIElement::update();

		chatfield.update();
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

	Cursor::Mousestate Chatbar::sendmouse(bool down, vector2d<int16_t> pos)
	{
		Cursor::Mousestate ret = UIElement::sendmouse(down, pos);

		if (chatfield.getbounds(position).contains(pos))
		{
			if (down)
			{
				UI::focustextfield(&chatfield);
				chatfield.setstate(Textfield::FOCUSED);
			}
			else if (chatfield.getstate() == Textfield::NORMAL)
			{
				ret = Cursor::MST_CANCLICK;
			}
		}
		else if (down)
		{
			UI::focustextfield(nullptr);
			chatfield.setstate(Textfield::NORMAL);
		}

		return ret;
	}

	void Chatbar::sendline(string line, int8_t)
	{
		lines.push_back(line);
	}
}