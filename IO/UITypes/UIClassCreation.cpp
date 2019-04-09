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
#pragma once
#include "UIClassCreation.h"
#include "UICharCreation.h"

#include "../Components/MapleButton.h"
#include "../UI.h"
#include "../UITypes/UILoginNotice.h"

#include "../../Data/ItemData.h"
#include "../../Net/Packets/CharCreationPackets.h"

#include "nlnx/nx.hpp"

namespace jrc
{
	UIClassCreation::UIClassCreation(uint16_t job) : cjob(job)
	{
		gender = false;
		charSet = false;
		named = false;

		version = { Text::Font::A11M, Text::Alignment::LEFT, Text::Color::DARKSTEEL, "Ver. 203.3" };

		nl::node login = nl::nx::ui["Login.img"];
		nl::node common = login["Common"];
		nl::node customizeChar = login["CustomizeChar"];
		nl::node bgsrc = nl::nx::map["Back"]["login.img"]["back"];
		nl::node keyConfig = nl::nx::ui["UIWindow2.img"]["KeyConfig"]["KeyType"];
		nl::node crsrc;

		sky = bgsrc["2"];
		cloud = bgsrc["27"];

		if (cjob == 2)
		{
			crsrc = login["NewCharAran"];

			nl::node customizeCharAran = customizeChar["2000"];
			nl::node board = customizeCharAran["board"];
			nl::node genderSelect = customizeCharAran["genderSelect"];

			sprites.emplace_back(bgsrc["33"], Point<int16_t>(256, 289)); // From v203.3
			sprites.emplace_back(bgsrc["34"], Point<int16_t>(587, 147)); // From v203.3
			sprites_gender_select.emplace_back(board["genderTop"], Point<int16_t>(491, 158));
			sprites_gender_select.emplace_back(board["boardMid"], Point<int16_t>(491, 210)); // TODO: Repeat y value down
			sprites_gender_select.emplace_back(board["boardBottom"], Point<int16_t>(491, 303));

			sprites_lookboard.emplace_back(crsrc["charSet"], Point<int16_t>(473, 93));

			std::map<uint8_t, uint8_t> charSettings = { {0,0}, {1,1}, {2,3}, {3,4}, {4,5}, {5,6}, {6,7} };

			for (int i = 0; i <= 6; i++)
				sprites_lookboard.emplace_back(crsrc["avatarSel"][charSettings[i]]["normal"], Point<int16_t>(504, 177 + (i * 18)));

			buttons[BT_CHARC_GENDER_M] = std::make_unique<MapleButton>(genderSelect["male"], Point<int16_t>(439, 96));
			buttons[BT_CHARC_GEMDER_F] = std::make_unique<MapleButton>(genderSelect["female"], Point<int16_t>(437, 96));

			buttons[BT_CHARC_SKINL] = std::make_unique<MapleButton>(crsrc["BtLeft"], Point<int16_t>(562, 177 + (2 * 18)));
			buttons[BT_CHARC_SKINR] = std::make_unique<MapleButton>(crsrc["BtRight"], Point<int16_t>(699, 177 + (2 * 18)));
			buttons[BT_CHARC_SKINL]->set_active(false);
			buttons[BT_CHARC_SKINR]->set_active(false);

			buttons[BT_CHARC_OK] = std::make_unique<MapleButton>(crsrc["BtYes"], Point<int16_t>(520, 387));
			buttons[BT_CHARC_CANCEL] = std::make_unique<MapleButton>(crsrc["BtNo"], Point<int16_t>(594, 387));
		}
		else if (cjob == 1)
		{
			crsrc = login["NewChar"];

			nl::node customizeCharExplorers = customizeChar["000"];
			nl::node board = customizeCharExplorers["board"];
			nl::node genderSelect = customizeCharExplorers["genderSelect"];

			sprites.emplace_back(bgsrc["14"], Point<int16_t>(250, 292)); // From v203.3
			sprites.emplace_back(board["genderTop"], Point<int16_t>(486, 85));
			sprites.emplace_back(board["boardMid"], Point<int16_t>(486, 199)); // TODO: Repeat y value down
			sprites.emplace_back(board["boardBottom"], Point<int16_t>(486, 319));

			buttons[BT_CHARC_GENDER_M] = std::make_unique<MapleButton>(genderSelect["male"], Point<int16_t>(487, 99));
			buttons[BT_CHARC_GEMDER_F] = std::make_unique<MapleButton>(genderSelect["female"], Point<int16_t>(485, 99));

			buttons[BT_CHARC_OK] = std::make_unique<MapleButton>(crsrc["BtYes"], Point<int16_t>(514, 384));
			buttons[BT_CHARC_CANCEL] = std::make_unique<MapleButton>(crsrc["BtNo"], Point<int16_t>(590, 384));
		}
		else if (cjob == 0)
		{
			crsrc = login["NewCharKnight"];

			nl::node customizeCharCygnus = customizeChar["1000"];
			nl::node board = customizeCharCygnus["board"];
			nl::node genderSelect = customizeCharCygnus["genderSelect"];

			sprites.emplace_back(bgsrc["46"], Point<int16_t>(400, 290)); // From v203.3
			sprites.emplace_back(board["genderTop"], Point<int16_t>(423, 94));
			sprites.emplace_back(board["boardMid"], Point<int16_t>(423, 212)); // TODO: Repeat y value down
			sprites.emplace_back(board["boardBottom"], Point<int16_t>(423, 338));

			buttons[BT_CHARC_GENDER_M] = std::make_unique<MapleButton>(genderSelect["male"], Point<int16_t>(425, 97));
			buttons[BT_CHARC_GEMDER_F] = std::make_unique<MapleButton>(genderSelect["female"], Point<int16_t>(423, 97));

			buttons[BT_CHARC_OK] = std::make_unique<MapleButton>(customizeCharCygnus["BtYes"], Point<int16_t>(510, 386));
			buttons[BT_CHARC_CANCEL] = std::make_unique<MapleButton>(customizeCharCygnus["BtNo"], Point<int16_t>(615, 386));
		}
		else
		{
			active = false;
			return;
		}

		sprites.emplace_back(common["frame"], Point<int16_t>(399, 289));
		sprites.emplace_back(common["step"]["3"], Point<int16_t>(40, -10));

		nameboard = crsrc["charName"];

		buttons[BT_BACK] = std::make_unique<MapleButton>(login["Common"]["BtStart"], Point<int16_t>(0, 505));

		buttons[BT_CHARC_FACEL] = std::make_unique<MapleButton>(crsrc["BtLeft"], Point<int16_t>(521, 216));
		buttons[BT_CHARC_FACER] = std::make_unique<MapleButton>(crsrc["BtRight"], Point<int16_t>(645, 216));
		buttons[BT_CHARC_HAIRL] = std::make_unique<MapleButton>(crsrc["BtLeft"], Point<int16_t>(521, 235));
		buttons[BT_CHARC_HAIRR] = std::make_unique<MapleButton>(crsrc["BtRight"], Point<int16_t>(645, 235));
		buttons[BT_CHARC_HAIRCL] = std::make_unique<MapleButton>(crsrc["BtLeft"], Point<int16_t>(521, 254));
		buttons[BT_CHARC_HAIRCR] = std::make_unique<MapleButton>(crsrc["BtRight"], Point<int16_t>(645, 254));
		buttons[BT_CHARC_TOPL] = std::make_unique<MapleButton>(crsrc["BtLeft"], Point<int16_t>(521, 292));
		buttons[BT_CHARC_TOPR] = std::make_unique<MapleButton>(crsrc["BtRight"], Point<int16_t>(645, 292));
		buttons[BT_CHARC_BOTL] = std::make_unique<MapleButton>(crsrc["BtLeft"], Point<int16_t>(521, 311));
		buttons[BT_CHARC_BOTR] = std::make_unique<MapleButton>(crsrc["BtRight"], Point<int16_t>(645, 311));
		buttons[BT_CHARC_SHOESL] = std::make_unique<MapleButton>(crsrc["BtLeft"], Point<int16_t>(521, 330));
		buttons[BT_CHARC_SHOESR] = std::make_unique<MapleButton>(crsrc["BtRight"], Point<int16_t>(645, 330));
		buttons[BT_CHARC_WEPL] = std::make_unique<MapleButton>(crsrc["BtLeft"], Point<int16_t>(521, 349));
		buttons[BT_CHARC_WEPR] = std::make_unique<MapleButton>(crsrc["BtRight"], Point<int16_t>(645, 348));

		sprites_keytype.emplace_back(keyConfig["backgrnd"], Point<int16_t>(181, 135));

		buttons[BT_KEY_TYPE_A] = std::make_unique<MapleButton>(keyConfig["btTypeA"], Point<int16_t>(181, 135));
		buttons[BT_KEY_TYPE_B] = std::make_unique<MapleButton>(keyConfig["btTypeB"], Point<int16_t>(182, 136));

		buttons[BT_CHARC_FACEL]->set_active(false);
		buttons[BT_CHARC_FACER]->set_active(false);
		buttons[BT_CHARC_HAIRL]->set_active(false);
		buttons[BT_CHARC_HAIRR]->set_active(false);
		buttons[BT_CHARC_HAIRCL]->set_active(false);
		buttons[BT_CHARC_HAIRCR]->set_active(false);
		buttons[BT_CHARC_TOPL]->set_active(false);
		buttons[BT_CHARC_TOPR]->set_active(false);
		buttons[BT_CHARC_BOTL]->set_active(false);
		buttons[BT_CHARC_BOTR]->set_active(false);
		buttons[BT_CHARC_SHOESL]->set_active(false);
		buttons[BT_CHARC_SHOESR]->set_active(false);
		buttons[BT_CHARC_WEPL]->set_active(false);
		buttons[BT_CHARC_WEPR]->set_active(false);

		buttons[BT_KEY_TYPE_A]->set_active(false);
		buttons[BT_KEY_TYPE_B]->set_active(false);

		namechar = { Text::A13M, Text::LEFT, Text::WHITE, { { 524, 186 }, { 630, 243 } }, 12 };
		namechar.set_state(Textfield::DISABLED);

		facename = { Text::A11M, Text::CENTER, Text::BLACK };
		hairname = { Text::A11M, Text::CENTER, Text::BLACK };
		haircname = { Text::A11M, Text::CENTER, Text::BLACK };
		bodyname = { Text::A11M, Text::CENTER, Text::BLACK };
		topname = { Text::A11M, Text::CENTER, Text::BLACK };
		botname = { Text::A11M, Text::CENTER, Text::BLACK };
		shoename = { Text::A11M, Text::CENTER, Text::BLACK };
		wepname = { Text::A11M, Text::CENTER, Text::BLACK };
		gendername = { Text::A11M, Text::CENTER, Text::BLACK };

		nl::node mkinfo = nl::nx::etc["MakeCharInfo.img"]["Info"];

		for (int32_t i = 0; i < 2; i++)
		{
			bool f;
			nl::node mk_n;

			if (i == 0)
			{
				f = true;
				mk_n = mkinfo["CharFemale"];
			}
			else
			{
				f = false;
				mk_n = mkinfo["CharMale"];
			}

			for (auto subnode : mk_n)
			{
				int num = stoi(subnode.name());

				for (auto idnode : subnode)
				{
					int value = idnode;

					switch (num)
					{
					case 0:
						faces[f].push_back(value);
						break;
					case 1:
						hairs[f].push_back(value);
						break;
					case 2:
						haircolors[f].push_back(static_cast<uint8_t>(value));
						break;
					case 3:
						skins[f].push_back(static_cast<uint8_t>(value));
						break;
					case 4:
						tops[f].push_back(value);
						break;
					case 5:
						bots[f].push_back(value);
						break;
					case 6:
						shoes[f].push_back(value);
						break;
					case 7:
						weapons[f].push_back(value);
						break;
					}
				}
			}
		}

		female = false;
		randomize_look();

		newchar.set_direction(true);

		position = { 0, 0 };
		dimension = { 800, 600 };
		active = true;
		cloudfx = 200.0f;
	}

