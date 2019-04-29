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

#include "UISoftkey.h"
#include "UICharCreation.h"
#include "UIWorldSelect.h"

#include "../UI.h"
#include "../Components/MapleButton.h"
#include "../Components/AreaButton.h"

#include "../../Constants.h"
#include "../../Configuration.h"
#include "../../Audio/Audio.h"
#include "../../Character/Job.h"
#include "../../Net/Packets/SelectCharPackets.h"

#include "nlnx/nx.hpp"

namespace jrc
{
	UICharSelect::UICharSelect(std::vector<CharEntry> cs, uint8_t charcount, uint8_t slots, int8_t pic) : characters(cs), charcount_absolute(charcount), slots_absolute(slots), require_pic(pic)
	{
		version = { Text::Font::A11M, Text::Alignment::LEFT, Text::Color::LEMONGRASS, "Ver. 203.3" };

		uint8_t world_id = 0;
		uint8_t channel_id = 0;

		if (auto worldselect = UI::get().get_element<UIWorldSelect>())
		{
			world_id = worldselect->get_world_id();
			channel_id = worldselect->get_channel_id();
		}

		selected_absolute = Setting<DefaultCharacter>::get().load();
		selected_relative = selected_absolute % PAGESIZE;
		page = selected_absolute / PAGESIZE;

		nl::node map = nl::nx::map["Back"]["login.img"];
		nl::node obj = nl::nx::map["Obj"]["login.img"];
		nl::node login = nl::nx::ui["Login.img"];
		nl::node common = login["Common"];
		charselect = login["CharSelect"];
		nl::node selectedWorld = charselect["selectedWorld"];

		sprites.emplace_back(map["back"]["13"], Point<int16_t>(392, 287)); // From v203.3
		sprites.emplace_back(common["frame"], Point<int16_t>(399, 289));
		sprites.emplace_back(common["step"]["2"], Point<int16_t>(40, -10));
		sprites.emplace_back(charselect["charInfo"], Point<int16_t>(671, 329));
		sprites.emplace_back(charselect["charSlot"]["0"], Point<int16_t>(589, 96));
		sprites.emplace_back(common["selectWorld"], Point<int16_t>(586, 36));
		sprites.emplace_back(selectedWorld["icon"][std::to_string(world_id)], Point<int16_t>(572, 37));
		sprites.emplace_back(selectedWorld["name"][std::to_string(world_id)], Point<int16_t>(578, 35));
		sprites.emplace_back(selectedWorld["ch"][std::to_string(channel_id)], Point<int16_t>(586, 35));

		nl::node pageNew = charselect["pageNew"];
		pages = pageNew["number"];

		pages_pos = Point<int16_t>(247, 452);

		sprites.emplace_back(pageNew["base"]["0"], pages_pos);

		pages_start = 44;
		int8_t pages_gap = 24;
		int8_t pages_end = pages_start + pages_gap;
		num_width = 7;

		pages_y = 6;

		total_pages = slots_absolute / PAGESIZE;

		if (total_pages < 10)
		{
			sprites.emplace_back(pages["0"], pages_pos + Point<int16_t>(pages_end, pages_y));
			sprites.emplace_back(pages[total_pages], pages_pos + Point<int16_t>(pages_end + num_width, pages_y));
		}
		else
		{
			std::string total_pages_s = std::to_string(total_pages);
			std::string first = total_pages_s.substr(0, 1);
			std::string second = total_pages_s.substr(1, 1);

			sprites.emplace_back(pages[first], pages_pos + Point<int16_t>(pages_end, pages_y));
			sprites.emplace_back(pages[second], pages_pos + Point<int16_t>(pages_end + num_width, pages_y));
		}

		int8_t actual_page = page + 1;

		if (actual_page < 10)
		{
			sprites.emplace_back(pages["0"], pages_pos + Point<int16_t>(pages_start, pages_y));
			sprites.emplace_back(pages[actual_page], pages_pos + Point<int16_t>(pages_start + num_width, pages_y));
		}
		else
		{
			std::string actual_page_s = std::to_string(actual_page);
			std::string first = actual_page_s.substr(0, 1);
			std::string second = actual_page_s.substr(1, 1);

			sprites.emplace_back(pages[first], pages_pos + Point<int16_t>(pages_start, pages_y));
			sprites.emplace_back(pages[second], pages_pos + Point<int16_t>(pages_start + num_width, pages_y));
		}

		nametag = charselect["nameTag"];

		buttons[BT_SELECTCHAR] = std::make_unique<MapleButton>(charselect["BtSelect"], Point<int16_t>(601, 383));
		buttons[BT_CREATECHAR] = std::make_unique<MapleButton>(charselect["BtNew"], Point<int16_t>(200, 485));
		buttons[BT_DELETECHAR] = std::make_unique<MapleButton>(charselect["BtDelete"], Point<int16_t>(316, 485));
		buttons[BT_PAGELEFT] = std::make_unique<MapleButton>(charselect["pageL"], Point<int16_t>(98, 481));
		buttons[BT_PAGERIGHT] = std::make_unique<MapleButton>(charselect["pageR"], Point<int16_t>(485, 481));
		buttons[BT_CHANGEPIC] = std::make_unique<MapleButton>(common["BtChangePIC"], Point<int16_t>(0, 35));
		buttons[BT_RESETPIC] = std::make_unique<MapleButton>(login["WorldSelect"]["BtResetPIC"], Point<int16_t>(0, 75));
		buttons[BT_CHANGELOC] = std::make_unique<MapleButton>(charselect["EditCharList"]["BtCharacter"], Point<int16_t>(1, 115));
		buttons[BT_BACK] = std::make_unique<MapleButton>(common["BtStart"], Point<int16_t>(0, 505));

		for (uint8_t i = 0; i < PAGESIZE; i++)
		{
			buttons[BT_CHAR0 + i] = std::make_unique<AreaButton>(
				Point<int16_t>(105 + (120 * (i % 4)), 170 + (200 * (i > 3))),
				Point<int16_t>(50, 80)
				);
		}

		levelset = { charselect["lv"], Charset::CENTER };
		namelabel = { Text::A15B, Text::CENTER, Text::WHITE, Text::TOBACCOBROWN };

		for (size_t i = 0; i < NUM_LABELS; i++)
			infolabels[i] = { Text::A11M, Text::RIGHT, Text::WHITE, Text::TOBACCOBROWN };

		for (auto& entry : characters)
		{
			charlooks.emplace_back(entry.look);
			nametags.emplace_back(nametag, Text::A13M, entry.stats.name);
		}

		update_counts();
		update_selection();

		charSlot = { Text::Font::A12M, Text::Alignment::LEFT, Text::Color::WHITE, Text::Color::JAMBALAYA };
		charSlot.change_text(get_slot_text());

		position = { 0, 0 };
		dimension = { 800, 600 };
		active = true;
	}

