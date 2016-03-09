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
	using Net::Session;

	Chatbar::Chatbar(Point<int16_t> pos)
	{
		position = pos;
		dimension = Point<int16_t>(500, 60);
		chatopen = true;
		chatrows = 4;
		rowpos = 0;
		rowmax = -1;
		lastpos = 0;

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

		node chat = nl::nx::ui["StatusBar2.img"]["chat"];

		tapbar = chat["tapBar"];
		tapbartop = chat["tapBarOver"];

		chatfield = Textfield(Text::A11M, Text::LEFT, Text::BLACK, 
			rectangle2d<int16_t>(
			Point<int16_t>(-435, -58),
			Point<int16_t>(-40, -35)
			), 0);
		chatfield.setstate(chatopen ? Textfield::NORMAL : Textfield::DISABLED);
		chatfield.setonreturn([&](string msg) {
			using Net::GeneralChatPacket;
			Session::get().dispatch(GeneralChatPacket(msg, true));

			lastentered.push_back(msg);
			lastpos = lastentered.size();
		});
		chatfield.setkey(Keyboard::UP, [&](){
			if (lastpos > 0)
			{
				lastpos--;
				chatfield.settext(lastentered[lastpos]);
			}
		});
		chatfield.setkey(Keyboard::DOWN, [&](){
			if (lastentered.size() > 0 && lastpos < lastentered.size() - 1)
			{
				lastpos++;
				chatfield.settext(lastentered[lastpos]);
			}
		});

		slider = unique_ptr<Slider>(
			new Slider(11, Range<int16_t>(0, tapbarheight * chatrows - 14), -22, 1, [&](bool up){
			int16_t next = up ? 
				rowpos - 1 : 
				rowpos + 1;
			if (next >= 0 && next <= rowmax)
				rowpos = next;
		}));
	}

	Chatbar::~Chatbar() {}

	void Chatbar::draw(float inter) const
	{
		chatspace[chatopen].draw(position);
		chatenter.draw(position);

		UIElement::draw(inter);

		if (chatopen)
		{
			using Graphics::DrawArgument;
			int16_t chatheight = tapbarheight * chatrows;
			Point<int16_t> tabpos = position - Point<int16_t>(576, chatheight + 65);
			tapbartop.draw(tabpos);
			tabpos.shifty(2);
			for (int16_t i = 0; i < chatrows; i++)
			{
				Point<int16_t> startpos = tabpos + Point<int16_t>(70, -1);
				for (int16_t j = 0; j < tapbarheight; j++)
				{
					tapbar.draw(DrawArgument(tabpos, 0.5f));
					tabpos.shifty(1);
				}
				int16_t rowid = rowpos + i - chatrows + 1;
				if (rowtexts.count(rowid))
				{
					rowtexts.at(rowid).draw(startpos);
				}
			}
			slider->draw(position + Point<int16_t>(0, - chatheight - 60));

			chattargets[chattarget].draw(position + Point<int16_t>(0, 2));
			chatcover.draw(position);
			chatfield.draw(position);
		}
		else if (rowtexts.count(rowmax))
		{
			rowtexts.at(rowmax).draw(position + Point<int16_t>(-500, -58));
		}
	}

	void Chatbar::update()
	{
		UIElement::update();

		chatfield.update(position);
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
			chatfield.setstate(Textfield::NORMAL);
			//config.getconfig()->chatopen = true;
			break;
		case BT_CLOSECHAT:
			chatopen = false;
			buttons[BT_OPENCHAT]->setactive(true);
			buttons[BT_CLOSECHAT]->setactive(false);
			buttons[BT_CHATTARGETS]->setactive(false);
			chatfield.setstate(Textfield::DISABLED);
			//config.getconfig()->chatopen = false;
			break;
		}

		buttons[id]->setstate(Button::NORMAL);
	}

	rectangle2d<int16_t> Chatbar::bounds() const
	{
		Point<int16_t> absp = position - Point<int16_t>(512, 90);
		return rectangle2d<int16_t>(absp, absp + dimension);
	}

	Cursor::State Chatbar::sendmouse(bool down, Point<int16_t> cursorpos)
	{
		if (slider && slider->isenabled())
		{
			Cursor::State sstate = slider->sendcursor(cursorpos - Point<int16_t>(512, 480), down);
			if (sstate != Cursor::IDLE)
			{
				return sstate;
			}
		}

		if (chatfield.getstate() == Textfield::NORMAL)
		{
			Cursor::State tstate = chatfield.sendcursor(cursorpos, down);
			if (tstate != Cursor::IDLE)
			{
				return tstate;
			}
		}

		return UIElement::sendmouse(down, cursorpos);
	}

	void Chatbar::sendline(string line, LineType type)
	{
		rowmax++;
		rowpos = rowmax;

		Text::Color color;
		switch (type)
		{
		case RED:
			color = Text::RED;
			break;
		case BLUE:
			color = Text::MEDIUMBLUE;
			break;
		default:
			color = Text::WHITE;
			break;
		}
		rowtexts[rowmax] = Text(Text::A11M, Text::LEFT, color);
		rowtexts[rowmax].settext(line);
	}
}