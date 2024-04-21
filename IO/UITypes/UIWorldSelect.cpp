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
#include "UIWorldSelect.h"

#include "UILoginNotice.h"
#include "UILoginWait.h"
#include "UIRegion.h"

#include "../UI.h"

#include "../Components/MapleButton.h"
#include "../Components/TwoSpriteButton.h"

#include "../../Audio/Audio.h"
#include "../../Util/Randomizer.h"

#include "../../Net/Packets/LoginPackets.h"

#ifdef USE_NX
#include <nlnx/nx.hpp>
#endif

namespace ms
{
	UIWorldSelect::UIWorldSelect() : UIElement(Point<int16_t>(0, 0), Point<int16_t>(800, 600)), worldcount(0), world_selected(false)
	{
		std::string version_text = Configuration::get().get_version();
		version = Text(Text::Font::A12B, Text::Alignment::LEFT, Color::Name::LEMONGRASS, "Ver. " + version_text);
		version_pos = nl::nx::UI["Login.img"]["Common"]["version"]["pos"];

		Point<int16_t> background_pos = Point<int16_t>(512, 384);
		channelsrc_pos = Point<int16_t>(314, 217);

		worldid = Setting<DefaultWorld>::get().load();
		channelid = Setting<DefaultChannel>::get().load();

		nl::node Login = nl::nx::UI["Login.img"];
		nl::node Common = Login["Common"];
		nl::node WorldSelect = Login["WorldSelect"];
		worldsrc = WorldSelect["BtWorld"]["release"];
		channelsrc = WorldSelect["BtChannel"];

		uint8_t regionid = Setting<DefaultRegion>::get().load();
		set_region(regionid);

		nl::node WorldSelectMap = nl::nx::MapLatest["Obj"]["login.img"]["WorldSelect"];
		sprites.emplace_back(WorldSelectMap["default"]["0"], background_pos);

		std::vector<std::string> backgrounds = { "MapleLive" };
		size_t backgrounds_size = backgrounds.size();

		if (backgrounds_size > 0)
		{
			if (backgrounds_size > 1)
			{
				Randomizer randomizer = Randomizer();
				size_t index = randomizer.next_int(backgrounds_size);

				sprites.emplace_back(WorldSelectMap[backgrounds[index]]["0"], background_pos);
			}
			else
			{
				sprites.emplace_back(WorldSelectMap[backgrounds[0]]["0"], background_pos);
			}
		}

		buttons[Buttons::BtRegion] = std::make_unique<MapleButton>(WorldSelect["BtRegion"], Point<int16_t>(0, 1));
		buttons[Buttons::BtExit] = std::make_unique<MapleButton>(Common["BtExit"]);

		for (uint16_t i = 0; i < Buttons::BtGoWorld - Buttons::BtChannel0; i++)
		{
			std::string channel = std::to_string(i);

			buttons[Buttons::BtChannel0 + i] = std::make_unique<TwoSpriteButton>(channelsrc["button:" + channel]["normal"]["0"], channelsrc["button:" + channel]["keyFocused"]["0"], channelsrc_pos);
			buttons[Buttons::BtChannel0 + i]->set_active(false);

			channel_gauge[i] = Gauge(
				Gauge::Type::WORLDSELECT,
				channelsrc["gauge"],
				CHANNEL_LOAD,
				0.0f
			);
		}

		channels_background = channelsrc["layer:bg"];

		buttons[Buttons::BtGoWorld] = std::make_unique<MapleButton>(channelsrc["button:GoWorld"], channelsrc_pos);
		buttons[Buttons::BtGoWorld]->set_active(false);

		chatballoon.change_text("Please select the World you would like to play in.");

		for (size_t i = 1; i <= FLAG_SIZE; i++)
			flag_sprites.emplace_back(Login["WorldNotice"][i]);

		worldNotice = WorldSelect["worldNotice"]["0"];
		rebootNotice = WorldSelect["worldNotice"]["reboot"];
		worldNoticeMessage = Text(Text::Font::A12B, Text::Alignment::LEFT, Color::Name::MINESHAFT);

		if (Configuration::get().get_auto_login())
		{
			uint8_t world = Configuration::get().get_auto_world();
			uint8_t channel = Configuration::get().get_auto_channel();

			Configuration::get().set_worldid(world);
			Configuration::get().set_channelid(channel);

			UI::get().emplace<UILoginWait>();

			auto loginwait = UI::get().get_element<UILoginWait>();

			if (loginwait && loginwait->is_active())
				CharlistRequestPacket(world, channel).dispatch();
		}
	}