	void UICharSelect::draw(float alpha) const
	{
		UIElement::draw_sprites(alpha);

		version.draw(position + Point<int16_t>(707, -9));
		charSlot.draw(position + Point<int16_t>(700, 100));

		int f = 0;
		bool flip = false;

		for (uint8_t i = 0; i < slots_relative; i++)
		{
			if (f >= 2 && f <= 3)
			{
				flip = true;
			}
			else
			{
				if (f > 2)
				{
					f = 0;
					flip = false;
				}
			}

			f++;

			if (i < charcount_relative)
			{
				Point<int16_t> charpos = get_char_pos(i);
				uint8_t index = i + page * PAGESIZE;

				uint16_t job = characters[index].stats.stats[Maplestat::JOB];
				auto adventure = Sprite(charselect["adventure"], DrawArgument(flip));
				auto aran = Sprite(charselect["aran"], DrawArgument(flip));
				auto knight = Sprite(charselect["knight"], DrawArgument(flip));

				if (job >= 0 && job < 1000)
					adventure.draw(charpos, alpha);
				else if (job >= 1000 && job < 2000)
					knight.draw(charpos, alpha);
				else if (job >= 2000 && job < 2200)
					aran.draw(charpos, alpha);
				else
					std::cout << "Job sign unhandled for job (" << job << ")" << std::endl;

				DrawArgument chararg = DrawArgument(charpos, flip);

				charlooks[index].draw(chararg, alpha);
				nametags[index].draw(charpos);
			}
			else
			{
				Point<int16_t> position_slot(130 + (125 * (i % 4)), 224 + (200 * (i > 3)));

				// TODO: This needs to animate, not sure why it's static
				auto emptysloteff = Animation(charselect["character"]["0"]);
				emptysloteff.draw(position_slot, alpha);

				auto emptyslot = Sprite(charselect["character"]["1"]["0"], DrawArgument(flip));
				emptyslot.draw(position_slot, alpha);
			}
		}

		if (selected_absolute < charcount_absolute)
		{
			int8_t lvy = -115;
			Point<int16_t> charinfopos = Point<int16_t>(665, 355);
			const StatsEntry& stats = characters[selected_absolute].stats;

			std::string levelstr = std::to_string(stats.stats[Maplestat::LEVEL]);
			int16_t lvx = levelset.draw(levelstr, charinfopos + Point<int16_t>(12, lvy));
			levelset.draw('l', charinfopos + Point<int16_t>(1 - lvx / 2, lvy));

			namelabel.draw(charinfopos + Point<int16_t>(8, -106));

			for (size_t i = 0; i < NUM_LABELS; i++)
			{
				Point<int16_t> labelpos = charinfopos + get_label_pos(i);
				infolabels[i].draw(labelpos);
			}
		}

		UIElement::draw_buttons(alpha);
	}

