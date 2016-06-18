/////////////////////////////////////////////////////////////////////////////
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
#include "UIChatbar.h"

#include "..\UI.h"

#include "..\Components\MapleButton.h"

#include "..\..\Net\Packets\MessagingPackets.h"

#include <nlnx\nx.hpp>

namespace jrc
{
	UIChatbar::UIChatbar(Point<int16_t> pos)
	{
		position = pos;
		dimension = { 500, 60 };
		chatopen = true;
		chatrows = 4;
		rowpos = 0;
		rowmax = -1;
		lastpos = 0;

		nl::node mainbar = nl::nx::ui["StatusBar2.img"]["mainBar"];

		buttons[BT_OPENCHAT] = std::make_unique<MapleButton>(mainbar["chatOpen"]);
		buttons[BT_CLOSECHAT] = std::make_unique<MapleButton>(mainbar["chatClose"]);
		buttons[BT_SCROLLUP] = std::make_unique<MapleButton>(mainbar["scrollUp"]);
		buttons[BT_SCROLLDOWN] = std::make_unique<MapleButton>(mainbar["scrollDown"]);
		buttons[BT_CHATTARGETS] = std::make_unique<MapleButton>(mainbar["chatTarget"]["base"]);

		buttons[chatopen ? BT_OPENCHAT : BT_CLOSECHAT]->set_active(false);
		buttons[BT_CHATTARGETS]->set_active(chatopen);

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

		nl::node chat = nl::nx::ui["StatusBar2.img"]["chat"];

		tapbar = chat["tapBar"];
		tapbartop = chat["tapBarOver"];

		chatbox = { 502, 1 + chatrows * CHATROWHEIGHT, Geometry::BLACK, 0.6f };

		chatfield = { Text::A11M, Text::LEFT, Text::BLACK, { { -435, -58 }, { -40, -35} }, 0 };
		chatfield.set_state(chatopen ? Textfield::NORMAL : Textfield::DISABLED);
		chatfield.set_enter_callback([&](std::string msg) {

			size_t last = msg.find_last_not_of(' ');
			if (last != std::string::npos)
			{
				msg.erase(last + 1);

				GeneralChatPacket(msg, true).dispatch();

				lastentered.push_back(msg);
				lastpos = lastentered.size();
			}
		});
		chatfield.set_key_callback(Keyboard::UP, [&](){
			if (lastpos > 0)
			{
				lastpos--;
				chatfield.change_text(lastentered[lastpos]);
			}
		});
		chatfield.set_key_callback(Keyboard::DOWN, [&](){
			if (lastentered.size() > 0 && lastpos < lastentered.size() - 1)
			{
				lastpos++;
				chatfield.change_text(lastentered[lastpos]);
			}
		});

		slider = {11, Range<int16_t>(0, CHATROWHEIGHT * chatrows - 14), -22, chatrows, 1, 
			[&](bool up) {
			int16_t next = up ?
				rowpos - 1 :
				rowpos + 1;
			if (next >= 0 && next <= rowmax)
				rowpos = next;
		} };
	}

	void UIChatbar::draw(float inter) const
	{
		chatspace[chatopen].draw(position);
		chatenter.draw(position);

		UIElement::draw(inter);

		if (chatopen)
		{
			tapbartop.draw({ position.x() - 576, getchattop() });
			chatbox.draw({ 0, getchattop() + 2 });

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
				rowtexts.at(rowid).draw({ 4, getchattop() - yshift - 1 });
			}

			slider.draw({ position.x(), getchattop() + 5 });

			chattargets[chattarget].draw(position + Point<int16_t>(0, 2));
			chatcover.draw(position);
			chatfield.draw(position);
		}
		else if (rowtexts.count(rowmax))
		{
			rowtexts.at(rowmax).draw(position + Point<int16_t>(-500, -60));
		}
	}

	void UIChatbar::update()
	{
		UIElement::update();

		chatfield.update(position);
	}

	Button::State UIChatbar::button_pressed(uint16_t id)
	{
		switch (id)
		{
		case BT_OPENCHAT:
			chatopen = true;
			buttons[BT_OPENCHAT]->set_active(false);
			buttons[BT_CLOSECHAT]->set_active(true);
			buttons[BT_CHATTARGETS]->set_active(true);
			chatfield.set_state(Textfield::NORMAL);
			break;
		case BT_CLOSECHAT:
			chatopen = false;
			buttons[BT_OPENCHAT]->set_active(true);
			buttons[BT_CLOSECHAT]->set_active(false);
			buttons[BT_CHATTARGETS]->set_active(false);
			chatfield.set_state(Textfield::DISABLED);
			break;
		}
		return Button::NORMAL;
	}

	bool UIChatbar::is_in_range(Point<int16_t> cursorpos) const
	{
		Point<int16_t> absp(0, getchattop() - 16);
		Point<int16_t> dim(500, chatrows * CHATROWHEIGHT + CHATYOFFSET + 16);
		return Rectangle<int16_t>(absp, absp + dim)
			.contains(cursorpos);
	}

	bool UIChatbar::remove_cursor(bool clicked, Point<int16_t> cursorpos)
	{
		if (slider.remove_cursor(clicked))
			return true;

		return UIElement::remove_cursor(clicked, cursorpos);
	}

	Cursor::State UIChatbar::send_cursor(bool clicking, Point<int16_t> cursorpos)
	{
		if (slider.isenabled())
		{
			auto cursoroffset = cursorpos - Point<int16_t>(position.x(), getchattop() + 5);
			Cursor::State sstate = slider.send_cursor(cursoroffset, clicking);
			if (sstate != Cursor::IDLE)
			{
				return sstate;
			}
		}

		if (chatfield.get_state() == Textfield::NORMAL)
		{
			Cursor::State tstate = chatfield.send_cursor(cursorpos, clicking);
			if (tstate != Cursor::IDLE)
			{
				return tstate;
			}
		}
		
		auto chattop = Rectangle<int16_t>(
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
				while (ydelta > 0 && chatrows > MINCHATROWS)
				{
					chatrows--;
					ydelta -= CHATROWHEIGHT;
				}
				while (ydelta < 0 && chatrows < MAXCHATROWS)
				{
					chatrows++;
					ydelta += CHATROWHEIGHT;
				}
				chatbox.setheight(1 + chatrows * CHATROWHEIGHT);
				slider.setrows(rowpos, chatrows, rowmax);
				slider.setvertical({ 0, CHATROWHEIGHT * chatrows - 14 });
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

		return UIElement::send_cursor(clicking, cursorpos);
	}

	void UIChatbar::send_line(const std::string& line, LineType type)
	{
		rowmax++;
		rowpos = rowmax;

		slider.setrows(rowpos, chatrows, rowmax);

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

		rowtexts.emplace(
			std::piecewise_construct,
			std::forward_as_tuple(rowmax),
			std::forward_as_tuple(Text::A12M, Text::LEFT, color, line, 480)
		);
	}

	int16_t UIChatbar::getchattop() const
	{
		return position.y() - chatrows * CHATROWHEIGHT - CHATYOFFSET;
	}
}