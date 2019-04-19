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
#include "UIChatbar.h"

#include "../Components/MapleButton.h"

#include "../Net/Packets/MessagingPackets.h"

#include "nlnx/nx.hpp"

namespace jrc
{
	UIChatbar::UIChatbar() : UIDragElement<PosCHAT>(Point<int16_t>(410, -5))
	{
		chatopen = Setting<Chatopen>::get().load();
		chatrows = 5;
		lastpos = 0;
		rowpos = 0;
		rowmax = -1;

		nl::node chat = nl::nx::ui["StatusBar3.img"]["chat"];
		nl::node ingame = chat["ingame"];
		nl::node view = ingame["view"];
		nl::node input = ingame["input"];
		nl::node chatTarget = chat["common"]["chatTarget"];

		chatspace[0] = view["min"]["top"];
		chatspace[1] = view["min"]["center"];
		chatspace[2] = view["min"]["bottom"];
		chatspace[3] = view["drag"];

		int16_t chattop_y = getchattop(true) - 33;
		closechat = Point<int16_t>(387, 21);

		buttons[BT_OPENCHAT] = std::make_unique<MapleButton>(view["btMax"], Point<int16_t>(391, -7));
		buttons[BT_CLOSECHAT] = std::make_unique<MapleButton>(view["btMin"], closechat + Point<int16_t>(0, chattop_y));
		buttons[BT_CHAT] = std::make_unique<MapleButton>(input["button:chat"], Point<int16_t>(344, -8));
		buttons[BT_LINK] = std::make_unique<MapleButton>(input["button:itemLink"], Point<int16_t>(365, -8));
		buttons[BT_HELP] = std::make_unique<MapleButton>(input["button:help"], Point<int16_t>(386, -8));

		buttons[chatopen ? BT_OPENCHAT : BT_CLOSECHAT]->set_active(false);
		buttons[BT_CHAT]->set_active(chatopen ? true : false);
		buttons[BT_LINK]->set_active(chatopen ? true : false);
		buttons[BT_HELP]->set_active(chatopen ? true : false);

		chattab_x = 6;
		chattab_y = chattop_y;
		chattab_span = 54;

		for (size_t i = 0; i < NUM_CHATTAB; i++)
		{
			buttons[BT_TAB_0 + i] = std::make_unique<MapleButton>(view["tab"], Point<int16_t>(chattab_x + (i * chattab_span), chattab_y));
			buttons[BT_TAB_0 + i]->set_active(chatopen ? true : false);
			chattab_text[CHT_ALL + i] = Text(Text::Font::A12M, Text::Alignment::CENTER, Text::Color::LIGHTERGREY, ChatTabText[i]);
		}

		chattab_text[CHT_ALL].change_color(Text::Color::WHITE);

		buttons[BT_TAB_0 + NUM_CHATTAB] = std::make_unique<MapleButton>(view["btAddTab"], Point<int16_t>(chattab_x + (NUM_CHATTAB * chattab_span), chattab_y));
		buttons[BT_TAB_0 + NUM_CHATTAB]->set_active(chatopen ? true : false);

		buttons[BT_CHAT_TARGET] = std::make_unique<MapleButton>(chatTarget["all"], Point<int16_t>(5, -8));
		buttons[BT_CHAT_TARGET]->set_active(chatopen ? true : false);

		chatenter = input["layer:chatEnter"];
		chatcover = input["layer:backgrnd"];

		chatfield = Textfield(Text::A11M, Text::LEFT, Text::WHITE, Rectangle<int16_t>(Point<int16_t>(62, -9), Point<int16_t>(330, 8)), 0);
		chatfield.set_state(chatopen ? Textfield::State::NORMAL : Textfield::State::DISABLED);

		chatfield.set_enter_callback(
			[&](std::string msg) {
				size_t last = msg.find_last_not_of(' ');

				if (last != std::string::npos)
				{
					msg.erase(last + 1);

					GeneralChatPacket(msg, true).dispatch();

					lastentered.push_back(msg);
					lastpos = lastentered.size();
				}
			}
		);

		chatfield.set_key_callback(
			KeyAction::UP, [&]() {
				if (lastpos > 0)
				{
					lastpos--;
					chatfield.change_text(lastentered[lastpos]);
				}
			}
		);

		chatfield.set_key_callback(
			KeyAction::DOWN, [&]() {
				if (lastentered.size() > 0 && lastpos < lastentered.size() - 1)
				{
					lastpos++;
					chatfield.change_text(lastentered[lastpos]);
				}
			}
		);

		//int16_t slider_x = 394;
		//int16_t slider_y = -80;
		//int16_t slider_height = slider_y + 56;
		//int16_t slider_unitrows = chatrows;
		//int16_t slider_rowmax = 1;
		//slider = Slider(Slider::Type::CHATBAR, Range<int16_t>(slider_y, slider_height), slider_x, slider_unitrows, slider_rowmax, [&](bool upwards) {});

		send_chatline("[Welcome] Welcome to MapleStory!!", LineType::YELLOW);

		dimension = Point<int16_t>(410, DIMENSION_Y);
		active = true;

		if (chatopen)
			dimension.shift_y(getchatbarheight());
	}

