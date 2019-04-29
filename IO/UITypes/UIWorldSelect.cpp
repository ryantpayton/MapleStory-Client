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
#include "UIWorldSelect.h"
#include "UILoginNotice.h"

#include "../../Configuration.h"
#include "../../Graphics/Sprite.h"
#include "../../IO/UI.h"
#include "../../IO/Components/MapleButton.h"
#include "../../IO/Components/TwoSpriteButton.h"
#include "../../Net/Packets/LoginPackets.h"

#include "nlnx/nx.hpp"

namespace jrc
{
	UIWorldSelect::UIWorldSelect() : UIElement({ 0, 0 }, { 800, 600 })
	{
		version = { Text::Font::A11M, Text::Alignment::LEFT, Text::Color::LEMONGRASS, "Ver. 203.3" };

		channelsrc_pos = Point<int16_t>(203, 154);

		worldid = Setting<DefaultWorld>::get().load();
		channelid = Setting<DefaultChannel>::get().load();

		nl::node obj = nl::nx::map["Obj"]["login.img"];
		nl::node login = nl::nx::ui["Login.img"];
		worldselect = login["WorldSelect"];
		worldsrc = worldselect["BtWorld"]["release"];
		channelsrc = worldselect["BtChannel"];
		nl::node common = login["Common"];

		int background = rand() % 1;

		if (background == 0)
			sprites.emplace_back(obj["WorldSelect"]["RetroWorld"]["0"], Point<int16_t>(370, 300)); // From v203.3
		else if (background == 1)
			sprites.emplace_back(obj["WorldSelect"]["SavageT"]["0"], Point<int16_t>(370, 300)); // From v203.3
		else
			sprites.emplace_back(obj["WorldSelect"]["default"]["0"], Point<int16_t>(370, 300)); // From v203.3

		sprites.emplace_back(common["frame"], Point<int16_t>(399, 289));
		sprites.emplace_back(common["step"]["1"], Point<int16_t>(40, -10));

		buttons[BT_VIEWRECOMMENDED] = std::make_unique<MapleButton>(worldselect["BtViewChoice"], Point<int16_t>(0, 43));
		buttons[BT_CHANGEREGION] = std::make_unique<MapleButton>(worldselect["BtRegion"], Point<int16_t>(3, 117));
		buttons[BT_QUITGAME] = std::make_unique<MapleButton>(common["BtExit"], Point<int16_t>(0, 505));

		sprites.emplace_back(worldsrc["layer:bg"], Point<int16_t>(646, 20));
		sprites.emplace_back(channelsrc["layer:bg"], channelsrc_pos);

		buttons[BT_ENTERWORLD] = std::make_unique<MapleButton>(channelsrc["button:GoWorld"], channelsrc_pos);
	}

	void UIWorldSelect::draw(float alpha) const
	{
		UIElement::draw(alpha);

		version.draw(position + Point<int16_t>(707, -9));
	}

	void UIWorldSelect::add_world(World world)
	{
		worlds.emplace_back(std::move(world));
		worldcount++;
	}

	void UIWorldSelect::draw_world()
	{
		if (worldcount <= 0)
			return; // TODO: Send the user back to the login screen? Otherwise, I think the screen will be blank with no worlds, or throw a UILoginNotice up with failed to communite to server?

		for (int w = 0; w < worlds.size(); ++w)
		{
			const World world = worlds[w];
			const std::string swid = std::to_string(world.wid);

			if (world.channelcount < 2)
				return; // I remove the world if there is only one channel because the graphic for the channel selection is defaulted to at least 2

			buttons[BT_WORLD0 + w] = std::make_unique<TwoSpriteButton>(worldsrc["button:" + swid]["normal"]["0"], worldsrc["button:" + swid]["keyFocused"]["0"], Point<int16_t>(646, 20));

			if (channelid >= world.channelcount)
				channelid = 0;

			if (worldid == w)
				change_world(world, true);
		}
	}

	void UIWorldSelect::change_world(World selectedWorld, bool init)
	{
		const std::string swid = std::to_string(selectedWorld.wid);

		sprites.emplace_back(channelsrc["release"]["layer:" + swid], channelsrc_pos);

		if (init)
			buttons[BT_WORLD0 + selectedWorld.wid]->set_state(Button::PRESSED);

		for (uint8_t c = 0; c < selectedWorld.channelcount; ++c)
		{
			nl::node chnode = channelsrc["button:" + std::to_string(c)];

			buttons[BT_CHANNEL0 + c] = std::make_unique<TwoSpriteButton>(chnode["normal"]["0"], chnode["keyFocused"]["0"], channelsrc_pos);

			if (c == channelid)
				buttons[BT_CHANNEL0 + c]->set_state(Button::PRESSED);
		}
	}

	uint8_t UIWorldSelect::get_world_id() const
	{
		return worldid;
	}

	uint8_t UIWorldSelect::get_channel_id() const
	{
		return channelid;
	}

	Button::State UIWorldSelect::button_pressed(uint16_t id)
	{
		if (id == BT_ENTERWORLD)
		{
			UI::get().disable();

			CharlistRequestPacket(worldid, channelid).dispatch();

			return Button::NORMAL;
		}
		else if (id == BT_QUITGAME)
		{
			UI::get().emplace<UIQuitConfirm>();
			UI::get().enable();

			return Button::NORMAL;
		}
		/*else if (id == BT_VIEWRECOMMENDED)
		{
			sprites.emplace_back(worldselect["alert"]["backgrd"], Point<int16_t>(100, 100));
			sprites.emplace_back(worldselect["alert"]["BtArrowL"]["normal"]["0"], Point<int16_t>(90, 150));
			sprites.emplace_back(worldselect["alert"]["BtArrowR"]["normal"]["0"], Point<int16_t>(110, 150));

			buttons[BT_VIEWRECOMMENDED_SELECT] = std::make_unique<MapleButton>(worldselect["alert"]["BtChoice"]["normal"]["0"], Point<int16_t>(90, 175));
			buttons[BT_VIEWRECOMMENDED_CANCEL] = std::make_unique<MapleButton>(worldselect["alert"]["BtClose"]["normal"]["0"], Point<int16_t>(110, 175));
		}
		else if (id == BT_VIEWRECOMMENDED_SELECT)
		{
			return Button::PRESSED;
		}
		else if (id == BT_VIEWRECOMMENDED_CANCEL)
		{
			sprites.pop_back();

			buttons.erase(BT_SELECT);
			buttons.erase(BT_CANCEL);

			return Button::PRESSED;
		}*/
		else if (id >= BT_WORLD0 && id <= BT_WORLD43)
		{
			buttons[BT_WORLD0 + worldid]->set_state(Button::NORMAL);

			const World world = worlds[worldid];

			for (int i = 0; i < world.channelcount; ++i)
				buttons.erase(BT_CHANNEL0 + i);

			sprites.pop_back();

			worldid = static_cast<uint8_t>(id - BT_WORLD0);

			change_world(worlds[worldid], false);

			return Button::PRESSED;
		}
		else if (id >= BT_CHANNEL0 && id <= BT_CHANNEL19)
		{
			buttons[BT_CHANNEL0 + channelid]->set_state(Button::NORMAL);
			channelid = static_cast<uint8_t>(id - BT_CHANNEL0);

			return Button::PRESSED;
		}
		else // Unhandled
		{
			return Button::NORMAL;
		}
	}
}