	void UIWorldSelect::draw(float alpha) const
	{
		UIElement::draw_sprites(alpha);

		worlds_background.draw(position + worldsrc_pos);

		for (size_t i = 0; i < worlds.size(); i++)
		{
			World world = worlds[i];

			if (world.flag > 0 && world.flag <= FLAG_SIZE)
				flag_sprites[world.flag - 1].draw(position + worldsrc_pos + Point<int16_t>(1, 25 + 33 * i), alpha);
		}

		if (world_selected)
		{
			channels_background.draw(position + channelsrc_pos);
			world_textures[worldid].draw(position + channelsrc_pos);

			uint16_t worldEnum = world_map.find(worldid)->second;

			if (worldEnum == Worlds::REBOOT0)
			{
				rebootNotice.draw(position);
			}
			else if (!worldNoticeMessage.get_text().empty())
			{
				worldNotice.draw(position + Point<int16_t>(-18, 3));
				worldNoticeMessage.draw(position + channelsrc_pos + Point<int16_t>(58, -50));
			}
		}

		UIElement::draw_buttons(alpha);

		if (world_selected)
		{
			World selectedWorld = worlds[worldid];
			uint8_t channel_total = selectedWorld.channel_count;

			for (size_t i = 0; i < Buttons::BtGoWorld - Buttons::BtChannel0 && i < channel_total; i++)
			{
				uint8_t columns = std::min(channel_total, COLUMNS);

				div_t div = std::div(i, columns);
				int32_t current_col = div.rem;
				int32_t current_row = div.quot;

				channel_gauge->draw(position + channelsrc_pos + Point<int16_t>(28 + 71 * current_col, 92 + 30 * current_row));
			}
		}

		version.draw(position + version_pos - Point<int16_t>(0, 5));
		chatballoon.draw(position + Point<int16_t>(747, 105));
	}

	Cursor::State UIWorldSelect::send_cursor(bool clicked, Point<int16_t> cursorpos)
	{
		Rectangle<int16_t> channels_bounds = Rectangle<int16_t>(
			position + channelsrc_pos,
			position + channelsrc_pos + channels_background.get_dimensions()
			);

		Rectangle<int16_t> worlds_bounds = Rectangle<int16_t>(
			position + worldsrc_pos,
			position + worldsrc_pos + worlds_background.get_dimensions()
			);

		if (world_selected && !channels_bounds.contains(cursorpos) && !worlds_bounds.contains(cursorpos))
		{
			if (clicked)
			{
				world_selected = false;
				clear_selected_world();
			}
		}

		Cursor::State ret = clicked ? Cursor::State::CLICKING : Cursor::State::IDLE;

		for (auto& btit : buttons)
		{
			if (btit.second->is_active() && btit.second->bounds(position).contains(cursorpos))
			{
				if (btit.second->get_state() == Button::State::NORMAL)
				{
					Sound(Sound::Name::BUTTONOVER).play();

					btit.second->set_state(Button::State::MOUSEOVER);
					ret = Cursor::State::CANCLICK;
				}
				else if (btit.second->get_state() == Button::State::PRESSED)
				{
					if (clicked)
					{
						Sound(Sound::Name::BUTTONCLICK).play();

						btit.second->set_state(button_pressed(btit.first));

						ret = Cursor::State::IDLE;
					}
					else
					{
						ret = Cursor::State::CANCLICK;
					}
				}
				else if (btit.second->get_state() == Button::State::MOUSEOVER)
				{
					if (clicked)
					{
						Sound(Sound::Name::BUTTONCLICK).play();

						btit.second->set_state(button_pressed(btit.first));

						ret = Cursor::State::IDLE;
					}
					else
					{
						ret = Cursor::State::CANCLICK;
					}
				}
			}
			else if (btit.second->get_state() == Button::State::MOUSEOVER)
			{
				btit.second->set_state(Button::State::NORMAL);
			}
		}

		return ret;
	}