	void UIClassCreation::draw(float alpha) const
	{
		for (int16_t i = 0; i < 2; i++)
		{
			for (int16_t k = 0; k < 800; k += sky.width())
			{
				sky.draw(Point<int16_t>(k, (400 * i) - 100));
			}
		}

		int16_t cloudx = static_cast<int16_t>(cloudfx) % 800;
		cloud.draw(Point<int16_t>(cloudx - cloud.width(), 300));
		cloud.draw(Point<int16_t>(cloudx, 300));
		cloud.draw(Point<int16_t>(cloudx + cloud.width(), 300));

		if (!gender)
		{
			for (auto& sprite : sprites_gender_select)
				sprite.draw(position, alpha);

			UIElement::draw(alpha);

			newchar.draw({ 394, 329 }, alpha);
		}
		else
		{
			if (!charSet)
			{
				UIElement::draw_sprites(alpha);

				for (auto& sprite : sprites_lookboard)
					sprite.draw(position, alpha);

				facename.draw(Point<int16_t>(643, 173 + (0 * 18)));
				hairname.draw(Point<int16_t>(643, 173 + (1 * 18)));
				//haircname.draw(Point<int16_t>(643, 173 + (2 * 18)));
				bodyname.draw(Point<int16_t>(643, 173 + (2 * 18)));
				topname.draw(Point<int16_t>(643, 173 + (3 * 18)));
				botname.draw(Point<int16_t>(643, 173 + (4 * 18)));
				shoename.draw(Point<int16_t>(643, 173 + (5 * 18)));
				wepname.draw(Point<int16_t>(643, 173 + (6 * 18)));
				//gendername.draw(Point<int16_t>(643, 173 + (8 * 18)));

				newchar.draw({ 394, 329 }, alpha);

				UIElement::draw_buttons(alpha);
			}
			else
			{
				if (!named)
				{
					UIElement::draw_sprites(alpha);

					nameboard.draw(Point<int16_t>(489, 96));
					namechar.draw(position);
					newchar.draw({ 394, 329 }, alpha);

					UIElement::draw_buttons(alpha);
				}
				else
				{
					UIElement::draw_sprites(alpha);

					nameboard.draw(Point<int16_t>(489, 96));

					UIElement::draw_buttons(alpha);

					for (auto& sprite : sprites_keytype)
						sprite.draw(position, alpha);

					buttons.at(BT_KEY_TYPE_A)->draw(position);
					buttons.at(BT_KEY_TYPE_B)->draw(position);
				}
			}
		}

		version.draw(position + Point<int16_t>(707, -9));
	}

