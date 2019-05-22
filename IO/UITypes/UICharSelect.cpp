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
#include "UICharSelect.h"
#include "UIWorldSelect.h"
#include "UICharCreation.h"
#include "UISoftkey.h"

#include "../UI.h"
#include "../Configuration.h"

#include "../Components/MapleButton.h"
#include "../Components/AreaButton.h"
#include "../Audio/Audio.h"
#include "../Character/Job.h"

#include "../Net/Packets/SelectCharPackets.h"

#include <nlnx/nx.hpp>

namespace jrc
{
	UICharSelect::UICharSelect(std::vector<CharEntry> c, int8_t characters_count, int32_t s, int8_t pic) : characters(c), characters_count(characters_count), slots(s), pic(pic)
	{
		std::string version_text = Configuration::get().get_version();
		version = Text(Text::Font::A11M, Text::Alignment::LEFT, Text::Color::LEMONGRASS, "Ver. " + version_text);

		pagepos = Point<int16_t>(247, 452);
		worldpos = Point<int16_t>(586, 36);
		charinfopos = Point<int16_t>(671, 329);

		selected_character = Setting<DefaultCharacter>::get().load();
		selected_page = selected_character / PAGESIZE;
		page_count = std::ceil((double)slots / (double)PAGESIZE);

		nl::node login = nl::nx::ui["Login.img"];
		nl::node common = login["Common"];
		nl::node charselect = login["CharSelect"];

		nl::node selectWorld = common["selectWorld"];
		nl::node selectedWorld = charselect["selectedWorld"];
		nl::node pageNew = charselect["pageNew"];

		world_dimensions = Texture(selectWorld).get_dimensions();

		if (auto worldselect = UI::get().get_element<UIWorldSelect>())
		{
			uint8_t world_id = worldselect->get_world_id();
			uint8_t channel_id = worldselect->get_channel_id();

			world_sprites.emplace_back(selectWorld, worldpos);
			world_sprites.emplace_back(selectedWorld["icon"][world_id], worldpos - Point<int16_t>(14, -1));
			world_sprites.emplace_back(selectedWorld["name"][world_id], worldpos - Point<int16_t>(8, 1));
			world_sprites.emplace_back(selectedWorld["ch"][channel_id], worldpos - Point<int16_t>(0, 1));
		}

		nl::node map = nl::nx::map["Back"]["login.img"];
		nl::node ani = map["ani"];

		sprites.emplace_back(map["back"]["13"], Point<int16_t>(392, 287)); // From v203.3
		sprites.emplace_back(ani["17"], Point<int16_t>(151, 273)); // From v167
		sprites.emplace_back(ani["18"], Point<int16_t>(365, 242)); // From v167
		sprites.emplace_back(ani["19"], Point<int16_t>(191, 198)); // From v167
		sprites.emplace_back(common["frame"], Point<int16_t>(399, 289));
		sprites.emplace_back(common["step"]["2"], Point<int16_t>(40, -10));
		sprites.emplace_back(pageNew["base"]["0"], pagepos);

		charinfo = charselect["charInfo"];
		charslot = charselect["charSlot"]["0"];
		pagenumber = Charset(pageNew["number"], Charset::Alignment::LEFT);
		pagenumberpos = pageNew["numberpos"];

		signpost[0] = charselect["adventure"]["0"];
		signpost[1] = charselect["knight"]["0"];
		signpost[2] = charselect["aran"]["0"];

		nametag = charselect["nameTag"];

		buttons[Buttons::CHARACTER_SELECT] = std::make_unique<MapleButton>(charselect["BtSelect"], Point<int16_t>(601, 383));
		buttons[Buttons::CHARACTER_NEW] = std::make_unique<MapleButton>(charselect["BtNew"], Point<int16_t>(200, 485));
		buttons[Buttons::CHARACTER_DELETE] = std::make_unique<MapleButton>(charselect["BtDelete"], Point<int16_t>(316, 485));
		buttons[Buttons::PAGELEFT] = std::make_unique<MapleButton>(charselect["pageL"], Point<int16_t>(98, 481));
		buttons[Buttons::PAGERIGHT] = std::make_unique<MapleButton>(charselect["pageR"], Point<int16_t>(485, 481));
		buttons[Buttons::CHANGEPIC] = std::make_unique<MapleButton>(common["BtChangePIC"], Point<int16_t>(0, 35));
		buttons[Buttons::RESETPIC] = std::make_unique<MapleButton>(login["WorldSelect"]["BtResetPIC"], Point<int16_t>(0, 75));
		buttons[Buttons::EDITCHARLIST] = std::make_unique<MapleButton>(charselect["EditCharList"]["BtCharacter"], Point<int16_t>(1, 115));
		buttons[Buttons::BACK] = std::make_unique<MapleButton>(common["BtStart"], Point<int16_t>(0, 505));

		for (size_t i = 0; i < PAGESIZE; i++)
			buttons[Buttons::CHARACTER_SLOT0 + i] = std::make_unique<AreaButton>(get_character_slot_pos(i, 105, 144), Point<int16_t>(50, 80));

		levelset = Charset(charselect["lv"], Charset::Alignment::CENTER);
		namelabel = OutlinedText(Text::Font::A15B, Text::Alignment::CENTER, Text::Color::WHITE, Text::Color::IRISHCOFFEE);

		for (size_t i = 0; i < InfoLabel::NUM_LABELS; i++)
			infolabels[i] = OutlinedText(Text::Font::A11M, Text::Alignment::RIGHT, Text::Color::WHITE, Text::Color::TOBACCOBROWN);

		for (auto& entry : characters)
		{
			charlooks.emplace_back(entry.look);
			nametags.emplace_back(nametag, Text::Font::A13M, entry.stats.name);
		}

		emptyslot_effect = charselect["character"]["0"];
		emptyslot = charselect["character"]["1"]["0"];

		selectedslot_effect[0] = charselect["effect"][0];
		selectedslot_effect[1] = charselect["effect"][1];

		chatslotlabel = OutlinedText(Text::Font::A12M, Text::Alignment::LEFT, Text::Color::PORCELAIN, Text::Color::BROWNDERBY);
		chatslotlabel.change_text(get_slot_text());

		position = Point<int16_t>(0, 0);
		dimension = Point<int16_t>(800, 600);
		active = true;

		update_buttons();

		if (characters_count > 0)
			update_selected_character();
		}