	void UIWorldSelect::send_key(int32_t keycode, bool pressed, bool escape)
	{
		if (pressed)
		{
			if (world_selected)
			{
				World selectedWorld = worlds[worldid];

				uint8_t selected_channel = channelid;
				uint8_t channel_total = selectedWorld.channel_count;

				uint8_t columns = std::min(channel_total, COLUMNS);

				uint8_t rows = std::floor((channel_total - 1) / COLUMNS) + 1;

				div_t div = std::div(selected_channel, columns);
				int32_t current_col = div.rem;

				if (keycode == KeyAction::Id::UP)
				{
					auto next_channel = (selected_channel - COLUMNS < 0 ? (selected_channel - COLUMNS) + rows * COLUMNS : selected_channel - COLUMNS);

					if (next_channel == channelid)
						return;

					if (next_channel > channel_total)
						button_pressed(next_channel - COLUMNS + Buttons::BtChannel0);
					else
						button_pressed(next_channel + Buttons::BtChannel0);
				}
				else if (keycode == KeyAction::Id::DOWN)
				{
					auto next_channel = (selected_channel + COLUMNS >= channel_total ? current_col : selected_channel + COLUMNS);

					if (next_channel == channelid)
						return;

					if (next_channel > channel_total)
						button_pressed(next_channel + COLUMNS + Buttons::BtChannel0);
					else
						button_pressed(next_channel + Buttons::BtChannel0);
				}
				else if (keycode == KeyAction::Id::LEFT || keycode == KeyAction::Id::TAB)
				{
					if (selected_channel != 0)
						selected_channel--;
					else
						selected_channel = channel_total - 1;

					button_pressed(selected_channel + Buttons::BtChannel0);
				}
				else if (keycode == KeyAction::Id::RIGHT)
				{
					if (selected_channel != channel_total - 1)
						selected_channel++;
					else
						selected_channel = 0;

					button_pressed(selected_channel + Buttons::BtChannel0);
				}
				else if (escape)
				{
					world_selected = false;

					clear_selected_world();
				}
				else if (keycode == KeyAction::Id::RETURN)
				{
					button_pressed(Buttons::BtGoWorld);
				}
			}
			else
			{
				auto selected_world = worldid;
				auto world_count = worldcount - 1;

				if (keycode == KeyAction::Id::LEFT || keycode == KeyAction::Id::RIGHT || keycode == KeyAction::Id::UP || keycode == KeyAction::Id::DOWN || keycode == KeyAction::Id::TAB)
				{
					bool world_found = false;
					bool forward = keycode == KeyAction::Id::LEFT || keycode == KeyAction::Id::UP;

					while (!world_found)
					{
						selected_world = get_next_world(selected_world, forward);

						for (auto world : worlds)
						{
							if (world.id == selected_world)
							{
								world_found = true;
								break;
							}
						}
					}

					buttons[Buttons::BtWorld0 + worldid]->set_state(Button::State::NORMAL);

					worldid = static_cast<uint8_t>(selected_world);

					buttons[Buttons::BtWorld0 + worldid]->set_state(Button::State::PRESSED);
				}
				else if (escape)
				{
					auto quitconfirm = UI::get().get_element<UIQuitConfirm>();

					if (quitconfirm && quitconfirm->is_active())
						return UI::get().send_key(keycode, pressed);
					else
						button_pressed(Buttons::BtExit);
				}
				else if (keycode == KeyAction::Id::RETURN)
				{
					auto quitconfirm = UI::get().get_element<UIQuitConfirm>();

					if (quitconfirm && quitconfirm->is_active())
					{
						return UI::get().send_key(keycode, pressed);
					}
					else
					{
						bool found = false;

						for (size_t i = Buttons::BtWorld0; i < Buttons::BtChannel0; i++)
						{
							auto state = buttons[Buttons::BtWorld0 + i]->get_state();

							if (state == Button::State::PRESSED)
							{
								found = true;
								break;
							}
						}

						if (found)
							button_pressed(selected_world + Buttons::BtWorld0);
						else
							buttons[Buttons::BtWorld0 + selected_world]->set_state(Button::State::PRESSED);
					}
				}
			}
		}
	}