	void UIClassCreation::update()
	{
		if (!gender)
		{
			for (auto& sprite : sprites_gender_select)
				sprite.update();

			newchar.update(Constants::TIMESTEP);
		}
		else
		{
			if (!charSet)
			{
				for (auto& sprite : sprites_lookboard)
					sprite.update();

				newchar.update(Constants::TIMESTEP);
			}
			else
			{
				if (!named)
				{
					namechar.update(position);
					newchar.update(Constants::TIMESTEP);
				}
				else
				{
					for (auto& sprite : sprites_keytype)
						sprite.update();

					namechar.set_state(Textfield::State::DISABLED);
				}
			}
		}

		UIElement::update();

		cloudfx += 0.25f;
	}

	Cursor::State UIClassCreation::send_cursor(bool clicked, Point<int16_t> cursorpos)
	{
		if (Cursor::State new_state = namechar.send_cursor(cursorpos, clicked))
			return new_state;

		return UIElement::send_cursor(clicked, cursorpos);
	}

	void UIClassCreation::send_naming_result(bool nameused)
	{
		if (!named)
		{
			if (!nameused)
			{
				named = true;
				namechar.set_state(Textfield::DISABLED);

				buttons[BT_KEY_TYPE_A]->set_active(true);
				buttons[BT_KEY_TYPE_B]->set_active(true);

				std::string cname = namechar.get_text();
				int32_t cface = faces[female][face];
				int32_t chair = hairs[female][hair];
				uint8_t chairc = haircolors[female][haircolor];
				uint8_t cskin = skins[female][skin];
				int32_t ctop = tops[female][top];
				int32_t cbot = bots[female][bot];
				int32_t cshoe = shoes[female][shoe];
				int32_t cwep = weapons[female][weapon];

				CreateCharPacket(cname, cjob, cface, chair, chairc, cskin, ctop, cbot, cshoe, cwep, female).dispatch();
			}

			buttons[BT_CHARC_OK]->set_state(Button::NORMAL);
		}
	}

