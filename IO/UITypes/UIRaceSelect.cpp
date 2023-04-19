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
#include "UIRaceSelect.h"

#include "UIAranCreation.h"
#include "UICharSelect.h"
#include "UICygnusCreation.h"
#include "UIExplorerCreation.h"
#include "UILoginNotice.h"
#include "UIWorldSelect.h"

#include "../UI.h"

#include "../Components/AreaButton.h"
#include "../Components/MapleButton.h"

#include "../../Configuration.h"

#include "../../Audio/Audio.h"

#ifdef USE_NX
#include <nlnx/nx.hpp>
#endif

namespace ms
{
	UIRaceSelect::UIRaceSelect() : UIElement(Point<int16_t>(0, 0), Point<int16_t>(800, 600))
	{
		std::string version_text = Configuration::get().get_version();
		version = Text(Text::Font::A12B, Text::Alignment::LEFT, Color::Name::LEMONGRASS, "Ver. " + version_text);
		version_pos = nl::nx::UI["Login.img"]["Common"]["version"]["pos"];

		nl::node Login = nl::nx::UI["Login.img"];
		nl::node Common = Login["Common"];
		nl::node RaceSelect = Login["RaceSelect_new"];

		Point<int16_t> make_pos = RaceSelect["make"]["pos"];
		Point<int16_t> make_posZero = RaceSelect["make"]["posZero"];

		pos = Point<int16_t>(std::abs(make_pos.x()), std::abs(make_pos.y()));
		posZero = Point<int16_t>(std::abs(make_posZero.x()), std::abs(make_posZero.y()));

		nl::node tempOrder = RaceSelect["order"][SELECTED_LIST];

		for (size_t i = 0; i < tempOrder.size(); i++)
			if (tempOrder[i].get_integer() != Classes::PINKBEAN)
				order.push_back(tempOrder[i]);

		hotlist = RaceSelect["hotList"][SELECTED_LIST];
		newlist = RaceSelect["newList"][SELECTED_LIST];
		bgm = RaceSelect["bgm"];

		hotlabel = RaceSelect["hotLabel"];
		hotlabelZero = RaceSelect["hotLabel2"];
		newlabel = RaceSelect["newLabel"];
		newlabelPos = RaceSelect["newLabel"]["pos"];
		hotbtn = RaceSelect["hot"];
		hotbtnPos = RaceSelect["hot"]["offset"];
		newbtn = RaceSelect["new"];
		newbtnPos = RaceSelect["new"]["offset"];

		class_index[0] = order[0];
		class_index[1] = order[1];
		class_index[2] = order[2];
		class_index[3] = order[3];
		class_index[4] = order[4];
		class_index[5] = order[5];

		mouseover[0] = true;
		mouseover[1] = false;
		mouseover[2] = false;
		mouseover[3] = false;
		mouseover[4] = false;
		mouseover[5] = false;

		nl::node button = RaceSelect["button"];
		nl::node buttonDisabled = RaceSelect["buttonDisabled"];
		buttonIntervalX = button["intervalX"];
		buttonPos = button["pos"];

		class_count = order.size();
		class_isdisabled = std::vector<bool>(class_count, true);
		class_disabled = std::vector<BoolPair<Texture>>(class_count);
		class_normal = std::vector<BoolPair<Texture>>(class_count);
		class_background = std::vector<Texture>(class_count);
		class_details = std::vector<Texture>(class_count);
		class_title = std::vector<Texture>(class_count);
		class_map = std::vector<uint16_t>(class_count);

		class_isdisabled[Classes::EXPLORERS] = false;
		class_isdisabled[Classes::CYGNUSKNIGHTS] = false;
		class_isdisabled[Classes::ARAN] = false;

		back = RaceSelect["Back"]["1"]["0"];
		backZero = RaceSelect["Back"]["2"]["0"];
		back_ani = RaceSelect["BackAni"];
		class_details_background = RaceSelect["Back1"]["0"]["0"];
		class_details_backgroundZero = RaceSelect["Back1"]["1"]["0"];

		Back2 = RaceSelect["Back2"];
		dot = Back2["dot"]["dot"];

		uint16_t node_index = 0;

		for (nl::node node : button)
		{
			std::string name = node.name();

			if (name == "intervalX" || name == "pos" || name == std::to_string(Classes::PINKBEAN) || name == std::to_string(Classes::YETI))
				continue;

			class_map[node_index++] = std::stoi(name);
		}

		std::sort(class_map.begin(), class_map.begin() + class_count);

		for (uint16_t i = 0; i < class_count; i++)
		{
			uint16_t corrected_index = class_map[i];

			class_normal[i][false] = button[corrected_index]["normal"]["0"];
			class_normal[i][true] = button[corrected_index]["mouseOver"]["0"];

			class_disabled[i][false] = buttonDisabled[corrected_index]["normal"]["0"];
			class_disabled[i][true] = buttonDisabled[corrected_index]["mouseOver"]["0"];

			if (i == 9)
				class_background[i] = RaceSelect["Back0"][corrected_index]["99"];
			else
				class_background[i] = RaceSelect["Back0"][corrected_index]["0"];

			class_details[i] = Back2[corrected_index]["0"];
			class_title[i] = RaceSelect["Back3"][corrected_index]["0"];
		}

		buttons[Buttons::BtStart] = std::make_unique<MapleButton>(Common["BtStart"], Point<int16_t>(0, 1));
		buttons[Buttons::BtPreview] = std::make_unique<MapleButton>(Common["BtPreview"]);
		buttons[Buttons::BtMake] = std::make_unique<MapleButton>(RaceSelect["make"]);
		buttons[Buttons::BtLeftArrow] = std::make_unique<MapleButton>(RaceSelect["leftArrow"]);
		buttons[Buttons::BtRightArrow] = std::make_unique<MapleButton>(RaceSelect["rightArrow"]);

		for (size_t i = 0; i <= Buttons::CLASS0; i++)
			buttons[Buttons::CLASS0 + i] = std::make_unique<AreaButton>(get_class_pos(i), class_normal[0][true].get_dimensions());

		index_shift = 0;
		selected_index = 0;
		selected_class = class_index[selected_index];

		buttons[Buttons::BtLeftArrow]->set_state(Button::State::DISABLED);

		Sound(Sound::Name::RACESELECT).play();
	}

