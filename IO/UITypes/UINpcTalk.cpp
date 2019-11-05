//////////////////////////////////////////////////////////////////////////////////
//	This file is part of the continued Journey MMORPG client					//
//	Copyright (C) 2015-2019  Daniel Allendorf, Ryan Payton						//
//																				//
//	This program is free software: you can redistribute it and/or modify		//
//	it under the terms of the GNU Affero General Public License as published by	//
//	the Free Software Foundation, either version 3 of the License, or			//
//	(at your option) any later version.											//
//																				//
//	This program is distributed in the hope that it will be useful,				//
//	but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				//
//	GNU Affero General Public License for more details.							//
//																				//
//	You should have received a copy of the GNU Affero General Public License	//
//	along with this program.  If not, see <https://www.gnu.org/licenses/>.		//
//////////////////////////////////////////////////////////////////////////////////
#include "UINpcTalk.h"

#include "../Components/MapleButton.h"

#include "../../Net/Packets/NpcInteractionPackets.h"

#include <nlnx/nx.hpp>

namespace ms
{
	UINpcTalk::UINpcTalk()
	{
		nl::node UIWindow2 = nl::nx::ui["UIWindow2.img"];
		nl::node UtilDlgEx = UIWindow2["UtilDlgEx"];

		top = UtilDlgEx["t"];
		fill = UtilDlgEx["c"];
		bottom = UtilDlgEx["s"];
		nametag = UtilDlgEx["bar"];

		buttons[Buttons::ALLLEVEL] = std::make_unique<MapleButton>(UtilDlgEx["BtAllLevel"]);
		buttons[Buttons::CLOSE] = std::make_unique<MapleButton>(UtilDlgEx["BtClose"]);
		buttons[Buttons::MYLEVEL] = std::make_unique<MapleButton>(UtilDlgEx["BtMyLevel"]);
		buttons[Buttons::NEXT] = std::make_unique<MapleButton>(UtilDlgEx["BtNext"]);

		// TODO: Replace when _inlink is fixed
		//buttons[Buttons::NO] = std::make_unique<MapleButton>(UtilDlgEx["BtNo"]);
		nl::node Quest = UIWindow2["Quest"];

		buttons[Buttons::NO] = std::make_unique<MapleButton>(Quest["BtNo"]);
		buttons[Buttons::OK] = std::make_unique<MapleButton>(UtilDlgEx["BtOK"]);
		buttons[Buttons::PREV] = std::make_unique<MapleButton>(UtilDlgEx["BtPrev"]);
		buttons[Buttons::QAFTER] = std::make_unique<MapleButton>(UtilDlgEx["BtQAfter"]);
		buttons[Buttons::QCNO] = std::make_unique<MapleButton>(UtilDlgEx["BtQCNo"]);
		buttons[Buttons::QCYES] = std::make_unique<MapleButton>(UtilDlgEx["BtQCYes"]);
		buttons[Buttons::QGIVEUP] = std::make_unique<MapleButton>(UtilDlgEx["BtQGiveup"]);
		buttons[Buttons::QNO] = std::make_unique<MapleButton>(UtilDlgEx["BtQNo"]);
		buttons[Buttons::QSTART] = std::make_unique<MapleButton>(UtilDlgEx["BtQStart"]);
		buttons[Buttons::QYES] = std::make_unique<MapleButton>(UtilDlgEx["BtQYes"]);
		buttons[Buttons::YES] = std::make_unique<MapleButton>(UtilDlgEx["BtYes"]);

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

	Button::State UINpcTalk::button_pressed(std::uint16_t buttonid)
	{
		deactivate();

		switch (type)
		{
		case TalkType::SENDNEXT:
		case TalkType::SENDOK:
			// Type = 0
			switch (buttonid)
			{
			case Buttons::CLOSE:
				NpcTalkMorePacket(type, -1).dispatch();
				break;
			case Buttons::NEXT:
			case Buttons::OK:
				NpcTalkMorePacket(type, 1).dispatch();
				break;
			}
			break;
		case TalkType::SENDNEXTPREV:
			// Type = 0
			switch (buttonid)
			{
			case Buttons::CLOSE:
				NpcTalkMorePacket(type, -1).dispatch();
				break;
			case Buttons::NEXT:
				NpcTalkMorePacket(type, 1).dispatch();
				break;
			case Buttons::PREV:
				NpcTalkMorePacket(type, 0).dispatch();
				break;
			}
			break;
		case TalkType::SENDYESNO:
			// Type = 1
			switch (buttonid)
			{
			case Buttons::CLOSE:
				NpcTalkMorePacket(type, -1).dispatch();
				break;
			case Buttons::NO:
				NpcTalkMorePacket(type, 0).dispatch();
				break;
			case Buttons::YES:
				NpcTalkMorePacket(type, 1).dispatch();
				break;
			}
			break;
		case TalkType::SENDACCEPTDECLINE:
			// Type = 1
			switch (buttonid)
			{
			case Buttons::CLOSE:
				NpcTalkMorePacket(type, -1).dispatch();
				break;
			case Buttons::QNO:
				NpcTalkMorePacket(type, 0).dispatch();
				break;
			case Buttons::QYES:
				NpcTalkMorePacket(type, 1).dispatch();
				break;
			}
			break;
		case TalkType::SENDGETTEXT:
			// TODO: What is this?
			break;
		case TalkType::SENDGETNUMBER:
			// Type = 3
			switch (buttonid)
			{
			case Buttons::CLOSE:
				NpcTalkMorePacket(type, 0).dispatch();
				break;
			case Buttons::OK:
				NpcTalkMorePacket(type, 1).dispatch();
				break;
			}
			break;
		case TalkType::SENDSIMPLE:
			// Type = 4
			switch (buttonid)
			{
			case Buttons::CLOSE:
				NpcTalkMorePacket(type, 0).dispatch();
				break;
			default:
				NpcTalkMorePacket(0).dispatch(); // TODO: Selection
				break;
			}
			break;
		default:
			break;
		}

		return Button::State::NORMAL;
	}

	UINpcTalk::TalkType UINpcTalk::get_by_value(std::int8_t value)
	{
		if (value > TalkType::NONE && value < TalkType::LENGTH)
			return static_cast<TalkType>(value);

		return TalkType::NONE;
	}

	void UINpcTalk::change_text(std::int32_t npcid, std::int8_t msgtype, std::int16_t, std::int8_t speakerbyte, const std::string& tx)
	{
		type = get_by_value(msgtype);
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

		std::int16_t y_cord = height + 48;

		buttons[Buttons::CLOSE]->set_position(Point<int16_t>(9, y_cord));
		buttons[Buttons::CLOSE]->set_active(true);

		switch (type)
		{
		case TalkType::SENDOK:
			buttons[Buttons::OK]->set_position(Point<int16_t>(471, y_cord));
			buttons[Buttons::OK]->set_active(true);
			break;
		case TalkType::SENDYESNO:
		{
			Point<int16_t> yes_position = Point<int16_t>(389, y_cord);

			buttons[Buttons::YES]->set_position(yes_position);
			buttons[Buttons::YES]->set_active(true);

			buttons[Buttons::NO]->set_position(yes_position + Point<int16_t>(65, 0));
			buttons[Buttons::NO]->set_active(true);
		}
		break;
		case TalkType::SENDNEXT:
			break;
		case TalkType::SENDNEXTPREV:
			break;
		case TalkType::SENDACCEPTDECLINE:
			break;
		case TalkType::SENDGETTEXT:
			break;
		case TalkType::SENDGETNUMBER:
			break;
		case TalkType::SENDSIMPLE:
			break;
		default:
			break;
		}

		position = Point<int16_t>(400 - top.width() / 2, 240 - height / 2);
		dimension = Point<int16_t>(top.width(), height + 120);
	}
}