	Button::State UIClassCreation::button_pressed(uint16_t id)
	{
		switch (id)
		{
		case BT_CHARC_OK:
			if (!gender)
			{
				gender = true;

				buttons[BT_CHARC_GENDER_M]->set_active(false);
				buttons[BT_CHARC_GEMDER_F]->set_active(false);

				buttons[BT_CHARC_SKINL]->set_active(true);
				buttons[BT_CHARC_SKINR]->set_active(true);

				buttons[BT_CHARC_OK]->set_position(Point<int16_t>(533, 358));
				buttons[BT_CHARC_CANCEL]->set_position(Point<int16_t>(607, 358));

				return Button::State::NORMAL;
			}
			else
			{
				if (!charSet)
				{
					charSet = true;

					buttons[BT_CHARC_SKINL]->set_active(false);
					buttons[BT_CHARC_SKINR]->set_active(false);

					buttons[BT_CHARC_OK]->set_position(Point<int16_t>(523, 233));
					buttons[BT_CHARC_CANCEL]->set_position(Point<int16_t>(597, 233));

					namechar.set_state(Textfield::State::FOCUSED);

					return Button::State::NORMAL;
				}
				else
				{
					if (!named)
					{
						std::string name = namechar.get_text();

						if (name.size() <= 0)
						{
							return Button::NORMAL;
						}
						else if (name.size() >= 4)
						{
							namechar.set_state(Textfield::NORMAL);

							UI::get().disable();
							UI::get().focus_textfield(nullptr);

							NameCharPacket(name).dispatch();

							return Button::PRESSED;
						}
						else
						{
							UI::get().emplace<UILoginNotice>(UILoginNotice::ILLEGAL_NAME);

							return Button::NORMAL;
						}
					}
					else
					{
						return Button::NORMAL;
					}
				}
			}
		case BT_BACK:
			back();
			return Button::State::PRESSED;
		case BT_CHARC_CANCEL:
			if (charSet)
			{
				charSet = false;

				buttons[BT_CHARC_SKINL]->set_active(true);
				buttons[BT_CHARC_SKINR]->set_active(true);

				buttons[BT_CHARC_OK]->set_position(Point<int16_t>(533, 358));
				buttons[BT_CHARC_CANCEL]->set_position(Point<int16_t>(607, 358));

				namechar.set_state(Textfield::State::DISABLED);

				return Button::State::NORMAL;
			}
			else
			{
				if (gender)
				{
					gender = false;

					buttons[BT_CHARC_GENDER_M]->set_active(true);
					buttons[BT_CHARC_GEMDER_F]->set_active(true);

					buttons[BT_CHARC_SKINL]->set_active(false);
					buttons[BT_CHARC_SKINR]->set_active(false);

					buttons[BT_CHARC_OK]->set_position(Point<int16_t>(520, 387));
					buttons[BT_CHARC_CANCEL]->set_position(Point<int16_t>(594, 387));

					return Button::State::NORMAL;
				}
				else
				{
					back();
					return Button::State::PRESSED;
				}
			}
		case BT_KEY_TYPE_A:
		case BT_KEY_TYPE_B:
			uint8_t type = (id == BT_KEY_TYPE_A) ? 0 : 1;

			UI::get().emplace<UIKeyConfirm>(type);

			return Button::State::PRESSED;
		}

		if (id >= BT_CHARC_FACEL && id <= BT_CHARC_GEMDER_F)
		{
			switch (id)
			{
			case BT_CHARC_FACEL:
				face = (face > 0) ? face - 1 : faces[female].size() - 1;
				newchar.set_face(faces[female][face]);
				facename.change_text(newchar.get_face()->get_name());
				break;
			case BT_CHARC_FACER:
				face = (face < faces[female].size() - 1) ? face + 1 : 0;
				newchar.set_face(faces[female][face]);
				facename.change_text(newchar.get_face()->get_name());
				break;
			case BT_CHARC_HAIRL:
				hair = (hair > 0) ? hair - 1 : hairs[female].size() - 1;
				newchar.set_hair(hairs[female][hair] + haircolors[female][haircolor]);
				hairname.change_text(newchar.get_hair()->get_name());
				break;
			case BT_CHARC_HAIRR:
				hair = (hair < hairs[female].size() - 1) ? hair + 1 : 0;
				newchar.set_hair(hairs[female][hair] + haircolors[female][haircolor]);
				hairname.change_text(newchar.get_hair()->get_name());
				break;
			case BT_CHARC_HAIRCL:
				haircolor = (haircolor > 0) ? haircolor - 1 : haircolors[female].size() - 1;
				newchar.set_hair(hairs[female][hair] + haircolors[female][haircolor]);
				haircname.change_text(newchar.get_hair()->getcolor());
				break;
			case BT_CHARC_HAIRCR:
				haircolor = (haircolor < haircolors[female].size() - 1) ? haircolor + 1 : 0;
				newchar.set_hair(hairs[female][hair] + haircolors[female][haircolor]);
				haircname.change_text(newchar.get_hair()->getcolor());
				break;
			case BT_CHARC_SKINL:
				skin = (skin > 0) ? skin - 1 : skins[female].size() - 1;
				newchar.set_body(skins[female][skin]);
				bodyname.change_text(newchar.get_body()->get_name());
				break;
			case BT_CHARC_SKINR:
				skin = (skin < skins[female].size() - 1) ? skin + 1 : 0;
				newchar.set_body(skins[female][skin]);
				bodyname.change_text(newchar.get_body()->get_name());
				break;
			case BT_CHARC_TOPL:
				top = (top > 0) ? top - 1 : tops[female].size() - 1;
				newchar.add_equip(tops[female][top]);
				topname.change_text(get_equipname(Equipslot::TOP));
				break;
			case BT_CHARC_TOPR:
				top = (top < tops[female].size() - 1) ? top + 1 : 0;
				newchar.add_equip(tops[female][top]);
				topname.change_text(get_equipname(Equipslot::TOP));
				break;
			case BT_CHARC_BOTL:
				bot = (bot > 0) ? bot - 1 : bots[female].size() - 1;
				newchar.add_equip(bots[female][bot]);
				botname.change_text(get_equipname(Equipslot::PANTS));
				break;
			case BT_CHARC_BOTR:
				bot = (bot < bots[female].size() - 1) ? bot + 1 : 0;
				newchar.add_equip(bots[female][bot]);
				botname.change_text(get_equipname(Equipslot::PANTS));
				break;
			case BT_CHARC_SHOESL:
				shoe = (shoe > 0) ? shoe - 1 : shoes[female].size() - 1;
				newchar.add_equip(shoes[female][shoe]);
				shoename.change_text(get_equipname(Equipslot::SHOES));
				break;
			case BT_CHARC_SHOESR:
				shoe = (shoe < shoes[female].size() - 1) ? shoe + 1 : 0;
				newchar.add_equip(shoes[female][shoe]);
				shoename.change_text(get_equipname(Equipslot::SHOES));
				break;
			case BT_CHARC_WEPL:
				weapon = (weapon > 0) ? weapon - 1 : weapons[female].size() - 1;
				newchar.add_equip(weapons[female][weapon]);
				wepname.change_text(get_equipname(Equipslot::WEAPON));
				break;
			case BT_CHARC_WEPR:
				weapon = (weapon < weapons[female].size() - 1) ? weapon + 1 : 0;
				newchar.add_equip(weapons[female][weapon]);
				wepname.change_text(get_equipname(Equipslot::WEAPON));
				break;
			case BT_CHARC_GENDER_M:
				if (female)
				{
					female = false;
					randomize_look();
				}

				break;
			case BT_CHARC_GEMDER_F:
				if (!female)
				{
					female = true;
					randomize_look();
				}

				break;
			}
			return Button::MOUSEOVER;
		}
		return Button::PRESSED;
	}

