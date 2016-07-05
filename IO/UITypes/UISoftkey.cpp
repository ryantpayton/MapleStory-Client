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
#include "UISoftKey.h"

#include "../Components/MapleButton.h"

#include "../../Net/Packets/SelectCharPackets.h"

#include "nlnx/nx.hpp"

namespace jrc
{
	UISoftkey::UISoftkey(Callback c)
		: callback(c) {

		nl::node src = nl::nx::ui["Login.img"]["Common"]["SoftKey"];

		sprites.emplace_back(src["backgrnd"]);
		sprites.emplace_back(src["backgrnd2"]);
		sprites.emplace_back(src["backgrnd3"]);

		buttons[BT_NEXT] = std::make_unique<MapleButton>(src["BtNext"]);
		buttons[BT_BACK] = std::make_unique<MapleButton>(src["BtDel"]);
		buttons[BT_OK] = std::make_unique<MapleButton>(src["BtOK"], Point<int16_t>(72, 235));
		buttons[BT_CANCEL] = std::make_unique<MapleButton>(src["BtCancel"], Point<int16_t>(13, 235));

		nl::node keys = src["BtNum"];

		buttons[BT_0] = std::make_unique<MapleButton>(keys["0"]);
		buttons[BT_1] = std::make_unique<MapleButton>(keys["1"]);
		buttons[BT_2] = std::make_unique<MapleButton>(keys["2"]);
		buttons[BT_3] = std::make_unique<MapleButton>(keys["3"]);
		buttons[BT_4] = std::make_unique<MapleButton>(keys["4"]);
		buttons[BT_5] = std::make_unique<MapleButton>(keys["5"]);
		buttons[BT_6] = std::make_unique<MapleButton>(keys["6"]);
		buttons[BT_7] = std::make_unique<MapleButton>(keys["7"]);
		buttons[BT_8] = std::make_unique<MapleButton>(keys["8"]);
		buttons[BT_9] = std::make_unique<MapleButton>(keys["9"]);

		buttons[BT_OK]->set_state(Button::DISABLED);

		entry = { Text::A11M, Text::LEFT, Text::BLACK };

		shufflekeys();

		position = { 330, 160 };
		dimension = { 140, 280 };
		active = true;
	}

	void UISoftkey::draw(float alpha) const
	{
		UIElement::draw(alpha);

		entry.draw(position + Point<int16_t>(15, 43));
	}

	Button::State UISoftkey::button_pressed(uint16_t id)
	{
		std::string entered = entry.get_text();

		switch (id)
		{
		case BT_0:
		case BT_1:
		case BT_2:
		case BT_3:
		case BT_4:
		case BT_5:
		case BT_6:
		case BT_7:
		case BT_8:
		case BT_9:
			if (entered.size() <= MAX_SIZE)
			{
				entered.append(std::to_string(id));
				shufflekeys();
			}
			buttons[id]->set_state(Button::NORMAL);
			break;
		case BT_BACK:
			if (entered.size() > 0)
			{
				entered.pop_back();
			}
			buttons[id]->set_state(Button::NORMAL);
			break;
		case BT_CANCEL:
			active = false;
			break;
		case BT_OK:
			if (entered.size() >= MIN_SIZE && callback)
			{
				callback(entered);
				active = false;
			}
			break;
		}

		switch (entered.size())
		{
		case MIN_SIZE - 1:
			buttons[BT_OK]->set_state(Button::DISABLED);
			break;
		case MIN_SIZE:
			buttons[BT_OK]->set_state(Button::NORMAL);
			break;
		case MAX_SIZE - 1:
			for (uint8_t i = 0; i < NUM_KEYS; i++)
			{
				buttons[i]->set_state(Button::NORMAL);
			}
			break;
		case MAX_SIZE:
			for (uint8_t i = 0; i < NUM_KEYS; i++)
			{
				buttons[i]->set_state(Button::DISABLED);
			}
			break;
		}

		entry.change_text(entered);

		return Button::IDENTITY;
	}

	void UISoftkey::shufflekeys()
	{
		std::vector<uint8_t> reserve;
		for (uint8_t i = 0; i < NUM_KEYS; i++)
		{
			reserve.push_back(i);
		}
		for (uint8_t i = 0; i < NUM_KEYS; i++)
		{
			size_t rand = random.next_int(reserve.size());
			Point<int16_t> pos = keypos(reserve[rand]);
			buttons[BT_0 + i]->set_position(pos);
			reserve.erase(reserve.begin() + rand);
		}
	}

	Point<int16_t> UISoftkey::keypos(uint8_t num) const
	{
		return{
			12 + (num % 3) * 39,
			94 + (num / 3) * 35
		};
	}
}