	void UIRaceSelect::draw(float inter) const
	{
		uint16_t corrected_index = get_corrected_class_index(selected_class);

		if (selected_class == Classes::ZERO)
			backZero.draw(position);
		else
			back.draw(position);

		UIElement::draw_sprites(inter);

		version.draw(position + version_pos - Point<int16_t>(0, 5));

		class_background[corrected_index].draw(position);

		if (selected_class == Classes::ZERO)
			class_details_backgroundZero.draw(position);
		else
			class_details_background.draw(position);

		class_details[corrected_index].draw(position);
		class_title[corrected_index].draw(position);

		for (nl::node node : hotlist)
		{
			if (node.get_integer() == selected_class)
			{
				if (selected_class == Classes::ZERO)
					hotlabelZero.draw(position, inter);
				else
					hotlabel.draw(position, inter);

				break;
			}
		}

		for (nl::node node : newlist)
		{
			if (node.get_integer() == selected_class)
			{
				newlabel.draw(position - Point<int16_t>(1, 0), inter);
				break;
			}
		}

		for (uint16_t i = 0; i < INDEX_COUNT; i++)
		{
			uint16_t cur_index = class_index[i];
			uint16_t cur_class = get_corrected_class_index(cur_index);

			bool selected = mouseover[i];
			bool disabled = class_isdisabled[cur_class];
			auto& found_class = disabled ? class_disabled : class_normal;
			Point<int16_t> button_pos = get_class_pos(i);
			Point<int16_t> pos_adj = (disabled && !selected) ? Point<int16_t>(0, 2) : Point<int16_t>(0, 0);
			found_class[cur_class][selected].draw(position + button_pos + pos_adj);

			for (nl::node node : hotlist)
			{
				if (node.get_integer() == cur_index)
				{
					hotbtn.draw(position + button_pos + hotbtnPos, inter);
					break;
				}
			}

			for (nl::node node : newlist)
			{
				if (node.get_integer() == cur_index)
				{
					newbtn.draw(position + button_pos + newbtnPos, inter);
					break;
				}
			}
		}

		nl::node dot_pos = Back2[corrected_index]["dot"];

		if (dot_pos)
		{
			dot.draw(DrawArgument(dot_pos["pos_ctrl"][dot_pos["ctrl"]]));
			dot.draw(DrawArgument(dot_pos["pos_def"][dot_pos["def"]]));
			dot.draw(DrawArgument(dot_pos["pos_pow"][dot_pos["pow"]]));
			dot.draw(DrawArgument(dot_pos["pos_spd"][dot_pos["spd"]]));
		}

		UIElement::draw_buttons(inter);

		back_ani.draw(position, inter);
	}

	void UIRaceSelect::update()
	{
		UIElement::update();

		hotlabel.update();
		hotlabelZero.update();
		newlabel.update();
		hotbtn.update();
		newbtn.update();

		if (selected_class == Classes::ZERO)
			buttons[Buttons::BtMake]->set_position(position + posZero);
		else
			buttons[Buttons::BtMake]->set_position(position + pos);

		back_ani.update();
	}