	void UIClassCreation::back()
	{
		active = false;

		if (auto charcreation = UI::get().get_element<UICharCreation>())
			charcreation->makeactive();
	}

	void UIClassCreation::randomize_look()
	{
		hair = randomizer.next_int(hairs[female].size());
		face = randomizer.next_int(faces[female].size());
		skin = randomizer.next_int(skins[female].size());
		haircolor = randomizer.next_int(haircolors[female].size());
		top = randomizer.next_int(tops[female].size());
		bot = randomizer.next_int(bots[female].size());
		shoe = randomizer.next_int(shoes[female].size());
		weapon = randomizer.next_int(weapons[female].size());

		newchar.set_body(skins[female][skin]);
		newchar.set_face(faces[female][face]);
		newchar.set_hair(hairs[female][hair] + haircolors[female][haircolor]);
		newchar.add_equip(tops[female][top]);
		newchar.add_equip(bots[female][bot]);
		newchar.add_equip(shoes[female][shoe]);
		newchar.add_equip(weapons[female][weapon]);

		bodyname.change_text(newchar.get_body()->get_name());
		facename.change_text(newchar.get_face()->get_name());
		hairname.change_text(newchar.get_hair()->get_name());
		haircname.change_text(newchar.get_hair()->getcolor());

		topname.change_text(get_equipname(Equipslot::TOP));
		botname.change_text(get_equipname(Equipslot::PANTS));
		shoename.change_text(get_equipname(Equipslot::SHOES));
		wepname.change_text(get_equipname(Equipslot::WEAPON));
		gendername.change_text(female ? "Female" : "Male");
	}

	const std::string& UIClassCreation::get_equipname(Equipslot::Id slot) const
	{
		if (int32_t item_id = newchar.get_equips().get_equip(slot))
		{
			return ItemData::get(item_id)
				.get_name();
		}
		else
		{
			static const std::string& nullstr = "Missing name.";
			return nullstr;
		}
	}
}