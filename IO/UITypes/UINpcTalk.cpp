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
#include "UINpcTalk.h"

#include "../Components/MapleButton.h"

#include "../Net/Packets/NpcInteractionPackets.h"

#include <nlnx/nx.hpp>

namespace jrc
{
	UINpcTalk::UINpcTalk()
	{
		nl::node src = nl::nx::ui["UIWindow2.img"]["UtilDlgEx"];

		top = src["t"];
		fill = src["c"];
		bottom = src["s"];
		nametag = src["bar"];

		buttons[Buttons::OK] = std::make_unique<MapleButton>(src["BtOK"]);
		buttons[Buttons::NEXT] = std::make_unique<MapleButton>(src["BtNext"]);
		buttons[Buttons::PREV] = std::make_unique<MapleButton>(src["BtPrev"]);
		buttons[Buttons::END] = std::make_unique<MapleButton>(src["BtClose"]);
		buttons[Buttons::YES] = std::make_unique<MapleButton>(src["BtYes"]);
		buttons[Buttons::NO] = std::make_unique<MapleButton>(src["BtNo"]);

		active = false;
	}

	void UINpcTalk::draw(float inter) const
	{
		Point<int16_t> drawpos = position;
		top.draw(drawpos);
		drawpos.shift_y(top.height());
		fill.draw(DrawArgument(drawpos, Point<int16_t>(0, vtile) * fill.height()));
		drawpos.shift_y(vtile * fill.height());
		bottom.draw(drawpos);

		UIElement::draw(inter);

		speaker.draw(DrawArgument(position + Point<int16_t>(80, 100), true));
		nametag.draw(position + Point<int16_t>(25, 100));
		name.draw(position + Point<int16_t>(80, 99));
		text.draw(position + Point<int16_t>(156, 16 + ((vtile * fill.height() - text.height()) / 2)));
	}

	Button::State UINpcTalk::button_pressed(uint16_t buttonid)
	{
		switch (buttonid)
		{
		case Buttons::END:
			NpcTalkMorePacket(type, 0).dispatch();
			active = false;
			break;
		}

		return Button::State::PRESSED;
	}

	void UINpcTalk::change_text(int32_t npcid, int8_t msgtype, int16_t, int8_t speakerbyte, const std::string& tx)
	{
		text = Text(Text::Font::A12M, Text::Alignment::LEFT, Color::Name::DARKGREY, tx, 320);

		if (speakerbyte == 0)
		{
			std::string strid = std::to_string(npcid);
			strid.insert(0, 7 - strid.size(), '0');
			strid.append(".img");
			speaker = nl::nx::npc[strid]["stand"]["0"];
			std::string namestr = nl::nx::string["Npc.img"][std::to_string(npcid)]["name"];
			name = Text(Text::Font::A11M, Text::Alignment::CENTER, Color::Name::WHITE, namestr);
		}
		else
		{
			speaker = Texture();
			name.change_text("");
		}

		vtile = 8;
		height = vtile * fill.height();

		for (auto& button : buttons)
		{
			button.second->set_active(false);
			button.second->set_state(Button::State::NORMAL);
		}

		buttons[Buttons::END]->set_position(Point<int16_t>(20, height + 48));
		buttons[Buttons::END]->set_active(true);

		switch (msgtype)
		{
		case 0:
			buttons[Buttons::OK]->set_position(Point<int16_t>(220, height + 48));
			buttons[Buttons::OK]->set_active(true);
			break;
		}

		type = msgtype;

		position = Point<int16_t>(400 - top.width() / 2, 240 - height / 2);
		dimension = Point<int16_t>(top.width(), height + 120);
	}
}