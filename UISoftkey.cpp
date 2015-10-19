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
#include "UISoftKey.h"
#include "MapleButton.h"
#include "SelectCharPackets.h"
#include "nx.hpp"
#include "node.hpp"

namespace IO
{
	UISoftkey::UISoftkey(SoftkeyType t, UI& u, Login& lg, Session& ses) : ui(u), login(lg), session(ses)
	{
		type = t;

		node src = nx::ui["Login.img"]["Common"]["SoftKey"];
		graphics.add(new Texture(src["backgrnd"]));
		graphics.add(new Texture(src["backgrnd2"]));
		graphics.add(new Texture(src["backgrnd3"]));

		buttons.add(BT_SOFTKEY_NEXT, new MapleButton(src["BtNext"], vector2d<int>()));
		buttons.add(BT_SOFTKEY_BACK, new MapleButton(src["BtDel"], vector2d<int>()));
		buttons.add(BT_SOFTKEY_OK, new MapleButton(src["BtOK"], vector2d<int>(72, 235)));
		buttons.add(BT_SOFTKEY_CANCEL, new MapleButton(src["BtCancel"], vector2d<int>(13, 235)));

		node keys = src["BtNum"];
		buttons.add(BT_SOFTKEY_0, new MapleButton(keys["0"], vector2d<int>()));
		buttons.add(BT_SOFTKEY_1, new MapleButton(keys["1"], vector2d<int>()));
		buttons.add(BT_SOFTKEY_2, new MapleButton(keys["2"], vector2d<int>()));
		buttons.add(BT_SOFTKEY_3, new MapleButton(keys["3"], vector2d<int>()));
		buttons.add(BT_SOFTKEY_4, new MapleButton(keys["4"], vector2d<int>()));
		buttons.add(BT_SOFTKEY_5, new MapleButton(keys["5"], vector2d<int>()));
		buttons.add(BT_SOFTKEY_6, new MapleButton(keys["6"], vector2d<int>()));
		buttons.add(BT_SOFTKEY_7, new MapleButton(keys["7"], vector2d<int>()));
		buttons.add(BT_SOFTKEY_8, new MapleButton(keys["8"], vector2d<int>()));
		buttons.add(BT_SOFTKEY_9, new MapleButton(keys["9"], vector2d<int>()));

		buttons.get(BT_SOFTKEY_OK)->setstate(BTS_DISABLED);

		entry = new Textlabel(DWF_12ML, TXC_BLACK, "", 0);

		shufflekeys();

		position = vector2d<int>(330, 160);
		dimension = vector2d<int>(140, 280);
		active = true;
	}

	UISoftkey::~UISoftkey()
	{
		delete entry;
	}

	void UISoftkey::draw()
	{
		UIElement::draw();

		if (active)
		{
			entry->draw(position + vector2d<int>(15, 41));
		}
	}

	void UISoftkey::buttonpressed(short id)
	{
		string entered = entry->gettext();
		switch (id)
		{
		case BT_SOFTKEY_0:
		case BT_SOFTKEY_1:
		case BT_SOFTKEY_2:
		case BT_SOFTKEY_3:
		case BT_SOFTKEY_4:
		case BT_SOFTKEY_5:
		case BT_SOFTKEY_6:
		case BT_SOFTKEY_7:
		case BT_SOFTKEY_8:
		case BT_SOFTKEY_9:
			if (entered.size() < 8)
			{
				entered.append(to_string(id));
				shufflekeys();
			}
			buttons.get(id)->setstate(BTS_NORMAL);
			break;
		case BT_SOFTKEY_BACK:
			if (entered.size() > 0)
			{
				entered.pop_back();
			}
			buttons.get(id)->setstate(BTS_NORMAL);
			break;
		case BT_SOFTKEY_CANCEL:
			active = false;
			break;
		case BT_SOFTKEY_OK:
			if (entered.size() > 5)
			{
				ui.disable();
				int cid = login.getcharid();
				switch (type)
				{
				case SFTK_CHARSELECT:
					session.dispatch(SelectCharPicPacket(entered, cid));
					break;
				case SFTK_REGISTER:
					session.dispatch(RegisterPicPacket(cid, entered));
					break;
				}
				active = false;
			}
			break;
		}

		switch (entered.size())
		{
		case 5:
			buttons.get(BT_SOFTKEY_OK)->setstate(BTS_DISABLED);
			break;
		case 6:
			buttons.get(BT_SOFTKEY_OK)->setstate(BTS_NORMAL);
			break;
		case 7:
			if (entry->getlength() == 8)
			{
				for (char i = 0; i < 10; i++)
				{
					buttons.get(i)->setstate(BTS_NORMAL);
				}
			}
			break;
		case 8:
			for (char i = 0; i < 10; i++)
			{
				buttons.get(i)->setstate(BTS_DISABLED);
			}
			break;
		}

		entry->settext(entered);
	}

	void UISoftkey::shufflekeys()
	{
		vector<char> reserve;
		for (char i = 0; i < 10; i++)
		{
			reserve.push_back(i);
		}
		for (char i = 0; i < 10; i++)
		{
			size_t rand = random.nextint(reserve.size() - 1);
			vector2d<int> pos = keypos(reserve[rand]);
			buttons.get(BT_SOFTKEY_0 + i)->setposition(pos);
			reserve.erase(reserve.begin() + rand);
		}
	}

	vector2d<int> UISoftkey::keypos(char num)
	{
		return vector2d<int>(12 + (num % 3) * 39, 94 + (num / 3) * 35);
	}
}