	Cursor::State UIRaceSelect::send_cursor(bool clicked, Point<int16_t> cursorpos)
	{
		for (auto& btit : buttons)
		{
			if (btit.second->is_active() && btit.second->bounds(position).contains(cursorpos))
			{
				if (btit.second->get_state() == Button::State::NORMAL)
				{
					Sound(Sound::Name::BUTTONOVER).play();

					if (btit.first >= Buttons::CLASS0)
						mouseover[btit.first - Buttons::CLASS0] = true;

					btit.second->set_state(Button::State::MOUSEOVER);
				}
				else if (btit.second->get_state() == Button::State::MOUSEOVER)
				{
					if (clicked)
					{
						Sound(Sound::Name::BUTTONCLICK).play();

						btit.second->set_state(button_pressed(btit.first));
					}
					else
					{
						if (btit.first >= Buttons::CLASS0)
							mouseover[btit.first - Buttons::CLASS0] = true;
					}
				}
			}
			else if (btit.second->get_state() == Button::State::MOUSEOVER)
			{
				if (btit.first >= Buttons::CLASS0)
					mouseover[btit.first - Buttons::CLASS0] = false;

				btit.second->set_state(Button::State::NORMAL);
			}
		}

		return Cursor::State::LEAF;
	}

	void UIRaceSelect::send_key(int32_t keycode, bool pressed, bool escape)
	{
		if (pressed)
		{
			if (escape)
			{
				show_charselect();
			}
			else if (keycode == KeyAction::Id::LEFT || keycode == KeyAction::Id::DOWN)
			{
				if (buttons[Buttons::BtLeftArrow]->get_state() == Button::State::NORMAL)
					button_pressed(Buttons::BtLeftArrow);
			}
			else if (keycode == KeyAction::Id::RIGHT || keycode == KeyAction::Id::UP)
			{
				if (buttons[Buttons::BtRightArrow]->get_state() == Button::State::NORMAL)
					button_pressed(Buttons::BtRightArrow);
			}
			else if (keycode == KeyAction::Id::RETURN)
			{
				button_pressed(Buttons::BtMake);
			}
		}
	}

	UIElement::Type UIRaceSelect::get_type() const
	{
		return TYPE;
	}

	bool UIRaceSelect::check_name(std::string name) const
	{
		nl::node ForbiddenName = nl::nx::Etc["ForbiddenName.img"];

		for (std::string forbiddenName : ForbiddenName)
		{
			std::string lName = to_lower(name);
			std::string fName = to_lower(forbiddenName);

			if (lName.find(fName) != std::string::npos)
				return false;
		}

		return true;
	}

	void UIRaceSelect::send_naming_result(bool nameused)
	{
		if (selected_class == Classes::EXPLORERS)
		{
			if (auto explorercreation = UI::get().get_element<UIExplorerCreation>())
				explorercreation->send_naming_result(nameused);
		}
		else if (selected_class == Classes::CYGNUSKNIGHTS)
		{
			if (auto cygnuscreation = UI::get().get_element<UICygnusCreation>())
				cygnuscreation->send_naming_result(nameused);
		}
		else if (selected_class == Classes::ARAN)
		{
			if (auto arancreation = UI::get().get_element<UIAranCreation>())
				arancreation->send_naming_result(nameused);
		}
	}