	void UICharSelect::draw(float inter) const
		{
		UIElement::draw_sprites(inter);

		version.draw(position + Point<int16_t>(707, -9));
		charslot.draw(position + Point<int16_t>(589, 96 - charslot_y));
		chatslotlabel.draw(position + Point<int16_t>(700, 100 - charslot_y));

		for (auto sprite : world_sprites)
			sprite.draw(position, inter);

		std::string total = pad_number_with_leading_zero(page_count);
		std::string current = pad_number_with_leading_zero(selected_page + 1);

		pagenumber.draw(current.substr(0, 1), position + pagepos + Point<int16_t>(pagenumberpos[0]));
		pagenumber.draw(current.substr(1, 1), position + pagepos + Point<int16_t>(pagenumberpos[1]));
		pagenumber.draw(total.substr(0, 1), position + pagepos + Point<int16_t>(pagenumberpos[2]));
		pagenumber.draw(total.substr(1, 1), position + pagepos + Point<int16_t>(pagenumberpos[3]));

		std::list<int8_t> fliplist = { 2, 3, 6, 7 };

		for (size_t i = 0; i < PAGESIZE; i++)
		{
			auto index = i + selected_page * PAGESIZE;
			bool flip_character = std::find(fliplist.begin(), fliplist.end(), i) != fliplist.end();
			bool selectedslot = index == selected_character;

			if (index < characters_count)
		{
				Point<int16_t> charpos = get_character_slot_pos(i, 135, 224);
				DrawArgument chararg = DrawArgument(charpos, flip_character);

				nametags[index].draw(charpos);

				const StatsEntry& character_stats = characters[index].stats;

				if (selectedslot)
	{
					selectedslot_effect[1].draw(charpos + Point<int16_t>(-5, 16), inter);

					int8_t lvy = -115;
					Point<int16_t> pos_adj = Point<int16_t>(665, 355);

					charinfo.draw(position + charinfopos);

					std::string levelstr = std::to_string(character_stats.stats[Maplestat::Id::LEVEL]);
					int16_t lvx = levelset.draw(levelstr, pos_adj + Point<int16_t>(12, lvy));
					levelset.draw('l', pos_adj + Point<int16_t>(1 - lvx / 2, lvy));

					namelabel.draw(pos_adj + Point<int16_t>(8, -106));

					for (size_t i = 0; i < InfoLabel::NUM_LABELS; i++)
		{
						Point<int16_t> labelpos = pos_adj + get_infolabel_pos(i);
						infolabels[i].draw(labelpos);
			}
				}

				uint8_t j = 0;
				uint16_t job = character_stats.stats[Maplestat::Id::JOB];

				if (job >= 0 && job < 1000)
					j = 0;
				else if (job >= 1000 && job < 2000)
					j = 1;
				else if (job >= 2000 && job < 2200)
					j = 2;
				else
					j = 0;

				signpost[j].draw(chararg);
				charlooks[index].draw(chararg, inter);

				if (selectedslot)
					selectedslot_effect[0].draw(charpos + Point<int16_t>(-5, -298), inter);
			}
			else if (i < slots)
			{
				Point<int16_t> emptyslotpos = get_character_slot_pos(i, 130, 224);

				emptyslot_effect.draw(emptyslotpos, inter);
				emptyslot.draw(DrawArgument(emptyslotpos, flip_character));
			}
		}

		UIElement::draw_buttons(inter);
			}

