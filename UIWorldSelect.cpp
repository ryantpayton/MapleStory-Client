/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015 SYJourney                                               //
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
#pragma once
#include "UIWorldSelect.h"
#include "MapleButton.h"
#include "TwoSpriteButton.h"
#include "Sprite.h"
#include "CharlistRequestPacket.h"
#include "nx.hpp"

namespace IO
{
	UIWorldSelect::UIWorldSelect(UI* u, Login* lg, Session* ses)
	{
		ui = u;
		login = lg;
		session = ses;

		worldid = 0;
		channelid = 0;

		node back = nx::map["Back"]["login.img"]["back"];
		node worlds = nx::ui["Login.img"]["WorldSelect"]["BtWorld"]["release"];
		node channels = nx::ui["Login.img"]["WorldSelect"]["BtChannel"];

		graphics.add(new Sprite(back["11"], vector2d<int>(370, 300)));
		graphics.add(new Sprite(worlds["layer:bg"], vector2d<int>(650, 45)));
		graphics.add(new Sprite(nx::ui["Login.img"]["Common"]["frame"], vector2d<int>(400, 290)));

		buttons.add(BT_ENTERWORLD, new MapleButton(channels["button:GoWorld"], vector2d<int>(200, 170)));

		buttons.add(BT_WORLD0, new MapleButton(worlds["button:15"], vector2d<int>(650, 20)));
		buttons.get(BT_WORLD0)->setstate(BTS_PRESSED);

		graphics.add(new Sprite(channels["layer:bg"], vector2d<int>(200, 170)));
		graphics.add(new Sprite(channels["release"]["layer:15"], vector2d<int>(200, 170)));

		uint8_t chcount = login->getworld(0)->getchcount();
		for (uint8_t i = 0; i < chcount; i++)
		{
			node chnode = channels["button:" + to_string(i)];
			buttons.add(BT_CHANNEL0 + i, new TwoSpriteButton(chnode["normal"]["0"], chnode["keyFocused"]["0"], vector2d<int>(200, 170)));
		}
		buttons.get(BT_CHANNEL0 + channelid)->setstate(BTS_PRESSED);

		position = vector2d<int>(0, 0);
		dimension = vector2d<int>(800, 600);
		active = true;
	}

	void UIWorldSelect::buttonpressed(short id)
	{
		if (id == BT_ENTERWORLD)
		{
			ui->disable();
			login->setworldid(worldid);
			login->setchannelid(channelid);
			session->dispatch(&CharlistRequestPacket(worldid, channelid));
		}
		else if (id >= BT_WORLD0 && id < BT_CHANNEL0)
		{
			 
		}
		else
		{
			buttons.get(BT_CHANNEL0 + channelid)->setstate(BTS_NORMAL);
			channelid = id - BT_CHANNEL0;
		}
	}
}
