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
#pragma once
#include "UISoftKey.h"
#include "MapleButton.h"
#include "SelectCharPackets83.h"
#include "nx.hpp"
#include "node.hpp"

namespace IO
{
	const uint8_t NUM_KEYS = 10;

	UISoftkey::UISoftkey(SoftkeyType t, UI& u, Login& lg, Session& ses) : ui(u), login(lg), session(ses)
	{
		type = t;

		node src = nl::nx::ui["Login.img"]["Common"]["SoftKey"];
		graphics.add(new Texture(src["backgrnd"]));
		graphics.add(new Texture(src["backgrnd2"]));
		graphics.add(new Texture(src["backgrnd3"]));

		buttons.add(BT_SOFTKEY_NEXT, new MapleButton(src["BtNext"], vector2d<int32_t>()));
		buttons.add(BT_SOFTKEY_BACK, new MapleButton(src["BtDel"], vector2d<int32_t>()));
		buttons.add(BT_SOFTKEY_OK, new MapleButton(src["BtOK"], vector2d<int32_t>(72, 235)));
		buttons.add(BT_SOFTKEY_CANCEL, new MapleButton(src["BtCancel"], vector2d<int32_t>(13, 235)));

		node keys = src["BtNum"];
		buttons.add(BT_SOFTKEY_0, new MapleButton(keys["0"], vector2d<int32_t>()));
		buttons.add(BT_SOFTKEY_1, new MapleButton(keys["1"], vector2d<int32_t>()));
		buttons.add(BT_SOFTKEY_2, new MapleButton(keys["2"], vector2d<int32_t>()));
		buttons.add(BT_SOFTKEY_3, new MapleButton(keys["3"], vector2d<int32_t>()));
		buttons.add(BT_SOFTKEY_4, new MapleButton(keys["4"], vector2d<int32_t>()));
		buttons.add(BT_SOFTKEY_5, new MapleButton(keys["5"], vector2d<int32_t>()));
		buttons.add(BT_SOFTKEY_6, new MapleButton(keys["6"], vector2d<int32_t>()));
		buttons.add(BT_SOFTKEY_7, new MapleButton(keys["7"], vector2d<int32_t>()));
		buttons.add(BT_SOFTKEY_8, new MapleButton(keys["8"], vector2d<int32_t>()));
		buttons.add(BT_SOFTKEY_9, new MapleButton(keys["9"], vector2d<int32_t>()));

		buttons.get(BT_SOFTKEY_OK)->setstate(BTS_DISABLED);

		entry = Textlabel(DWF_12ML, TXC_BLACK, "", 0);

		shufflekeys();

		position = vector2d<int32_t>(330, 160);
		dimension = vector2d<int32_t>(140, 280);
		active = true;
	}

	UISoftkey::~UISoftkey() {}

	void UISoftkey::draw() const
	{
		UIElement::draw();

		if (active)
		{
			entry.gettext().draw(position + vector2d<int32_t>(15, 41));
		}
	}

	void UISoftkey::buttonpressed(uint16_t id)
	{
		string entered = entry.gettext().gettext();
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
				entered.append(std::to_string(id));
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
				int32_t cid = login.getcharid();
				switch (type)
				{
				case SFTK_CHARSELECT:
					using::Net::SelectCharPicPacket83;
					session.dispatch(SelectCharPicPacket83(entered, cid));
					break;
				case SFTK_REGISTER:
					using::Net::RegisterPicPacket83;
					session.dispatch(RegisterPicPacket83(cid, entered));
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
			if (entry.gettext().getlength() == 8)
			{
				for (uint8_t i = 0; i < NUM_KEYS; i++)
				{
					buttons.get(i)->setstate(BTS_NORMAL);
				}
			}
			break;
		case 8:
			for (uint8_t i = 0; i < NUM_KEYS; i++)
			{
				buttons.get(i)->setstate(BTS_DISABLED);
			}
			break;
		}

		entry.gettext().settext(entered, 0);
	}

	void UISoftkey::shufflekeys()
	{
		using::std::vector;
		vector<uint8_t> reserve;
		for (uint8_t i = 0; i < NUM_KEYS; i++)
		{
			reserve.push_back(i);
		}
		for (uint8_t i = 0; i < NUM_KEYS; i++)
		{
			size_t rand = random.nextint(reserve.size() - 1);
			vector2d<int32_t> pos = keypos(reserve[rand]);
			buttons.get(BT_SOFTKEY_0 + i)->setposition(pos);
			reserve.erase(reserve.begin() + rand);
		}
	}

	vector2d<int32_t> UISoftkey::keypos(uint8_t num) const
	{
		return vector2d<int32_t>(12 + (num % 3) * 39, 94 + (num / 3) * 35);
	}
}