	void UICharSelect::update()
	{
		UIElement::update();

		if (show_timestamp)
	{
			if (timestamp > 0)
		{
				timestamp -= Constants::TIMESTEP;

				if (timestamp <= 176)
					charslot_y += 1;
		}
		}
		else
		{
			if (timestamp <= 176)
			{
				timestamp += Constants::TIMESTEP;

				if (charslot_y >= 0)
					charslot_y -= 1;
					}
				}

		for (auto& charlook : charlooks)
			charlook.update(Constants::TIMESTEP);

		for (auto& effect : selectedslot_effect)
			effect.update();

		emptyslot_effect.update();
					}

	Cursor::State UICharSelect::send_cursor(bool clicked, Point<int16_t> cursorpos)
	{
		Rectangle<int16_t> charslot_bounds = Rectangle<int16_t>(
			worldpos,
			worldpos + world_dimensions
			);

		if (charslot_bounds.contains(cursorpos))
		{
			if (clicked)
			{
				show_timestamp = !show_timestamp;

				return Cursor::State::CLICKING;
			}
		}

		return UIElement::send_cursor(clicked, cursorpos);
	}

	void UICharSelect::send_key(int32_t keycode, bool pressed)
	{
		if (pressed)
		{
			if (keycode == KeyAction::Id::LEFT)
			{
				// selected_characters--;
			}
			else if (keycode == KeyAction::Id::UP)
			{
				// same page, get position directly above current position
		}
			else if (keycode == KeyAction::Id::RIGHT)
			{
				// selected_characters++;
	}
			else if (keycode == KeyAction::Id::DOWN)
			{
				// same page, get position directly below current position
			}
			else if (keycode == KeyAction::Id::TAB)
			{
				// toggle leaf cursor for menus
				// 0 - Start
				// 1 - Create
				// 2 - Delete
				// 3 - Clear
			}
			else if (keycode == KeyAction::Id::ESCAPE)
			{
				// back();
			}
		}
	}

	void UICharSelect::add_character(CharEntry&& character)
	{
		charlooks.emplace_back(character.look);
		nametags.emplace_back(nametag, Text::Font::A13M, character.stats.name);
		characters.emplace_back(std::forward<CharEntry>(character));

		characters_count++;

		auto page_matches = (characters_count - 1) / PAGESIZE == selected_page;

		if (!page_matches)
			button_pressed(Buttons::PAGERIGHT);

		update_buttons();

		if (characters_count > 1)
			select_last_slot();
		else
			update_selected_character();

		chatslotlabel.change_text(get_slot_text());
	}

