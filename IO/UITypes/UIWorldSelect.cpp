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
	UIWorldSelect::UIWorldSelect() : UIElement(Point<int16_t>(0, 0), Point<int16_t>(800, 600)), worldcount(0), recommended_worldcount(0), recommended_worldid(0), world_selected(false), use_recommended(true), show_recommended(false), draw_chatballoon(true)
	{
		std::string version_text = Configuration::get().get_version();
		version = Text(Text::Font::A11B, Text::Alignment::LEFT, Color::Name::LEMONGRASS, "Ver. " + version_text);

		recommended_message = Text(Text::Font::A11M, Text::Alignment::CENTER, Color::Name::JAMBALAYA, "", 100, true, 5);

		Point<int16_t> background_pos = Point<int16_t>(400, 301);
		channelsrc_pos = Point<int16_t>(203, 164);

		worldid = Setting<DefaultWorld>::get().load();
		channelid = Setting<DefaultChannel>::get().load();

		nl::node Login = nl::nx::ui["Login.img"];
		nl::node Common = Login["Common"];
		WorldSelect = Login["WorldSelect"];
		worldsrc = WorldSelect["BtWorld"]["release"];
		channelsrc = WorldSelect["BtChannel"];

		uint8_t regionid = Setting<DefaultRegion>::get().load();
		set_region(regionid);

		nl::node login = nl::nx::mapLatest["Obj"]["login.img"];
		sprites.emplace_back(login["WorldSelect"]["default"]["0"], background_pos);

		std::vector<std::string> backgrounds = { "16thNewtro" };
		size_t backgrounds_size = backgrounds.size();

		if (backgrounds_size > 0)
		{
			if (backgrounds_size > 1)
			{
				Randomizer randomizer = Randomizer();
				size_t index = randomizer.next_int(backgrounds_size);

				sprites.emplace_back(WorldSelect[backgrounds[index]]["0"], background_pos);
			}
			else
			{
				sprites.emplace_back(WorldSelect[backgrounds[0]]["0"], background_pos);
			}
		}

		sprites.emplace_back(login["Common"]["frame"]["2"]["0"], Point<int16_t>(400, 300));
		sprites.emplace_back(Common["frame"], Point<int16_t>(400, 300));
		sprites.emplace_back(Common["step"]["1"], Point<int16_t>(40, 0));

		nl::node alert = WorldSelect["alert"];
		buttons[Buttons::BT_VIEWALL] = std::make_unique<MapleButton>(WorldSelect["BtViewAll"], Point<int16_t>(0, 53));
		buttons[Buttons::BT_VIEWRECOMMENDED] = std::make_unique<MapleButton>(WorldSelect["BtViewChoice"], Point<int16_t>(0, 53));
		buttons[Buttons::BT_VIEWRECOMMENDED_SELECT] = std::make_unique<MapleButton>(alert["BtChoice"], Point<int16_t>(349, 327));
		buttons[Buttons::BT_VIEWRECOMMENDED_CANCEL] = std::make_unique<MapleButton>(alert["BtClose"], Point<int16_t>(407, 327));
		buttons[Buttons::BT_VIEWRECOMMENDED_PREV] = std::make_unique<MapleButton>(alert["BtArrowL"], Point<int16_t>(338, 244));
		buttons[Buttons::BT_VIEWRECOMMENDED_NEXT] = std::make_unique<MapleButton>(alert["BtArrowR"], Point<int16_t>(439, 244));

		buttons[Buttons::BT_VIEWALL]->set_active(false);
		buttons[Buttons::BT_VIEWRECOMMENDED]->set_active(use_recommended ? true : false);
		buttons[Buttons::BT_VIEWRECOMMENDED_SELECT]->set_active(false);
		buttons[Buttons::BT_VIEWRECOMMENDED_CANCEL]->set_active(false);
		buttons[Buttons::BT_VIEWRECOMMENDED_PREV]->set_active(false);
		buttons[Buttons::BT_VIEWRECOMMENDED_NEXT]->set_active(false);

		buttons[Buttons::BT_VIEWRECOMMENDED]->set_state(Button::State::DISABLED);

		recommended_texture = alert["backgrd"];

		buttons[Buttons::BT_CHANGEREGION] = std::make_unique<MapleButton>(WorldSelect["BtRegion"], Point<int16_t>(3, 127));
		buttons[Buttons::BT_QUITGAME] = std::make_unique<MapleButton>(Common["BtExit"], Point<int16_t>(0, 515));

		for (uint16_t i = 0; i < Buttons::BT_ENTERWORLD - Buttons::BT_CHANNEL0; i++)
		{
			std::string channel = std::to_string(i);

			buttons[Buttons::BT_CHANNEL0 + i] = std::make_unique<TwoSpriteButton>(channelsrc["button:" + channel]["normal"]["0"], channelsrc["button:" + channel]["keyFocused"]["0"], channelsrc_pos);
			buttons[Buttons::BT_CHANNEL0 + i]->set_active(false);

			channel_gauge[i] = Gauge(
				Gauge::Type::WORLDSELECT,
				channelsrc["gauge"],
				CHANNEL_LOAD,
				0.0f
			);
		}

		channels_background = channelsrc["layer:bg"];

		buttons[Buttons::BT_ENTERWORLD] = std::make_unique<MapleButton>(channelsrc["button:GoWorld"], channelsrc_pos);
		buttons[Buttons::BT_ENTERWORLD]->set_active(false);

		chatballoon.change_text("Please select the World you would like to play in.");

		for (size_t i = 1; i <= FLAG_SIZE; i++)
			flag_sprites.emplace_back(Login["WorldNotice"][i]);

		worldNotice = WorldSelect["worldNotice"]["0"];
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

		if (show_recommended)
		{
			Point<int16_t> recommended_world_adj = Point<int16_t>(0, 0);

			if (recommended_worldid == Buttons::BT_WORLD4)
			{
				int16_t y_adj = 48; // This based on the distance between the bottom of the red banner to the top of the border separating the world logo and the text/buttons in recommended_texture
				Texture recommended_world_texture = recommended_world_textures[recommended_worldid];

				recommended_world_adj = Point<int16_t>(
					recommended_texture.width() / 2 - recommended_world_texture.width() / 2,
					recommended_texture.height() / 2 - recommended_world_texture.height() / 2 - y_adj - std::abs(recommended_world_texture.get_origin().y())
					);
			}

			recommended_texture.draw(position + Point<int16_t>(302, 152));
			recommended_world_textures[recommended_worldid].draw(position + Point<int16_t>(336, 187) - recommended_world_adj);
			recommended_message.draw(position + Point<int16_t>(401, 259));
		}

		if (world_selected)
		{
			channels_background.draw(position + channelsrc_pos);
			world_textures[worldid].draw(position + channelsrc_pos);

			if (!worldNoticeMessage.get_text().empty())
			{
				worldNotice.draw(position - Point<int16_t>(0, 30));
				worldNoticeMessage.draw(position + channelsrc_pos + Point<int16_t>(57, -50));
			}
		}

		UIElement::draw_buttons(alpha);

		if (world_selected)
		{
			World selectedWorld = worlds[worldid];
			uint8_t channel_total = selectedWorld.channel_count;

			for (size_t i = 0; i < Buttons::BT_ENTERWORLD - Buttons::BT_CHANNEL0 && i < channel_total; i++)
			{
				uint8_t columns = std::min(channel_total, COLUMNS);

				div_t div = std::div(i, columns);
				int32_t current_col = div.rem;
				int32_t current_row = div.quot;

				channel_gauge->draw(position + channelsrc_pos + Point<int16_t>(28 + 71 * current_col, 92 + 30 * current_row));
			}
		}

		version.draw(position + Point<int16_t>(707, 4));

		if (draw_chatballoon)
			chatballoon.draw(position + Point<int16_t>(501, 105));
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
						button_pressed(next_channel - COLUMNS + Buttons::BT_CHANNEL0);
					else
						button_pressed(next_channel + Buttons::BT_CHANNEL0);
				}
				else if (keycode == KeyAction::Id::DOWN)
				{
					auto next_channel = (selected_channel + COLUMNS >= channel_total ? current_col : selected_channel + COLUMNS);

					if (next_channel == channelid)
						return;

					if (next_channel > channel_total)
						button_pressed(next_channel + COLUMNS + Buttons::BT_CHANNEL0);
					else
						button_pressed(next_channel + Buttons::BT_CHANNEL0);
				}
				else if (keycode == KeyAction::Id::LEFT || keycode == KeyAction::Id::TAB)
				{
					if (selected_channel != 0)
						selected_channel--;
					else
						selected_channel = channel_total - 1;

					button_pressed(selected_channel + Buttons::BT_CHANNEL0);
				}
				else if (keycode == KeyAction::Id::RIGHT)
				{
					if (selected_channel != channel_total - 1)
						selected_channel++;
					else
						selected_channel = 0;

					button_pressed(selected_channel + Buttons::BT_CHANNEL0);
				}
				else if (escape)
				{
					world_selected = false;

					clear_selected_world();
				}
				else if (keycode == KeyAction::Id::RETURN)
				{
					button_pressed(Buttons::BT_ENTERWORLD);
				}
			}
			else if (show_recommended)
			{
				if (escape || keycode == KeyAction::Id::RETURN)
					toggle_recommended(false);
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

					buttons[Buttons::BT_WORLD0 + worldid]->set_state(Button::State::NORMAL);

					worldid = static_cast<uint8_t>(selected_world);

					buttons[Buttons::BT_WORLD0 + worldid]->set_state(Button::State::PRESSED);
				}
				else if (escape)
				{
					auto quitconfirm = UI::get().get_element<UIQuitConfirm>();

					if (quitconfirm && quitconfirm->is_active())
						return UI::get().send_key(keycode, pressed);
					else
						button_pressed(Buttons::BT_QUITGAME);
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

						for (size_t i = Buttons::BT_WORLD0; i < Buttons::BT_CHANNEL0; i++)
						{
							auto state = buttons[Buttons::BT_WORLD0 + i]->get_state();

							if (state == Button::State::PRESSED)
							{
								found = true;
								break;
							}
						}

						if (found)
							button_pressed(selected_world + Buttons::BT_WORLD0);
						else
							buttons[Buttons::BT_WORLD0 + selected_world]->set_state(Button::State::PRESSED);
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

			buttons[Buttons::BT_WORLD0 + world.id]->set_active(true);

			if (channelid >= world.channel_count)
				channelid = 0;
		}
	}

	void UIWorldSelect::add_world(World world)
	{
		worlds.emplace_back(std::move(world));
		worldcount++;
	}

	void UIWorldSelect::add_recommended_world(RecommendedWorld world)
	{
		if (use_recommended)
		{
			recommended_worlds.emplace_back(std::move(world));
			recommended_worldcount++;

			buttons[Buttons::BT_VIEWRECOMMENDED]->set_state(Button::State::NORMAL);
		}
	}

	void UIWorldSelect::change_world(World selectedWorld)
	{
		buttons[Buttons::BT_WORLD0 + selectedWorld.id]->set_state(Button::State::PRESSED);

		for (size_t i = 0; i < selectedWorld.channel_count; ++i)
		{
			buttons[Buttons::BT_CHANNEL0 + i]->set_active(true);

			if (i == channelid)
				buttons[Buttons::BT_CHANNEL0 + i]->set_state(Button::State::PRESSED);

			channel_gauge[i].update(selectedWorld.channel_capacities[i]);
		}

		buttons[Buttons::BT_ENTERWORLD]->set_active(true);

		worldNoticeMessage.change_text(selectedWorld.event_message);
	}

	void UIWorldSelect::remove_selected()
	{
		deactivate();

		Sound(Sound::Name::SCROLLUP).play();

		world_selected = false;

		clear_selected_world();

		draw_chatballoon = false;
	}

	void UIWorldSelect::set_region(uint8_t regionid)
	{
		world_map[Buttons::BT_WORLD0] = regionid == 5 ? Worlds::SCANIA : Worlds::LUNA;
		world_map[Buttons::BT_WORLD1] = Worlds::BERA;
		world_map[Buttons::BT_WORLD2] = Worlds::AURORA;
		world_map[Buttons::BT_WORLD3] = Worlds::ELYSIUM;
		world_map[Buttons::BT_WORLD4] = Worlds::REBOOT0;

		nl::node region = worldsrc["index"][regionid];
		worlds_background = region["layer:bg"];
		worldsrc_pos = region["pos"];

		for (uint16_t i = Buttons::BT_WORLD0; i < Buttons::BT_CHANNEL0; i++)
		{
			std::string world = std::to_string(world_map[i]);
			world_textures.emplace_back(channelsrc["release"]["layer:" + world]);
			recommended_world_textures.emplace_back(WorldSelect["world"][world]);

			nl::node worldbtn = worldsrc["button:" + world];
			buttons[Buttons::BT_WORLD0 + i] = std::make_unique<TwoSpriteButton>(worldbtn["normal"]["0"], worldbtn["keyFocused"]["0"], worldsrc_pos + Point<int16_t>(region["origin"][i + 1]));
			buttons[Buttons::BT_WORLD0 + i]->set_active(false);
		}
	}

	uint16_t UIWorldSelect::get_worldbyid(uint16_t worldid)
	{
		return world_map.find(worldid)->second;
	}

	Button::State UIWorldSelect::button_pressed(uint16_t id)
	{
		if (id == Buttons::BT_ENTERWORLD)
		{
			enter_world();

			return Button::State::NORMAL;
		}
		else if (id == Buttons::BT_QUITGAME)
		{
			UI::get().emplace<UIQuitConfirm>();

			return Button::State::NORMAL;
		}
		else if (id == Buttons::BT_VIEWRECOMMENDED)
		{
			world_selected = false;
			clear_selected_world();
			toggle_recommended(true);

			return Button::State::NORMAL;
		}
		else if (id == Buttons::BT_VIEWALL)
		{
			toggle_recommended(false);

			return Button::State::NORMAL;
		}
		else if (id == Buttons::BT_VIEWRECOMMENDED_SELECT)
		{
			buttons[Buttons::BT_WORLD0 + worldid]->set_state(Button::State::NORMAL);

			worldid = recommended_worldid;

			buttons[Buttons::BT_WORLD0 + worldid]->set_state(Button::State::PRESSED);

			toggle_recommended(false);

			return Button::State::NORMAL;
		}
		else if (id == Buttons::BT_VIEWRECOMMENDED_CANCEL)
		{
			toggle_recommended(false);

			return Button::State::NORMAL;
		}
		else if (id == Buttons::BT_VIEWRECOMMENDED_PREV)
		{
			if (recommended_worldid > 0)
				recommended_worldid--;
			else
				recommended_worldid = recommended_worldcount - 1;

			recommended_message.change_text(recommended_worlds[recommended_worldid].message);

			return Button::State::NORMAL;
		}
		else if (id == Buttons::BT_VIEWRECOMMENDED_NEXT)
		{
			if (recommended_worldid < recommended_worldcount - 1)
				recommended_worldid++;
			else
				recommended_worldid = 0;

			recommended_message.change_text(recommended_worlds[recommended_worldid].message);

			return Button::State::NORMAL;
		}
		else if (id == Buttons::BT_CHANGEREGION)
		{
			UI::get().emplace<UIRegion>();

			deactivate();

			return Button::State::NORMAL;
		}
		else if (id >= Buttons::BT_WORLD0 && id < Buttons::BT_CHANNEL0)
		{
			toggle_recommended(false);

			buttons[Buttons::BT_WORLD0 + worldid]->set_state(Button::State::NORMAL);

			worldid = id - Buttons::BT_WORLD0;

			ServerStatusRequestPacket(worldid).dispatch();

			world_selected = true;
			clear_selected_world();
			change_world(worlds[worldid]);

			return Button::State::PRESSED;
		}
		else if (id >= Buttons::BT_CHANNEL0 && id < Buttons::BT_ENTERWORLD)
		{
			uint8_t selectedch = static_cast<uint8_t>(id - Buttons::BT_CHANNEL0);

			if (selectedch != channelid)
			{
				buttons[Buttons::BT_CHANNEL0 + channelid]->set_state(Button::State::NORMAL);
				channelid = static_cast<uint8_t>(id - Buttons::BT_CHANNEL0);
				buttons[Buttons::BT_CHANNEL0 + channelid]->set_state(Button::State::PRESSED);
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

	void UIWorldSelect::toggle_recommended(bool active)
	{
		if (recommended_worldcount > 0)
		{
			recommended_worldid = 0;
			show_recommended = active;

			buttons[Buttons::BT_VIEWALL]->set_active(active);
			buttons[Buttons::BT_VIEWRECOMMENDED]->set_active(!active);
			buttons[Buttons::BT_VIEWRECOMMENDED_SELECT]->set_active(active);
			buttons[Buttons::BT_VIEWRECOMMENDED_CANCEL]->set_active(active);
			buttons[Buttons::BT_VIEWRECOMMENDED_PREV]->set_active(active);
			buttons[Buttons::BT_VIEWRECOMMENDED_NEXT]->set_active(active);

			if (recommended_worldcount <= 1)
			{
				buttons[Buttons::BT_VIEWRECOMMENDED_PREV]->set_state(Button::State::DISABLED);
				buttons[Buttons::BT_VIEWRECOMMENDED_NEXT]->set_state(Button::State::DISABLED);
			}
			else
			{
				buttons[Buttons::BT_VIEWRECOMMENDED_PREV]->set_state(Button::State::NORMAL);
				buttons[Buttons::BT_VIEWRECOMMENDED_NEXT]->set_state(Button::State::NORMAL);
			}

			if (!active)
				recommended_message.change_text("");
			else
				recommended_message.change_text(recommended_worlds[recommended_worldid].message);
		}
	}

	void UIWorldSelect::clear_selected_world()
	{
		channelid = 0;

		for (size_t i = Buttons::BT_CHANNEL0; i < Buttons::BT_ENTERWORLD; i++)
			buttons[i]->set_state(Button::State::NORMAL);

		buttons[Buttons::BT_CHANNEL0]->set_state(Button::State::PRESSED);

		for (size_t i = 0; i < Buttons::BT_ENTERWORLD - Buttons::BT_CHANNEL0; i++)
		{
			buttons[Buttons::BT_CHANNEL0 + i]->set_active(false);

			channel_gauge[i].update(0);
		}

		buttons[Buttons::BT_ENTERWORLD]->set_active(false);

		worldNoticeMessage.change_text("");
	}

	uint16_t UIWorldSelect::get_next_world(uint16_t id, bool upward)
	{
		uint16_t next_world;

		if (world_map[Buttons::BT_WORLD0] == Worlds::SCANIA)
		{
			switch (id)
			{
				case Buttons::BT_WORLD0:
					next_world = (upward) ? Worlds::REBOOT0 : Worlds::BERA;
					break;
				case Buttons::BT_WORLD1:
					next_world = (upward) ? Worlds::SCANIA : Worlds::AURORA;
					break;
				case Buttons::BT_WORLD2:
					next_world = (upward) ? Worlds::BERA : Worlds::ELYSIUM;
					break;
				case Buttons::BT_WORLD3:
					next_world = (upward) ? Worlds::AURORA : Worlds::REBOOT0;
					break;
				case Buttons::BT_WORLD4:
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
				case Buttons::BT_WORLD0:
					next_world = (upward) ? Worlds::REBOOT0 : Worlds::REBOOT0;
					break;
				case Buttons::BT_WORLD4:
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