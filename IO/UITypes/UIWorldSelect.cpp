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
#include "UIWorldSelect.h"

#include "..\..\Configuration.h"
#include "..\..\Graphics\Sprite.h"
#include "..\..\IO\UI.h"
#include "..\..\IO\Components\MapleButton.h"
#include "..\..\IO\Components\TwoSpriteButton.h"
#include "..\..\Net\Session.h"
#include "..\..\Net\Packets\LoginPackets.h"

#include "nlnx\nx.hpp"

namespace jrc
{
	UIWorldSelect::UIWorldSelect()
	{
		worldid = Setting<DefaultWorld>::get().load();
		channelid = Setting<DefaultChannel>::get().load();

		nl::node back = nl::nx::map["Back"]["login.img"]["back"];
		nl::node worlds = nl::nx::ui["Login.img"]["WorldSelect"]["BtWorld"]["release"];
		nl::node channels = nl::nx::ui["Login.img"]["WorldSelect"]["BtChannel"];
		nl::node frame = nl::nx::ui["Login.img"]["Common"]["frame"];

		sprites.emplace_back(back["11"], Point<int16_t>(370, 300));
		sprites.emplace_back(worlds["layer:bg"], Point<int16_t>(650, 45));
		sprites.emplace_back(frame, Point<int16_t>(400, 290));

		buttons[BT_ENTERWORLD] = std::make_unique<MapleButton>(
			channels["button:GoWorld"], 
			Point<int16_t>(200, 170)
			);

		worldcount = Session::get().getlogin().getnumworlds();
		if (worldcount > 0)
		{
			buttons[BT_WORLD0] = std::make_unique<MapleButton>(worlds["button:15"], Point<int16_t>(650, 20));
			buttons[BT_WORLD0]->setstate(Button::PRESSED);

			sprites.emplace_back(channels["layer:bg"], Point<int16_t>(200, 170));
			sprites.emplace_back(channels["release"]["layer:15"], Point<int16_t>(200, 170));

			uint8_t chcount = Session::get().getlogin().getworld(worldid).channelcount;
			if (channelid >= chcount)
				channelid = 0;

			for (uint8_t i = 0; i < chcount; i++)
			{
				nl::node chnode = channels["button:" + std::to_string(i)];
				buttons[BT_CHANNEL0 + i] = std::make_unique<TwoSpriteButton>(
					chnode["normal"]["0"], chnode["keyFocused"]["0"],
					Point<int16_t>(200, 170)
					);
			}
			buttons[BT_CHANNEL0 + channelid]->setstate(Button::PRESSED);
		}

		position = Point<int16_t>(0, 0);
		dimension = Point<int16_t>(800, 600);
		active = true;
	}

	void UIWorldSelect::buttonpressed(uint16_t id)
	{
		if (id == BT_ENTERWORLD)
		{
			UI::get().disable();

			Session::get().getlogin()
				.setworldid(worldid);
			Session::get().getlogin()
				.setchannelid(channelid);

			CharlistRequestPacket(worldid, channelid)
				.dispatch();
		}
		else if (id >= BT_WORLD0 && id < BT_CHANNEL0)
		{
			 
		}
		else
		{
			buttons[BT_CHANNEL0 + channelid]->setstate(Button::NORMAL);
			channelid = static_cast<uint8_t>(id - BT_CHANNEL0);
		}
	}
}