	void UICharSelect::remove_character(int32_t id)
	{
		for (size_t i = 0; i < characters.size(); i++)
		{
			if (characters[i].id == id)
			{
				charlooks.erase(charlooks.begin() + i);
				nametags.erase(nametags.begin() + i);
				characters.erase(characters.begin() + i);

				characters_count--;

				if (selected_page > 0)
				{
					auto page_matches = (characters_count - 1) / PAGESIZE == selected_page;

					if (!page_matches)
						button_pressed(Buttons::PAGELEFT);
				}

				update_buttons();

				if (selected_character < characters_count)
					update_selected_character();
		else
					select_last_slot();

				chatslotlabel.change_text(get_slot_text());

				return;
		}
		}
	}

	const CharEntry& UICharSelect::get_character(int32_t id)
		{
		for (auto& character : characters)
			if (character.id == id)
				return character;

		Console::get().print(__func__, "Warning: Invalid character id '" + std::to_string(id) + "'");

		static const CharEntry null_character = { {}, {}, 0 };

		return null_character;
		}

	Button::State UICharSelect::button_pressed(uint16_t buttonid)
	{
		switch (buttonid)
		{
		case Buttons::CHARACTER_SELECT:
		{
			Setting<DefaultCharacter>::get().save(selected_character);
			int32_t id = characters[selected_character].id;

			switch (pic)
		{
		case 0:
				UI::get().emplace<UISoftkey>(
					[id](const std::string& pic)
					{
						RegisterPicPacket(id, pic).dispatch();
					}
				);

			break;
		case 1:
				UI::get().emplace<UISoftkey>(
					[id](const std::string& pic)
					{
						SelectCharPicPacket(pic, id).dispatch();
					}
				);

			break;
		case 2:
				Sound(Sound::Name::SELECTCHAR).play();
				SelectCharPacket(id).dispatch();

			break;
		}
	}
		break;
		case Buttons::CHARACTER_NEW:
			deactivate();

			UI::get().emplace<UICharCreation>();
			break;
		case Buttons::CHARACTER_DELETE:
	{
			int32_t id = characters[selected_character].id;

			UI::get().emplace<UISoftkey>(
				[id](const std::string& pic)
		{
					DeleteCharPacket(pic, id).dispatch();
		}
			);
		}
		break;
		case Buttons::PAGELEFT:
		{
			uint8_t previous_page = selected_page;

			if (selected_page > 0)
				selected_page--;
			else if (characters_count > PAGESIZE)
				selected_page = page_count - 1;

			if (previous_page != selected_page)
				update_buttons();

			select_last_slot();
	}
		break;
		case Buttons::PAGERIGHT:
		{
			uint8_t previous_page = selected_page;

			if (selected_page < page_count - 1)
				selected_page++;
			else
				selected_page = 0;

			if (previous_page != selected_page)
	{
				update_buttons();

				button_pressed(Buttons::CHARACTER_SLOT0);
			}
		}
		break;
		case Buttons::CHANGEPIC:
			break;
		case Buttons::RESETPIC:
			break;
		case Buttons::EDITCHARLIST:
			break;
		case Buttons::BACK:
			deactivate();

			Sound(Sound::Name::SCROLLUP).play();

			if (auto worldselect = UI::get().get_element<UIWorldSelect>())
				worldselect->makeactive();

			break;
		default:
			if (buttonid >= Buttons::CHARACTER_SLOT0)
	{
				uint8_t previous_character = selected_character;
				selected_character = buttonid - Buttons::CHARACTER_SLOT0 + selected_page * PAGESIZE;

				if (previous_character != selected_character)
				{
					if (previous_character < characters_count)
		{
						charlooks[previous_character].set_stance(Stance::Id::STAND1);
						nametags[previous_character].set_selected(false);
					}

					if (selected_character < characters_count)
						update_selected_character();
				}
			}

				break;
		}

		return Button::State::NORMAL;
	}