	void UIChatbar::draw(float inter) const
	{
		UIElement::draw_sprites(inter);

		int16_t chattop = getchattop(chatopen);

		if (chatopen)
		{
			chatspace[0].draw(position + Point<int16_t>(0, chattop));

			if (chatrows > 1)
				chatspace[1].draw(DrawArgument(position + Point<int16_t>(0, -28), Point<int16_t>(0, 28 + chattop)));

			chatspace[2].draw(position + Point<int16_t>(0, -28));
			chatspace[3].draw(position + Point<int16_t>(0, -15 + chattop));

			chatcover.draw(DrawArgument(position + Point<int16_t>(0, -13), Point<int16_t>(409, 0)));
			chatenter.draw(DrawArgument(position + Point<int16_t>(0, -13), Point<int16_t>(285, 0)));
			chatfield.draw(position + Point<int16_t>(-4, -4));
			//slider.draw(position);

			int16_t yshift = chattop;

			for (size_t i = 0; i < chatrows; i++)
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

				rowtexts.at(rowid).draw(position + Point<int16_t>(9, getchattop(chatopen) - yshift - 21));
			}

			UIElement::draw_buttons(inter);

			for (size_t i = 0; i < NUM_CHATTAB; i++)
				chattab_text[CHT_ALL + i].draw(position + Point<int16_t>(chattab_x + (i * chattab_span) + 25, chattab_y - 3));
		}
		else
		{
			chatspace[0].draw(position + Point<int16_t>(0, -1));
			chatspace[1].draw(position + Point<int16_t>(0, -1));
			chatspace[2].draw(position);
			chatspace[3].draw(position + Point<int16_t>(0, -16));

			if (rowtexts.count(rowmax))
				rowtexts.at(rowmax).draw(position + Point<int16_t>(9, -6));

			UIElement::draw_buttons(inter);
		}
	}

	void UIChatbar::update()
	{
		UIElement::update();

		for (size_t i = 0; i < NUM_CHATTAB; i++)
			buttons[BT_TAB_0 + i]->set_position(Point<int16_t>(chattab_x + (i * chattab_span), chattab_y));

		buttons[BT_TAB_0 + NUM_CHATTAB]->set_position(Point<int16_t>(chattab_x + (NUM_CHATTAB * chattab_span), chattab_y));
		buttons[BT_CLOSECHAT]->set_position(closechat + Point<int16_t>(0, chattab_y));

		chatfield.update(position);

		for (auto iter : message_cooldowns)
			iter.second -= Constants::TIMESTEP;
	}

	void UIChatbar::send_key(int32_t keycode, bool pressed) {}

	bool UIChatbar::is_in_range(Point<int16_t> cursorpos) const
	{
		auto bounds = getbounds(dimension);
		return bounds.contains(cursorpos);
	}

	Cursor::State UIChatbar::send_cursor(bool clicking, Point<int16_t> cursorpos)
	{
		if (chatopen)
		{
			if (Cursor::State new_state = chatfield.send_cursor(cursorpos, clicking))
				return new_state;

			return check_dragtop(clicking, cursorpos);
		}
		else
		{
			return UIDragElement::send_cursor(clicking, cursorpos);
		}
	}

	Cursor::State UIChatbar::check_dragtop(bool clicking, Point<int16_t> cursorpos)
	{
		Rectangle<int16_t> chattop = getbounds(dragarea);
		Point<int16_t> chattop_lt = chattop.getlt();
		Point<int16_t> chattop_rb = chattop.getrb();
		int16_t chattop_rb_y = chattop_rb.y();

		auto chattop_rb_adj = Point<int16_t>(chattop_rb.x(), chattop_rb_y - 5);

		if (chatopen)
			chattop = Rectangle<int16_t>(Point<int16_t>(chattop_lt.x(), chattop_lt.y() + 20), chattop_rb_adj);
		else
			chattop = Rectangle<int16_t>(chattop_lt, chattop_rb_adj);

		bool contains = chattop.contains(cursorpos);

		if (dragchattop)
		{
			if (clicking)
			{
				int16_t ydelta = cursorpos.y() - chattop_rb_y + 10;

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

				//slider.setrows(rowpos, chatrows, rowmax);
				//slider.setvertical(Range<int16_t>(0, CHATROWHEIGHT * chatrows - 14));

				chattab_y = getchattop(chatopen) - 33;
				dimension.set_y(getchatbarheight());

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
				return Cursor::CHATBARVDRAG;
			}
		}

		return UIDragElement::send_cursor(clicking, cursorpos);
	}

	bool UIChatbar::indragrange(Point<int16_t> cursorpos) const
	{
		auto bounds = getbounds(dragarea);
		return bounds.contains(cursorpos);
	}

	void UIChatbar::send_chatline(const std::string& line, LineType type)
	{
		rowmax++;
		rowpos = rowmax;

		//slider.setrows(rowpos, chatrows, rowmax);

		Text::Color color;

		switch (type)
		{
		case RED:
			color = Text::Color::DARKRED;
			break;
		case BLUE:
			color = Text::Color::MEDIUMBLUE;
			break;
		case YELLOW:
			color = Text::Color::YELLOW;
			break;
		default:
			color = Text::Color::WHITE;
			break;
		}

		rowtexts.emplace(
			std::piecewise_construct,
			std::forward_as_tuple(rowmax),
			std::forward_as_tuple(Text::Font::A11M, Text::Alignment::LEFT, color, line, 480)
		);
	}

	void UIChatbar::display_message(Messages::Type line, UIChatbar::LineType type)
	{
		if (message_cooldowns[line] > 0)
			return;

		std::string message{ Messages::messages[line] };
		send_chatline(message, type);

		message_cooldowns[line] = MESSAGE_COOLDOWN;
	}

	Button::State UIChatbar::button_pressed(uint16_t buttonid)
	{
		switch (buttonid)
		{
		case BT_OPENCHAT:
			chatopen = true;
			buttons[BT_OPENCHAT]->set_active(false);
			buttons[BT_CLOSECHAT]->set_active(true);
			buttons[BT_CHAT]->set_active(true);
			buttons[BT_HELP]->set_active(true);
			buttons[BT_LINK]->set_active(true);

			for (size_t i = 0; i < NUM_CHATTAB; i++)
				buttons[BT_TAB_0 + i]->set_active(true);

			buttons[BT_TAB_0 + NUM_CHATTAB]->set_active(true);
			buttons[BT_CHAT_TARGET]->set_active(true);

			chatfield.set_state(Textfield::State::NORMAL);

			dimension.shift_y(getchatbarheight());
			break;
		case BT_CLOSECHAT:
			chatopen = false;
			buttons[BT_OPENCHAT]->set_active(true);
			buttons[BT_CLOSECHAT]->set_active(false);
			buttons[BT_CHAT]->set_active(false);
			buttons[BT_HELP]->set_active(false);
			buttons[BT_LINK]->set_active(false);

			for (size_t i = 0; i < NUM_CHATTAB; i++)
				buttons[BT_TAB_0 + i]->set_active(false);

			buttons[BT_TAB_0 + NUM_CHATTAB]->set_active(false);
			buttons[BT_CHAT_TARGET]->set_active(false);

			chatfield.set_state(Textfield::State::DISABLED);

			dimension.set_y(DIMENSION_Y);
			break;
		case BT_TAB_0:
		case BT_TAB_1:
		case BT_TAB_2:
		case BT_TAB_3:
		case BT_TAB_4:
		case BT_TAB_5:
			for (size_t i = 0; i < NUM_CHATTAB; i++)
			{
				buttons[BT_TAB_0 + i]->set_state(Button::State::NORMAL);
				chattab_text[CHT_ALL + i].change_color(Text::Color::LIGHTERGREY);
			}

			chattab_text[buttonid - BT_TAB_0].change_color(Text::Color::WHITE);

			return Button::State::PRESSED;
		}

		Setting<Chatopen>::get().save(chatopen);

		return Button::State::NORMAL;
	}

	int16_t UIChatbar::getchattop(bool chat_open) const
	{
		if (chat_open)
			return getchatbarheight() * -1;
		else
			return -1;
	}

	int16_t UIChatbar::getchatbarheight() const
	{
		return 15 + chatrows * CHATROWHEIGHT;
	}

	Rectangle<int16_t> UIChatbar::getbounds(Point<int16_t> additional_area) const
	{
		int16_t screen_adj = (chatopen) ? 35 : 16;

		auto absp = position + Point<int16_t>(0, getchattop(chatopen));
		auto da = absp + additional_area;

		absp = Point<int16_t>(absp.x(), absp.y() - screen_adj);
		da = Point<int16_t>(da.x(), da.y());

		return Rectangle<int16_t>(absp, da);
	}
}