	void UICharSelect::update()
	{
		UIElement::update();

		for (auto& chit : charlooks)
			chit.update(Constants::TIMESTEP);
	}

	Button::State UICharSelect::button_pressed(uint16_t bid)
	{
		if (bid >= BT_CHAR0)
		{
			uint8_t index = selected_absolute;

			if (selected_absolute >= charcount_absolute)
				index -= 1;

			nametags[index].set_selected(false);
			charlooks[index].set_stance(Stance::STAND1);
			buttons[BT_CHAR0 + selected_relative]->set_state(Button::NORMAL);

			selected_relative = static_cast<uint8_t>(bid - BT_CHAR0);
			selected_absolute = selected_relative + page * PAGESIZE;
			update_selection();
			return Button::IDENTITY;
		}
		else
		{
			switch (bid)
			{
			case BT_SELECTCHAR:
				send_selection();
				return Button::NORMAL;
			case BT_CREATECHAR:
				active = false;
				UI::get().emplace<UICharCreation>();
				return Button::NORMAL;
			case BT_DELETECHAR:
				send_deletion();
				return Button::NORMAL;
			case BT_PAGERIGHT:
				for (int i = 0; i < PAGESIZE; i++) {
					int index = i + page * PAGESIZE;

					if (index < charcount_absolute)
					{
						nametags[index].set_selected(false);
						charlooks[index].set_stance(Stance::STAND1);
					}

					buttons[BT_CHAR0 + i]->set_state(Button::NORMAL);
				}

				if (page < total_pages - 1)
					page++;
				else
					page = 0;

				update_counts();
				update_selection();
				return Button::IDENTITY;
			case BT_PAGELEFT:
				for (int i = 0; i < PAGESIZE; i++) {
					int index = i + page * PAGESIZE;

					if (index < charcount_absolute)
					{
						nametags[index].set_selected(false);
						charlooks[index].set_stance(Stance::STAND1);
					}

					buttons[BT_CHAR0 + i]->set_state(Button::NORMAL);
				}

				if (page > 0)
					page--;

				update_counts();
				update_selection();
				return Button::IDENTITY;
			case BT_BACK:
				active = false;

				if (auto worldselect = UI::get().get_element<UIWorldSelect>())
					worldselect->makeactive();

				return Button::PRESSED;
			default:
				return Button::NORMAL;
			}
		}
	}

	void UICharSelect::update_selection()
	{
		if (selected_relative >= charcount_relative)
			return;

		if (selected_absolute >= charcount_absolute)
			return;

		charlooks[selected_absolute].set_stance(Stance::WALK1);
		nametags[selected_absolute].set_selected(true);

		buttons[BT_CHAR0 + selected_relative]->set_state(Button::PRESSED);
		namelabel.change_text(characters[selected_absolute].stats.name);
		charSlot.change_text(get_slot_text());

		for (size_t i = 0; i < NUM_LABELS; i++)
			infolabels[i].change_text(get_label_string(i));
	}

	void UICharSelect::update_counts()
	{
		charcount_relative = charcount_absolute;

		if (charcount_relative > (page + 1) * PAGESIZE)
			charcount_relative = PAGESIZE;
		else if (charcount_relative < page * PAGESIZE)
			charcount_relative = 0;
		else
			charcount_relative -= page * PAGESIZE;

		selected_absolute = selected_relative + page * PAGESIZE;
		selected_relative = selected_absolute % PAGESIZE;

		if (selected_relative >= charcount_relative)
			selected_relative = 0;

		slots_relative = slots_absolute - page * PAGESIZE;

		if (slots_relative > PAGESIZE)
			slots_relative = PAGESIZE;

		if (charcount_absolute < slots_absolute)
			buttons[BT_CREATECHAR]->set_state(Button::NORMAL);
		else
			buttons[BT_CREATECHAR]->set_state(Button::DISABLED);

		if (charcount_relative > 0)
			buttons[BT_DELETECHAR]->set_state(Button::NORMAL);
		else
			buttons[BT_DELETECHAR]->set_state(Button::DISABLED);

		for (uint8_t i = 0; i < PAGESIZE; i++)
		{
			if (i < charcount_relative)
				buttons[BT_CHAR0 + i]->set_state(Button::NORMAL);
			else
				buttons[BT_CHAR0 + i]->set_state(Button::DISABLED);
		}

		int8_t actual_page = page + 1;

		if (actual_page < 10)
		{
			sprites.pop_back();
			sprites.emplace_back(pages[actual_page], pages_pos + Point<int16_t>(pages_start + num_width, pages_y));
		}
		else
		{
			std::string actual_page_s = std::to_string(actual_page);
			std::string first = actual_page_s.substr(0, 1);
			std::string second = actual_page_s.substr(1, 1);

			sprites._Pop_back_n(2);
			sprites.emplace_back(pages[first], pages_pos + Point<int16_t>(pages_start, pages_y));
			sprites.emplace_back(pages[second], pages_pos + Point<int16_t>(pages_start + num_width, pages_y));
		}
	}