	UIElement::Type UIWorldSelect::get_type() const
	{
		return TYPE;
	}

	void UIWorldSelect::draw_world()
	{
		if (worldcount <= 0)
			return; // TODO: Send the user back to the login screen? Otherwise, I think the screen will be blank with no worlds, or throw a UILoginNotice up with failed to communite to server?

		for (auto world : worlds)
		{
			if (world.channel_count < 2)
				return; // I remove the world if there is only one channel because the graphic for the channel selection is defaulted to at least 2

			buttons[Buttons::BtWorld0 + world.id]->set_active(true);

			if (channelid >= world.channel_count)
				channelid = 0;
		}
	}

	void UIWorldSelect::add_world(World world)
	{
		worlds.emplace_back(std::move(world));
		worldcount++;
	}

	void UIWorldSelect::change_world(World selectedWorld)
	{
		buttons[Buttons::BtWorld0 + selectedWorld.id]->set_state(Button::State::PRESSED);

		for (size_t i = 0; i < selectedWorld.channel_count; ++i)
		{
			buttons[Buttons::BtChannel0 + i]->set_active(true);

			if (i == channelid)
				buttons[Buttons::BtChannel0 + i]->set_state(Button::State::PRESSED);

			channel_gauge[i].update(selectedWorld.channel_capacities[i]);
		}

		buttons[Buttons::BtGoWorld]->set_active(true);

		worldNoticeMessage.change_text(selectedWorld.event_message);
	}

	void UIWorldSelect::remove_selected()
	{
		deactivate();

		Sound(Sound::Name::SCROLLUP).play();

		world_selected = false;

		clear_selected_world();
	}

	void UIWorldSelect::set_region(uint8_t regionid)
	{
		world_map[Buttons::BtWorld0] = regionid == 5 ? Worlds::SCANIA : Worlds::LUNA;
		world_map[Buttons::BtWorld1] = Worlds::BERA;
		world_map[Buttons::BtWorld2] = Worlds::AURORA;
		world_map[Buttons::BtWorld3] = Worlds::ELYSIUM;
		world_map[Buttons::BtWorld4] = Worlds::REBOOT0;

		nl::node region = worldsrc["index"][regionid];
		worlds_background = region["layer:bg"];
		worldsrc_pos = region["pos"];

		for (uint16_t i = Buttons::BtWorld0; i < Buttons::BtChannel0; i++)
		{
			std::string world = std::to_string(world_map[i]);
			world_textures.emplace_back(channelsrc["release"]["layer:" + world]);

			nl::node worldbtn = worldsrc["button:" + world];
			buttons[Buttons::BtWorld0 + i] = std::make_unique<TwoSpriteButton>(worldbtn["normal"]["0"], worldbtn["keyFocused"]["0"], worldsrc_pos + Point<int16_t>(region["origin"][i + 1]));
			buttons[Buttons::BtWorld0 + i]->set_active(false);
		}
	}

	uint16_t UIWorldSelect::get_worldbyid(uint16_t worldid)
	{
		return world_map.find(worldid)->second;
	}

