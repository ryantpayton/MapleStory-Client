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
#include "UINpcTalk.h"

#include "IO\Components\MapleButton.h"
#include "Net\Packets\NpcInteractionPackets.h"

#include "nlnx\nx.hpp"
#include "nlnx\node.hpp"

namespace IO
{
	UINpcTalk::UINpcTalk()
	{
		node src = nl::nx::ui["UIWindow2.img"]["UtilDlgEx"];

		top = src["t"];
		fill = src["c"];
		bottom = src["s"];
		nametag = src["bar"];

		buttons[OK] = unique_ptr<MapleButton>(new MapleButton(src["BtOK"]));
		buttons[NEXT] = unique_ptr<MapleButton>(new MapleButton(src["BtNext"]));
		buttons[PREV] = unique_ptr<MapleButton>(new MapleButton(src["BtPrev"]));
		buttons[END] = unique_ptr<MapleButton>(new MapleButton(src["BtClose"]));
		buttons[YES] = unique_ptr<MapleButton>(new MapleButton(src["BtYes"]));
		buttons[NO] = unique_ptr<MapleButton>(new MapleButton(src["BtNo"]));

		active = false;
	}

	void UINpcTalk::draw(float inter) const
	{
		using Graphics::DrawArgument;

		Point<int16_t> drawpos = position;
		top.draw(drawpos);
		drawpos.shifty(top.height());
		fill.draw(DrawArgument(drawpos, Point<int16_t>(0, vtile) * fill.height()));
		drawpos.shifty(vtile * fill.height());
		bottom.draw(drawpos);

		UIElement::draw(inter);

		speaker.draw(DrawArgument(position + Point<int16_t>(80, 100), true));
		nametag.draw(position + Point<int16_t>(25, 100));
		name.draw(position + Point<int16_t>(80, 99));
		npctext.draw(position + Point<int16_t>(156, 16 + ((vtile * fill.height() - npctext.getheight()) / 2)));
	}

	void UINpcTalk::buttonpressed(uint16_t buttonid)
	{
		switch (buttonid)
		{
		case END:
			using Net::NpcTalkMorePacket;
			NpcTalkMorePacket(type, 0).dispatch();
			active = false;
			break;
		}
	}

	void UINpcTalk::settext(int32_t npcid, int8_t msgtype, int16_t, int8_t speakerbyte, string text)
	{
		npctext = Npctext(text, 320);

		if (speakerbyte == 0)
		{
			string strid = std::to_string(npcid);
			strid.insert(0, 7 - strid.size(), '0');
			strid.append(".img");
			speaker = nl::nx::npc[strid]["stand"]["0"];
			name = Text(Text::A11M, Text::CENTER, Text::WHITE);
			name.settext(nl::nx::string["Npc.img"][std::to_string(npcid)]["name"]);
		}
		else
		{
			speaker = Texture();
			name.settext("");
		}

		vtile = 8;
		height = vtile * fill.height();

		for (auto& button : buttons)
		{
			button.second->setactive(false);
			button.second->setstate(Button::NORMAL);
		}
		buttons[END]->setposition(Point<int16_t>(20, height + 48));
		buttons[END]->setactive(true);
		switch (msgtype)
		{
		case 0:
			buttons[OK]->setposition(Point<int16_t>(220, height + 48));
			buttons[OK]->setactive(true);
			break;
		}
		type = msgtype;

		position = Point<int16_t>(400 - top.width() / 2, 240 - height / 2);
		dimension = Point<int16_t>(top.width(), height + 120);
		active = true;
	}
}