	void UICharSelect::send_selection()
	{
		if (selected_relative >= charcount_relative)
		{
			buttons[BT_SELECTCHAR]->set_state(Button::MOUSEOVER);
			return;
		}

		Setting<DefaultCharacter>::get().save(selected_absolute);
		int32_t cid = characters[selected_absolute].cid;

		switch (require_pic)
		{
		case 0:
			UI::get().emplace<UISoftkey>([cid](const std::string& pic) {
				UI::get().disable();
				RegisterPicPacket(cid, pic).dispatch();
				});
			break;
		case 1:
			UI::get().emplace<UISoftkey>([cid](const std::string& pic) {
				UI::get().disable();
				SelectCharPicPacket(pic, cid).dispatch();
				});
			break;
		case 2:
			UI::get().disable();
			Sound(Sound::SELECTCHAR).play();
			SelectCharPacket(cid).dispatch();
			break;
		}
	}

	void UICharSelect::send_deletion()
	{
		if (selected_relative >= charcount_relative)
		{
			buttons[BT_DELETECHAR]->set_state(Button::MOUSEOVER);
			return;
		}

		int32_t cid = characters[selected_absolute].cid;

		UI::get().emplace<UISoftkey>([cid](const std::string& pic) {
			UI::get().disable();
			DeleteCharPacket(pic, cid).dispatch();
			});
	}

	void UICharSelect::add_character(CharEntry&& character)
	{
		characters.emplace_back(std::forward<CharEntry>(character));
		charlooks.emplace_back(character.look);
		nametags.emplace_back(nametag, Text::A13M, character.stats.name);

		charcount_absolute++;
		charcount_relative++;

		update_counts();
		update_selection();
	}

	void UICharSelect::remove_char(int32_t cid)
	{
		size_t index = 0;

		for (auto& character : characters)
		{
			if (character.cid == cid)
				break;

			index++;
		}

		if (index == characters.size())
			return;

		characters.erase(characters.begin() + index);
		charlooks.erase(charlooks.begin() + index);
		nametags.erase(nametags.begin() + index);

		charcount_absolute--;
		charcount_relative--;

		update_counts();
		update_selection();
	}

	const CharEntry& UICharSelect::get_character(int32_t cid)
	{
		for (auto& character : characters)
			if (character.cid == cid)
				return character;

		Console::get().print(__func__, "Warning: Invalid cid (" + std::to_string(cid) + ")");

		static const CharEntry null_entry{ {}, {}, 0 };

		return null_entry;
	}

	std::string UICharSelect::get_label_string(size_t label) const
	{
		const StatsEntry& stats = characters[selected_absolute].stats;

		switch (label)
		{
		case JOB:
			return Job(stats.stats[Maplestat::JOB]).get_name();
		case STR:
			return std::to_string(stats.stats[Maplestat::STR]);
		case DEX:
			return std::to_string(stats.stats[Maplestat::DEX]);
		case INT:
			return std::to_string(stats.stats[Maplestat::INT]);
		case LUK:
			return std::to_string(stats.stats[Maplestat::LUK]);
		default:
			return "";
		}
	}

	Point<int16_t> UICharSelect::get_label_pos(size_t label) const
	{
		switch (label)
		{
		case JOB:
			return{ 68, -74 };
		case STR:
			return{ -3, -50 };
		case DEX:
			return{ -3, -27 };
		case INT:
			return{ 68, -50 };
		case LUK:
			return{ 68, -27 };
		default:
			return{};
		}
	}

	Point<int16_t> UICharSelect::get_char_pos(size_t i) const
	{
		int16_t x = 135 + (120 * (i % 4));
		int16_t y = 224 + (200 * (i > 3));

		return{ x, y };
	}

	std::string UICharSelect::get_slot_text() const
	{
		int8_t actual_selected = selected_absolute + 1;
		std::string charSlotText = (actual_selected < 10 ? "0" : "") + std::to_string(actual_selected) + "/" + (slots_absolute < 10 ? "0" : "") + std::to_string(slots_absolute);

		return charSlotText;
	}
}