	Button::State UIWorldSelect::button_pressed(uint16_t id)
	{
		if (id == Buttons::BtGoWorld)
		{
			enter_world();

			return Button::State::NORMAL;
		}
		else if (id == Buttons::BtExit)
		{
			UI::get().emplace<UIQuitConfirm>();

			return Button::State::NORMAL;
		}
		else if (id == Buttons::BtRegion)
		{
			UI::get().emplace<UIRegion>();

			deactivate();

			return Button::State::NORMAL;
		}
		else if (id >= Buttons::BtWorld0 && id < Buttons::BtChannel0)
		{
			buttons[Buttons::BtWorld0 + worldid]->set_state(Button::State::NORMAL);

			worldid = id - Buttons::BtWorld0;

			ServerStatusRequestPacket(worldid).dispatch();

			world_selected = true;
			clear_selected_world();
			change_world(worlds[worldid]);

			return Button::State::PRESSED;
		}
		else if (id >= Buttons::BtChannel0 && id < Buttons::BtGoWorld)
		{
			uint8_t selectedch = static_cast<uint8_t>(id - Buttons::BtChannel0);

			if (selectedch != channelid)
			{
				buttons[Buttons::BtChannel0 + channelid]->set_state(Button::State::NORMAL);
				channelid = static_cast<uint8_t>(id - Buttons::BtChannel0);
				buttons[Buttons::BtChannel0 + channelid]->set_state(Button::State::PRESSED);
				Sound(Sound::Name::WORLDSELECT).play();
			}
			else
			{
				enter_world();
			}

			return Button::State::PRESSED;
		}
		else
		{
			return Button::State::NORMAL;
		}
	}

	void UIWorldSelect::enter_world()
	{
		Configuration::get().set_worldid(worldid);
		Configuration::get().set_channelid(channelid);

		UI::get().emplace<UILoginWait>();
		auto loginwait = UI::get().get_element<UILoginWait>();

		if (loginwait && loginwait->is_active())
			CharlistRequestPacket(worldid, channelid).dispatch();
	}

	void UIWorldSelect::clear_selected_world()
	{
		channelid = 0;

		for (size_t i = Buttons::BtChannel0; i < Buttons::BtGoWorld; i++)
			buttons[i]->set_state(Button::State::NORMAL);

		buttons[Buttons::BtChannel0]->set_state(Button::State::PRESSED);

		for (size_t i = 0; i < Buttons::BtGoWorld - Buttons::BtChannel0; i++)
		{
			buttons[Buttons::BtChannel0 + i]->set_active(false);

			channel_gauge[i].update(0);
		}

		buttons[Buttons::BtGoWorld]->set_active(false);

		worldNoticeMessage.change_text("");
	}

	uint16_t UIWorldSelect::get_next_world(uint16_t id, bool upward)
	{
		uint16_t next_world;

		if (world_map[Buttons::BtWorld0] == Worlds::SCANIA)
		{
			switch (id)
			{
				case Buttons::BtWorld0:
					next_world = (upward) ? Worlds::REBOOT0 : Worlds::BERA;
					break;
				case Buttons::BtWorld1:
					next_world = (upward) ? Worlds::SCANIA : Worlds::AURORA;
					break;
				case Buttons::BtWorld2:
					next_world = (upward) ? Worlds::BERA : Worlds::ELYSIUM;
					break;
				case Buttons::BtWorld3:
					next_world = (upward) ? Worlds::AURORA : Worlds::REBOOT0;
					break;
				case Buttons::BtWorld4:
					next_world = (upward) ? Worlds::ELYSIUM : Worlds::SCANIA;
					break;
				default:
					break;
			}
		}
		else
		{
			switch (id)
			{
				case Buttons::BtWorld0:
					next_world = (upward) ? Worlds::REBOOT0 : Worlds::REBOOT0;
					break;
				case Buttons::BtWorld4:
					next_world = (upward) ? Worlds::SCANIA : Worlds::SCANIA;
					break;
				default:
					break;
			}
		}

		auto world = world_map.begin();

		while (world != world_map.end())
		{
			if (world->second == next_world)
				return world->first;

			world++;
		}

		return Worlds::SCANIA;
	}
}