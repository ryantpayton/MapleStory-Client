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

#include "IO\UI.h"
#include "Net\Packets\MessagingPackets.h"

#include "nlnx\nx.hpp"

//temporary
#include "Gameplay\Stage.h"

namespace IO
{
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

		chatbox = Rectangle(502, 1 + chatrows * CHATROWHEIGHT, Geometry::BLACK, 0.6f);

		chatfield = Textfield(Text::A11M, Text::LEFT, Text::BLACK, 
			rectangle2d<int16_t>(
			Point<int16_t>(-435, -58),
			Point<int16_t>(-40, -35)
			), 0);
		chatfield.setstate(chatopen ? Textfield::NORMAL : Textfield::DISABLED);
		chatfield.setonreturn([&](string msg) {
			// temporary
			if (msg[0] == 's')
			{
				int32_t id = std::stoi(msg.substr(1));
				Gameplay::Stage::get().sendkey(Keyboard::SKILL, id, true);
			}
			else
			{
				using Net::GeneralChatPacket;
				GeneralChatPacket(msg, true).dispatch();
			}

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
			new Slider(11, Range<int16_t>(0, CHATROWHEIGHT * chatrows - 14), -22, chatrows, 1, [&](bool up){
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
			tapbartop.draw(Point<int16_t>(position.x() - 576, getchattop()));
			chatbox.draw(Point<int16_t>(0, getchattop() + 2));

			int16_t chatheight = CHATROWHEIGHT * chatrows;
			int16_t yshift = -chatheight;
			for (int16_t i = 0; i < chatrows; i++)
			{
				int16_t rowid = rowpos - i;
				if (!rowtexts.count(rowid))
					break;

				int16_t textheight = rowtexts.at(rowid).height() / CHATROWHEIGHT;
				while (textheight > 0)
				{
					yshift += CHATROWHEIGHT;
					textheight--;
				}
				rowtexts.at(rowid).draw(Point<int16_t>(4, getchattop() - yshift - 1));
			}

			slider->draw(Point<int16_t>(position.x(), getchattop() + 5));

			chattargets[chattarget].draw(position + Point<int16_t>(0, 2));
			chatcover.draw(position);
			chatfield.draw(position);
		}
		else if (rowtexts.count(rowmax))
		{
			rowtexts.at(rowmax).draw(position + Point<int16_t>(-500, -60));
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
			break;
		case BT_CLOSECHAT:
			chatopen = false;
			buttons[BT_OPENCHAT]->setactive(true);
			buttons[BT_CLOSECHAT]->setactive(false);
			buttons[BT_CHATTARGETS]->setactive(false);
			chatfield.setstate(Textfield::DISABLED);
			break;
		}

		buttons[id]->setstate(Button::NORMAL);
	}

	bool Chatbar::isinrange(Point<int16_t> cursorpos) const
	{
		auto absp = Point<int16_t>(0, getchattop());
		auto dim = Point<int16_t>(500, chatrows * CHATROWHEIGHT + CHATYOFFSET);
		return rectangle2d<int16_t>(absp, absp + dim).contains(cursorpos);
	}

	Cursor::State Chatbar::sendmouse(bool clicking, Point<int16_t> cursorpos)
	{
		if (slider && slider->isenabled())
		{
			auto cursoroffset = cursorpos - Point<int16_t>(position.x(), getchattop() + 5);
			Cursor::State sstate = slider->sendcursor(cursoroffset, clicking);
			if (sstate != Cursor::IDLE)
			{
				return sstate;
			}
		}

		if (chatfield.getstate() == Textfield::NORMAL)
		{
			Cursor::State tstate = chatfield.sendcursor(cursorpos, clicking);
			if (tstate != Cursor::IDLE)
			{
				return tstate;
			}
		}
		
		auto chattop = rectangle2d<int16_t>(
			0,  502, 
			getchattop(),
			getchattop() + 6
			);
		bool contains = chattop.contains(cursorpos);
		if (dragchattop)
		{
			if (clicking)
			{
				int16_t ydelta = cursorpos.y() - getchattop();
				while (ydelta > 0)
				{
					chatrows--;
					ydelta -= CHATROWHEIGHT;
				}
				while (ydelta < 0)
				{
					chatrows++;
					ydelta += CHATROWHEIGHT;
				}
				chatbox.setheight(1 + chatrows * CHATROWHEIGHT);
				slider->setvertical(Range<int16_t>(0, CHATROWHEIGHT * chatrows - 14));
				slider->setrows(chatrows, rowmax);
				return Cursor::CLICKING;
			}
			else
			{
				dragchattop = false;
			}
		}
		else if (contains)
		{
			if (clicking)
			{
				dragchattop = true;
				return Cursor::CLICKING;
			}
			else
			{
				return Cursor::CANCLICK;
			}
		}

		return UIElement::sendmouse(clicking, cursorpos);
	}

	void Chatbar::sendline(string line, LineType type)
	{
		rowmax++;
		rowpos = rowmax;

		slider->setrows(chatrows, rowmax);

		Text::Color color;
		switch (type)
		{
		case RED:
			color = Text::DARKRED;
			break;
		case BLUE:
			color = Text::MEDIUMBLUE;
			break;
		case YELLOW:
			color = Text::YELLOW;
			break;
		default:
			color = Text::WHITE;
			break;
		}
		rowtexts[rowmax] = Text(Text::A12M, Text::LEFT, color);
		rowtexts[rowmax].settext(line, 480);
	}

	int16_t Chatbar::getchattop() const
	{
		return position.y() - chatrows * CHATROWHEIGHT - CHATYOFFSET;
	}
}