	Button::State UIRaceSelect::button_pressed(uint16_t buttonid)
	{
		if (buttonid == Buttons::BtStart)
		{
			show_worldselect();

			return Button::State::NORMAL;
		}
		else if (buttonid == Buttons::BtPreview)
		{
			show_charselect();

			return Button::State::NORMAL;
		}
		else if (buttonid == Buttons::BtMake)
		{
			uint16_t corrected_index = get_corrected_class_index(selected_class);

			std::function<void()> okhandler = [&, corrected_index]()
			{
				if (!class_isdisabled[corrected_index])
				{
					Sound(Sound::Name::SCROLLUP).play();

					deactivate();

					if (selected_class == Classes::EXPLORERS)
						UI::get().emplace<UIExplorerCreation>();
					else if (selected_class == Classes::CYGNUSKNIGHTS)
						UI::get().emplace<UICygnusCreation>();
					else if (selected_class == Classes::ARAN)
						UI::get().emplace<UIAranCreation>();
				}
			};

			UI::get().emplace<UIClassConfirm>(selected_class, class_isdisabled[corrected_index], okhandler);

			return Button::State::NORMAL;
		}
		else if (buttonid == Buttons::BtLeftArrow)
		{
			uint16_t new_index = selected_index - 1;

			if (selected_index - index_shift == 0)
			{
				index_shift--;

				class_index[0] = order[new_index + 5 - Buttons::CLASS0];
				class_index[1] = order[new_index + 6 - Buttons::CLASS0];
				class_index[2] = order[new_index + 7 - Buttons::CLASS0];
				class_index[3] = order[new_index + 8 - Buttons::CLASS0];
				class_index[4] = order[new_index + 9 - Buttons::CLASS0];
				class_index[5] = order[new_index + 10 - Buttons::CLASS0];
			}

			select_class(new_index);

			return Button::State::IDENTITY;
		}
		else if (buttonid == Buttons::BtRightArrow)
		{
			uint16_t new_index = selected_index + 1;
			uint16_t selected = class_index[selected_index - index_shift];

			if (selected == class_index[5])
			{
				index_shift++;

				class_index[0] = order[new_index + 0 - Buttons::CLASS0];
				class_index[1] = order[new_index + 1 - Buttons::CLASS0];
				class_index[2] = order[new_index + 2 - Buttons::CLASS0];
				class_index[3] = order[new_index + 3 - Buttons::CLASS0];
				class_index[4] = order[new_index + 4 - Buttons::CLASS0];
				class_index[5] = order[new_index + 5 - Buttons::CLASS0];
			}

			select_class(new_index);

			return Button::State::IDENTITY;
		}
		else if (buttonid >= Buttons::CLASS0)
		{
			uint16_t index = buttonid - Buttons::CLASS0 + index_shift;

			select_class(index);

			return Button::State::IDENTITY;
		}
		else
		{
			return Button::State::DISABLED;
		}
	}

	void UIRaceSelect::select_class(uint16_t index)
	{
		uint16_t previous_index = selected_index;
		selected_index = index;

		if (previous_index != selected_index)
		{
			Sound(Sound::Name::RACESELECT).play();

			uint16_t previous = previous_index - index_shift;

			mouseover[previous] = false;
			buttons[previous + Buttons::CLASS0]->set_state(Button::State::NORMAL);

			uint16_t selected = selected_index - index_shift;

			selected_class = class_index[selected];
			mouseover[selected] = true;

			if (selected_class == Classes::KINESIS)
			{
				nl::node node = bgm[selected_class];
				std::string found_bgm = node["bgm"];
				std::size_t found_img = found_bgm.find(".img");

				if (found_img == std::string::npos)
				{
					std::size_t found_slash = found_bgm.find('/');

					if (found_slash != std::string::npos)
					{
						found_bgm.insert(found_slash, ".img");

						Music(found_bgm).play();
					}
				}
			}
			else if (class_index[previous] == Classes::KINESIS)
			{
				std::string LoginMusicNewtro = Configuration::get().get_login_music_newtro();

				Music(LoginMusicNewtro).play();
			}
		}
		else
		{
			button_pressed(Buttons::BtMake);
		}

		if (selected_index > 0)
			buttons[Buttons::BtLeftArrow]->set_state(Button::State::NORMAL);
		else
			buttons[Buttons::BtLeftArrow]->set_state(Button::State::DISABLED);

		if (selected_index < order.size() - 1)
			buttons[Buttons::BtRightArrow]->set_state(Button::State::NORMAL);
		else
			buttons[Buttons::BtRightArrow]->set_state(Button::State::DISABLED);
	}

	void UIRaceSelect::show_charselect()
	{
		Sound(Sound::Name::SCROLLUP).play();

		UI::get().remove(UIElement::Type::RACESELECT);

		if (auto charselect = UI::get().get_element<UICharSelect>())
			charselect->makeactive();
	}

	void UIRaceSelect::show_worldselect()
	{
		Sound(Sound::Name::SCROLLUP).play();

		UI::get().remove(UIElement::Type::RACESELECT);
		UI::get().remove(UIElement::Type::CHARSELECT);

		if (auto worldselect = UI::get().get_element<UIWorldSelect>())
			worldselect->makeactive();
	}

	Point<int16_t> UIRaceSelect::get_class_pos(size_t index) const
	{
		uint16_t x_adj = index * buttonIntervalX;

		return buttonPos + Point<int16_t>(x_adj, 0);
	}

	std::string UIRaceSelect::to_lower(std::string value) const
	{
		std::transform(value.begin(), value.end(), value.begin(), ::tolower);

		return value;
	}

	uint16_t UIRaceSelect::get_corrected_class_index(uint16_t index) const
	{
		for (uint16_t i = 0; i < class_count; i++)
			if (index == class_map[i])
				return i;

		LOG(LOG_DEBUG, "Failed to find corrected class index");

		return index;
	}
}