	void UICharSelect::update_buttons()
	{
		for (size_t i = 0; i < PAGESIZE; i++)
		{
			auto index = i + selected_page * PAGESIZE;

			if (index < characters_count)
				buttons[Buttons::CHARACTER_SLOT0 + i]->set_state(Button::State::NORMAL);
			else
				buttons[Buttons::CHARACTER_SLOT0 + i]->set_state(Button::State::DISABLED);
		}

		if (characters_count >= slots)
			buttons[Buttons::CHARACTER_NEW]->set_state(Button::State::DISABLED);
		else
			buttons[Buttons::CHARACTER_NEW]->set_state(Button::State::NORMAL);

		bool character_found = false;

		for (int i = PAGESIZE - 1; i >= 0; i--)
		{
			auto index = i + selected_page * PAGESIZE;

			if (index < characters_count)
			{
				character_found = true;

				break;
	}
		}

		buttons[Buttons::CHARACTER_SELECT]->set_active(character_found);
		buttons[Buttons::CHARACTER_DELETE]->set_state(character_found ? Button::State::NORMAL : Button::State::DISABLED);
	}

	void UICharSelect::update_selected_character()
	{
		Sound(Sound::Name::CHARSELECT).play();

		charlooks[selected_character].set_stance(Stance::Id::WALK1);
		nametags[selected_character].set_selected(true);

		const StatsEntry& character_stats = characters[selected_character].stats;

		namelabel.change_text(character_stats.name);

		for (size_t i = 0; i < InfoLabel::NUM_LABELS; i++)
			infolabels[i].change_text(get_infolabel(i, character_stats));
	}

	void UICharSelect::select_last_slot()
	{
		for (int i = PAGESIZE - 1; i >= 0; i--)
		{
			auto index = i + selected_page * PAGESIZE;

			if (index < characters_count)
		{
				button_pressed(i + Buttons::CHARACTER_SLOT0);

				return;
			}
		}
	}

	std::string UICharSelect::get_slot_text()
	{
		show_timestamp = true;
		timestamp = 7000;
		charslot_y = 0;

		return pad_number_with_leading_zero(characters_count) + "/" + pad_number_with_leading_zero(slots);
	}

	std::string UICharSelect::pad_number_with_leading_zero(uint8_t value) const
		{
		std::string return_val = std::to_string(value);
		return_val.insert(return_val.begin(), 2 - return_val.length(), '0');

		return return_val;
		}

	Point<int16_t> UICharSelect::get_character_slot_pos(size_t index, uint16_t x_adj, uint16_t y_adj) const
	{
		auto x = 125 * (index % 4);
		auto y = 200 * (index > 3);

		return Point<int16_t>(x + x_adj, y + y_adj);
	}

	Point<int16_t> UICharSelect::get_infolabel_pos(size_t index) const
	{
		switch (index)
	{
		case InfoLabel::JOB:
			return { 68, -74 };
		case InfoLabel::STR:
			return { -3, -50 };
		case InfoLabel::DEX:
			return { -3, -27 };
		case InfoLabel::INT:
			return { 68, -50 };
		case InfoLabel::LUK:
			return { 68, -27 };
		case InfoLabel::NUM_LABELS:
			break;
		default:
			break;
		}

		return {};
	}

	std::string UICharSelect::get_infolabel(size_t index, StatsEntry character_stats) const
	{
		switch (index)
	{
		case InfoLabel::JOB:
			return Job(character_stats.stats[Maplestat::Id::JOB]).get_name();
		case InfoLabel::STR:
			return std::to_string(character_stats.stats[Maplestat::Id::STR]);
		case InfoLabel::DEX:
			return std::to_string(character_stats.stats[Maplestat::Id::DEX]);
		case InfoLabel::INT:
			return std::to_string(character_stats.stats[Maplestat::Id::INT]);
		case InfoLabel::LUK:
			return std::to_string(character_stats.stats[Maplestat::Id::LUK]);
		case InfoLabel::NUM_LABELS:
			break;
		default:
			break;
		}

		return "";
	}
}