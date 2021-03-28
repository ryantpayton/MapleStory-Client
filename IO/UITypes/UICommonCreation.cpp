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
#include "UICommonCreation.h"

#include "UICharSelect.h"
#include "UILoginNotice.h"
#include "UIRaceSelect.h"

#include "../UI.h"

#include "../Components/MapleButton.h"

#include "../../Configuration.h"

#include "../../Audio/Audio.h"
#include "../../Data/ItemData.h"

#include "../../Net/Packets/CharCreationPackets.h"

#ifdef USE_NX
#include <nlnx/nx.hpp>
#endif

namespace ms
{
	UICommonCreation::UICommonCreation(const std::string& c) : UIElement(Point<int16_t>(0, 0), Point<int16_t>(800, 600)), classType(c), genderSelected(false), charLookSelected(false), charNameSelected(false), newCharNameDisabled(true), female(false)
	{
		load_char_look();

		std::string version_text = Configuration::get().get_version();
		version = Text(Text::Font::A11B, Text::Alignment::LEFT, Color::Name::LEMONGRASS, "Ver. " + version_text);

		nl::node Login = nl::nx::ui["Login.img"];
		nl::node Common = Login["Common"];
		nl::node createLimit = Login["NewChar"]["createLimit"];
		nl::node CustomizeChar = Login["CustomizeChar"][classType];
		nl::node board = CustomizeChar["board"];
		nl::node genderSelect = CustomizeChar["genderSelect"];

		nl::node frame = nl::nx::mapLatest["Obj"]["login.img"]["Common"]["frame"]["2"]["0"];

		sprites.emplace_back(createLimit["backgrnd"]);
		sprites_gender_select.emplace_back(board["genderTop"], getGenderTopPos());
		sprites_gender_select.emplace_back(board["boardMid"], getBoardMidPos());
		sprites_gender_select.emplace_back(board["boardBottom"], getBoardBottomPos());

		nl::node avatarSel = CustomizeChar["avatarSel"];

		for (size_t i = 0; i <= avatarSel.size(); i++)
			sprites_char_look.emplace_back(avatarSel[i]["normal"], getAvatarSelPos(i));

		sprites.emplace_back(frame, Point<int16_t>(400, 300));
		sprites.emplace_back(Common["frame"], Point<int16_t>(400, 300));
		sprites.emplace_back(Common["step"]["3"], Point<int16_t>(40, 0));

		nameboard = CustomizeChar["charName"];
		createLimitPos = Texture(createLimit["backgrnd"]).get_origin();
		createLimitNum = Charset(createLimit["num"], Charset::Alignment::RIGHT);

		buttons[Buttons::BtStart] = std::make_unique<MapleButton>(Login["Common"]["BtStart"], Point<int16_t>(0, 515));
		buttons[Buttons::BtGenderMale] = std::make_unique<MapleButton>(genderSelect["male"], getGenderSelectMalePos());
		buttons[Buttons::BtGenderFemale] = std::make_unique<MapleButton>(genderSelect["female"], getGenderSelectFemalePos());

		Point<int16_t> charLookLeft = getCharLookLeftPos();
		Point<int16_t> charLookRight = getCharLookRightPos();

		buttons[Buttons::BtFaceLeft] = std::make_unique<MapleButton>(CustomizeChar["BtLeft"], charLookLeft);
		buttons[Buttons::BtFaceRight] = std::make_unique<MapleButton>(CustomizeChar["BtRight"], charLookRight);

		charLookLeft.shift_y(18);
		charLookRight.shift_y(18);

		buttons[Buttons::BtHairLeft] = std::make_unique<MapleButton>(CustomizeChar["BtLeft"], charLookLeft);
		buttons[Buttons::BtHairRight] = std::make_unique<MapleButton>(CustomizeChar["BtRight"], charLookRight);

		charLookLeft.shift_y(18);
		charLookRight.shift_y(18);

		if (hairColorEnabled)
		{
			charLookLeft.shift_y(18);
			charLookRight.shift_y(18);

			if (classType == "1000")
			{
				charLookLeft.shift_y(18);
				charLookRight.shift_y(18);
			}
		}

		buttons[Buttons::BtSkinLeft] = std::make_unique<MapleButton>(CustomizeChar["BtLeft"], charLookLeft);
		buttons[Buttons::BtSkinRight] = std::make_unique<MapleButton>(CustomizeChar["BtRight"], charLookRight);

		charLookLeft.shift_y(18);
		charLookRight.shift_y(18);

		buttons[Buttons::BtTopLeft] = std::make_unique<MapleButton>(CustomizeChar["BtLeft"], charLookLeft);
		buttons[Buttons::BtTopRight] = std::make_unique<MapleButton>(CustomizeChar["BtRight"], charLookRight);

		charLookLeft.shift_y(18);
		charLookRight.shift_y(18);

		if (bottomEnabled)
		{
			buttons[Buttons::BtBottomLeft] = std::make_unique<MapleButton>(CustomizeChar["BtLeft"], charLookLeft);
			buttons[Buttons::BtBottomRight] = std::make_unique<MapleButton>(CustomizeChar["BtRight"], charLookRight);

			charLookLeft.shift_y(18);
			charLookRight.shift_y(18);
		}

		if (capeEnabled)
		{
			buttons[Buttons::BtCapeLeft] = std::make_unique<MapleButton>(CustomizeChar["BtLeft"], charLookLeft);
			buttons[Buttons::BtCapeRight] = std::make_unique<MapleButton>(CustomizeChar["BtRight"], charLookRight);

			charLookLeft.shift_y(18);
			charLookRight.shift_y(18);
		}

		buttons[Buttons::BtShoesLeft] = std::make_unique<MapleButton>(CustomizeChar["BtLeft"], charLookLeft);
		buttons[Buttons::BtShoesRight] = std::make_unique<MapleButton>(CustomizeChar["BtRight"], charLookRight);

		charLookLeft.shift_y(18);
		charLookRight.shift_y(18);

		buttons[Buttons::BtWeaponLeft] = std::make_unique<MapleButton>(CustomizeChar["BtLeft"], charLookLeft);
		buttons[Buttons::BtWeaponRight] = std::make_unique<MapleButton>(CustomizeChar["BtRight"], charLookRight);

		buttons[Buttons::BtYes] = std::make_unique<MapleButton>(CustomizeChar["BtYes"], getBtYesPos(0));
		buttons[Buttons::BtNo] = std::make_unique<MapleButton>(CustomizeChar["BtNo"], getBtNoPos(0));

		nl::node hairSelect = CustomizeChar["hairSelect"];
		hairSelectSize = hairSelect.size();

		for (size_t i = 0; i < hairSelectSize; i++)
			buttons[Buttons::BtHairSelect0 + i] = std::make_unique<MapleButton>(hairSelect[i], getHairSelectPos(i));

		disable_char_look();

		newCharName = Textfield(Text::Font::A12M, Text::Alignment::LEFT, getNewCharNameColor(), getNewCharNameRect(), 12);

		newCharName.set_enter_callback(
			[&](std::string)
			{
				button_pressed(Buttons::BtYes);
			}
		);

		newCharName.set_key_callback(
			KeyAction::Id::ESCAPE,
			[&]()
			{
				button_pressed(Buttons::BtNo);
			}
		);

		faceName = Text(Text::Font::A11M, Text::Alignment::CENTER, Color::Name::BLACK);
		hairStyleName = Text(Text::Font::A11M, Text::Alignment::CENTER, Color::Name::BLACK);
		skinColorName = Text(Text::Font::A11M, Text::Alignment::CENTER, Color::Name::BLACK);
		topName = Text(Text::Font::A11M, Text::Alignment::CENTER, Color::Name::BLACK);
		bottomName = Text(Text::Font::A11M, Text::Alignment::CENTER, Color::Name::BLACK);
		capeName = Text(Text::Font::A11M, Text::Alignment::CENTER, Color::Name::BLACK);
		shoeName = Text(Text::Font::A11M, Text::Alignment::CENTER, Color::Name::BLACK);
		weaponName = Text(Text::Font::A11M, Text::Alignment::CENTER, Color::Name::BLACK);

		randomize_look();

		newCharLook.set_direction(true);
	}

	void UICommonCreation::draw(float inter) const
	{
		for (const Sprite& sprite : sprites_background)
			sprite.draw(position, inter);

		if (!genderSelected)
		{
			for (size_t i = 0; i < sprites_gender_select.size(); i++)
			{
				if (i == 1)
				{
					for (size_t f = 0; f <= getVerticalCount(); f++)
						sprites_gender_select[i].draw(position + Point<int16_t>(0, f * getVerticalAdj()), inter);
				}
				else
				{
					sprites_gender_select[i].draw(position, inter);
				}
			}

			UIElement::draw(inter);

			newCharLook.draw(Point<int16_t>(394, 339), inter);
		}
		else
		{
			if (!charLookSelected)
			{
				UIElement::draw_sprites(inter);

				if (classType == "1000")
				{
					for (size_t i = 0; i < sprites_lookboard.size(); i++)
					{
						if (i == 1)
						{
							for (size_t f = 0; f <= getVerticalCount() + 1; f++)
								sprites_lookboard[i].draw(position + Point<int16_t>(0, f * getVerticalAdj()), inter);
						}
						else
						{
							sprites_lookboard[i].draw(position, inter);
						}
					}
				}
				else
				{
					for (auto& sprite : sprites_lookboard)
						sprite.draw(position, inter);
				}

				for (auto& sprite : sprites_char_look)
					sprite.draw(position, inter);

				size_t name_index = 0;

				faceName.draw(getNamePos(name_index));

				name_index++;

				hairStyleName.draw(getNamePos(name_index));

				name_index++;

				if (hairColorEnabled)
					name_index++;

				skinColorName.draw(getNamePos(name_index));

				name_index++;

				topName.draw(getNamePos(name_index));

				name_index++;

				if (bottomEnabled)
				{
					bottomName.draw(getNamePos(name_index));

					name_index++;
				}

				if (capeEnabled)
				{
					capeName.draw(getNamePos(name_index));

					name_index++;
				}

				shoeName.draw(getNamePos(name_index));

				name_index++;

				weaponName.draw(getNamePos(name_index));

				newCharLook.draw(Point<int16_t>(394, 339), inter);

				UIElement::draw_buttons(inter);
			}
			else
			{
				if (!charNameSelected)
				{
					UIElement::draw_sprites(inter);

					nameboard.draw(getNameboardPos());

					newCharName.draw(position + getTextfieldPos(), Point<int16_t>(1, -1));
					newCharLook.draw(Point<int16_t>(394, 339), inter);

					UIElement::draw_buttons(inter);
				}
				else
				{
					UIElement::draw_sprites(inter);

					nameboard.draw(getNameboardPos());

					UIElement::draw_buttons(inter);
				}
			}
		}

		// TOOD: Get number of character able to create
		createLimitNum.draw("10", 8, position - createLimitPos + Point<int16_t>(14, 0));

		version.draw(position + Point<int16_t>(707, 4));
	}

	void UICommonCreation::update()
	{
		if (!genderSelected)
		{
			for (Sprite& sprite : sprites_gender_select)
				sprite.update();
		}
		else
		{
			if (!charLookSelected)
			{
				for (Sprite& sprite : sprites_lookboard)
					sprite.update();

				for (Sprite& sprite : sprites_char_look)
					sprite.update();
			}
			else
			{
				if (!charNameSelected)
				{
					newCharName.update();

					if (!newCharNameDisabled)
						newCharName.set_state(Textfield::State::FOCUSED);
					else
						newCharName.set_state(Textfield::State::DISABLED);
				}
				else
				{
					newCharName.set_state(Textfield::State::DISABLED);
				}
			}
		}

		UIElement::update();
	}

	void UICommonCreation::send_key(int32_t keycode, bool pressed, bool escape)
	{
		if (pressed)
		{
			if (escape)
				button_pressed(Buttons::BtNo);
			else if (keycode == KeyAction::Id::RETURN)
				button_pressed(Buttons::BtYes);
		}
	}

	UIElement::Type UICommonCreation::get_type() const
	{
		return TYPE;
	}

	void UICommonCreation::send_naming_result(bool nameused)
	{
		if (!charNameSelected)
		{
			if (!nameused)
			{
				charNameSelected = true;

				std::string name = newCharName.get_text();
				int32_t job = -1;
				int64_t face = faces[female][faceIndex];

				int64_t hair = 0;
				int64_t hairStyle = hairStyles[female][hairStyleIndex];
				auto hairColors_ = hairColors[female][hairStyle];

				if (hairColors_.size() > 0)
					hair = hairColors_[hairColorIndex];
				else
					hair = hairStyle;

				int64_t skin = skinColors[female][skinColorIndex];
				int64_t top = tops[female][topIndex];

				int64_t bottom = 0;

				if (bottoms[female].size() > 0)
					bottom = bottoms[female][bottomIndex];

				// TODO: Need to do something with cape
				int64_t cape = 0;

				if (capes[female].size() > 0)
					cape = capes[female][capeIndex];

				int64_t shoe = shoes[female][shoeIndex];
				int64_t weapon = weapons[female][weaponIndex];

				if (classType == "000")
					job = 1;
				else if (classType == "2000")
					job = 2;
				else if (classType == "1000")
					job = 0;

				if (job >= 0)
				{
					CreateCharPacket(name, job, face, hair, skin, top, bottom, shoe, weapon, female).dispatch();

					auto onok = [&](bool alternate)
					{
						Sound(Sound::Name::SCROLLUP).play();

						UI::get().remove(UIElement::Type::LOGINNOTICE_CONFIRM);
						UI::get().remove(UIElement::Type::LOGINNOTICE);
						UI::get().remove(UIElement::Type::CLASSCREATION);
						UI::get().remove(UIElement::Type::RACESELECT);

						if (auto charselect = UI::get().get_element<UICharSelect>())
							charselect->post_add_character();
					};

					UI::get().emplace<UIKeySelect>(onok, true);
				}
			}
			else
			{
				auto onok = [&]()
				{
					newCharNameDisabled = false;

					buttons[Buttons::BtYes]->set_state(Button::State::NORMAL);
					buttons[Buttons::BtNo]->set_state(Button::State::NORMAL);
				};

				UI::get().emplace<UILoginNotice>(UILoginNotice::Message::NAME_IN_USE, onok);
			}
		}
	}

	Button::State UICommonCreation::button_pressed(uint16_t buttonid)
	{
		if (buttonid == Buttons::BtStart)
		{
			Sound(Sound::Name::SCROLLUP).play();

			UI::get().remove(UIElement::Type::CLASSCREATION);
			UI::get().emplace<UIRaceSelect>();

			return Button::State::NORMAL;
		}
		else if (buttonid == Buttons::BtYes)
		{
			if (!genderSelected)
			{
				genderSelected = true;

				buttons[Buttons::BtGenderMale]->set_active(false);
				buttons[Buttons::BtGenderFemale]->set_active(false);

				enable_char_look();

				return Button::State::NORMAL;
			}
			else
			{
				if (!charLookSelected)
				{
					charLookSelected = true;

					disable_char_look();

					buttons[Buttons::BtYes]->set_position(getBtYesPos(2));
					buttons[Buttons::BtNo]->set_position(getBtNoPos(2));

					newCharNameDisabled = false;

					return Button::State::NORMAL;
				}
				else
				{
					if (!charNameSelected)
					{
						std::string name = newCharName.get_text();

						if (name.size() <= 0)
						{
							return Button::State::NORMAL;
						}
						else if (name.size() >= 4)
						{
							newCharNameDisabled = true;

							buttons[Buttons::BtYes]->set_state(Button::State::DISABLED);
							buttons[Buttons::BtNo]->set_state(Button::State::DISABLED);

							if (auto raceselect = UI::get().get_element<UIRaceSelect>())
							{
								if (raceselect->check_name(name))
								{
									NameCharPacket(name).dispatch();

									return Button::State::IDENTITY;
								}
							}

							auto okhandler = [&]()
							{
								newCharNameDisabled = false;

								buttons[Buttons::BtYes]->set_state(Button::State::NORMAL);
								buttons[Buttons::BtNo]->set_state(Button::State::NORMAL);
							};

							UI::get().emplace<UILoginNotice>(UILoginNotice::Message::ILLEGAL_NAME, okhandler);

							return Button::State::NORMAL;
						}
						else
						{
							newCharNameDisabled = true;

							buttons[Buttons::BtYes]->set_state(Button::State::DISABLED);
							buttons[Buttons::BtNo]->set_state(Button::State::DISABLED);

							auto okhandler = [&]()
							{
								newCharNameDisabled = false;

								buttons[Buttons::BtYes]->set_state(Button::State::NORMAL);
								buttons[Buttons::BtNo]->set_state(Button::State::NORMAL);
							};

							UI::get().emplace<UILoginNotice>(UILoginNotice::Message::ILLEGAL_NAME, okhandler);

							return Button::State::IDENTITY;
						}
					}
					else
					{
						return Button::State::NORMAL;
					}
				}
			}
		}
		else if (buttonid == Buttons::BtNo)
		{
			if (charLookSelected)
			{
				charLookSelected = false;

				enable_char_look();

				newCharNameDisabled = true;

				return Button::State::NORMAL;
			}
			else
			{
				if (genderSelected && choosableGender)
				{
					genderSelected = false;

					buttons[Buttons::BtGenderMale]->set_active(true);
					buttons[Buttons::BtGenderFemale]->set_active(true);

					disable_char_look();

					buttons[Buttons::BtYes]->set_position(getBtYesPos(0));
					buttons[Buttons::BtNo]->set_position(getBtNoPos(0));

					return Button::State::NORMAL;
				}
				else
				{
					button_pressed(Buttons::BtStart);

					return Button::State::NORMAL;
				}
			}
		}
		else if (buttonid == Buttons::BtFaceLeft)
		{
			faceIndex = (faceIndex > 0) ? faceIndex - 1 : faces[female].size() - 1;

			newCharLook.set_face(faces[female][faceIndex]);
			faceName.change_text(newCharLook.get_face()->get_name());

			check_names();

			return Button::State::NORMAL;
		}
		else if (buttonid == Buttons::BtFaceRight)
		{
			faceIndex = (faceIndex < faces[female].size() - 1) ? faceIndex + 1 : 0;

			newCharLook.set_face(faces[female][faceIndex]);
			faceName.change_text(newCharLook.get_face()->get_name());

			check_names();

			return Button::State::NORMAL;
		}
		else if (buttonid == Buttons::BtHairLeft)
		{
			hairStyleIndex = (hairStyleIndex > 0) ? hairStyleIndex - 1 : hairStyles[female].size() - 1;

			int64_t hairStyle = hairStyles[female][hairStyleIndex];
			auto hairColors_ = hairColors[female][hairStyle];

			newCharLook.set_hair(hairColors_[hairColorIndex]);
			hairStyleName.change_text(newCharLook.get_hair()->get_name());

			check_names();

			return Button::State::NORMAL;
		}
		else if (buttonid == Buttons::BtHairRight)
		{
			hairStyleIndex = (hairStyleIndex < hairStyles[female].size() - 1) ? hairStyleIndex + 1 : 0;

			int64_t hairStyle = hairStyles[female][hairStyleIndex];
			auto hairColors_ = hairColors[female][hairStyle];

			newCharLook.set_hair(hairColors_[hairColorIndex]);
			hairStyleName.change_text(newCharLook.get_hair()->get_name());

			check_names();

			return Button::State::NORMAL;
		}
		else if (buttonid == Buttons::BtSkinLeft)
		{
			skinColorIndex = (skinColorIndex > 0) ? skinColorIndex - 1 : skinColors[female].size() - 1;

			newCharLook.set_body(skinColors[female][skinColorIndex]);
			skinColorName.change_text(newCharLook.get_body()->get_name());

			check_names();

			return Button::State::NORMAL;
		}
		else if (buttonid == Buttons::BtSkinRight)
		{
			skinColorIndex = (skinColorIndex < skinColors[female].size() - 1) ? skinColorIndex + 1 : 0;

			newCharLook.set_body(skinColors[female][skinColorIndex]);
			skinColorName.change_text(newCharLook.get_body()->get_name());

			check_names();

			return Button::State::NORMAL;
		}
		else if (buttonid == Buttons::BtTopLeft)
		{
			topIndex = (topIndex > 0) ? topIndex - 1 : tops[female].size() - 1;

			newCharLook.add_equip(tops[female][topIndex]);
			topName.change_text(get_equipname(EquipSlot::Id::TOP));

			check_names();

			return Button::State::NORMAL;
		}
		else if (buttonid == Buttons::BtTopRight)
		{
			topIndex = (topIndex < tops[female].size() - 1) ? topIndex + 1 : 0;

			newCharLook.add_equip(tops[female][topIndex]);
			topName.change_text(get_equipname(EquipSlot::Id::TOP));

			check_names();

			return Button::State::NORMAL;
		}
		else if (buttonid == Buttons::BtBottomLeft)
		{
			bottomIndex = (bottomIndex > 0) ? bottomIndex - 1 : bottoms[female].size() - 1;

			newCharLook.add_equip(bottoms[female][bottomIndex]);
			bottomName.change_text(get_equipname(EquipSlot::Id::BOTTOM));

			check_names();

			return Button::State::NORMAL;
		}
		else if (buttonid == Buttons::BtBottomRight)
		{
			bottomIndex = (bottomIndex < bottoms[female].size() - 1) ? bottomIndex + 1 : 0;

			newCharLook.add_equip(bottoms[female][bottomIndex]);
			bottomName.change_text(get_equipname(EquipSlot::Id::BOTTOM));

			check_names();

			return Button::State::NORMAL;
		}
		else if (buttonid == Buttons::BtCapeLeft)
		{
			capeIndex = (capeIndex > 0) ? capeIndex - 1 : capes[female].size() - 1;

			newCharLook.add_equip(capes[female][capeIndex]);
			capeName.change_text(get_equipname(EquipSlot::Id::CAPE));

			check_names();

			return Button::State::NORMAL;
		}
		else if (buttonid == Buttons::BtCapeRight)
		{
			capeIndex = (capeIndex < capes[female].size() - 1) ? capeIndex + 1 : 0;

			newCharLook.add_equip(capes[female][capeIndex]);
			capeName.change_text(get_equipname(EquipSlot::Id::CAPE));

			check_names();

			return Button::State::NORMAL;
		}
		else if (buttonid == Buttons::BtShoesLeft)
		{
			shoeIndex = (shoeIndex > 0) ? shoeIndex - 1 : shoes[female].size() - 1;

			newCharLook.add_equip(shoes[female][shoeIndex]);
			shoeName.change_text(get_equipname(EquipSlot::Id::SHOES));

			check_names();

			return Button::State::NORMAL;
		}
		else if (buttonid == Buttons::BtShoesRight)
		{
			shoeIndex = (shoeIndex < shoes[female].size() - 1) ? shoeIndex + 1 : 0;

			newCharLook.add_equip(shoes[female][shoeIndex]);
			shoeName.change_text(get_equipname(EquipSlot::Id::SHOES));

			check_names();

			return Button::State::NORMAL;
		}
		else if (buttonid == Buttons::BtWeaponLeft)
		{
			weaponIndex = (weaponIndex > 0) ? weaponIndex - 1 : weapons[female].size() - 1;

			newCharLook.add_equip(weapons[female][weaponIndex]);
			weaponName.change_text(get_equipname(EquipSlot::Id::WEAPON));

			check_names();

			return Button::State::NORMAL;
		}
		else if (buttonid == Buttons::BtWeaponRight)
		{
			weaponIndex = (weaponIndex < weapons[female].size() - 1) ? weaponIndex + 1 : 0;

			newCharLook.add_equip(weapons[female][weaponIndex]);
			weaponName.change_text(get_equipname(EquipSlot::Id::WEAPON));

			check_names();

			return Button::State::NORMAL;
		}
		else if (buttonid == Buttons::BtGenderMale)
		{
			if (female)
			{
				female = false;

				randomize_look();
			}

			return Button::State::NORMAL;
		}
		else if (buttonid == Buttons::BtGenderFemale)
		{
			if (!female)
			{
				female = true;

				randomize_look();
			}

			return Button::State::NORMAL;
		}
		else if (buttonid >= Buttons::BtHairSelect0 && buttonid < Buttons::LENGTH)
		{
			hairColorIndex = buttonid - Buttons::BtHairSelect0;

			int64_t hairStyle = hairStyles[female][hairStyleIndex];
			auto hairColors_ = hairColors[female][hairStyle];

			newCharLook.set_hair(hairColors_[hairColorIndex]);
			hairStyleName.change_text(newCharLook.get_hair()->get_name());

			check_names();

			return Button::State::NORMAL;
		}
		else
		{
			return Button::State::PRESSED;
		}
	}

	void UICommonCreation::randomize_look()
	{
		faceIndex = randomizer.next_int(faces[female].size());
		hairStyleIndex = randomizer.next_int(hairStyles[female].size());

		int64_t hairStyle = hairStyles[female][hairStyleIndex];
		auto hairColors_ = hairColors[female][hairStyle];
		hairColorIndex = randomizer.next_int(hairColors_.size());

		skinColorIndex = randomizer.next_int(skinColors[female].size());
		topIndex = randomizer.next_int(tops[female].size());
		bottomIndex = randomizer.next_int(bottoms[female].size());
		capeIndex = randomizer.next_int(capes[female].size());
		shoeIndex = randomizer.next_int(shoes[female].size());
		weaponIndex = randomizer.next_int(weapons[female].size());

		newCharLook.set_face(faces[female][faceIndex]);

		if (hairColors_.size() > 0)
			newCharLook.set_hair(hairColors_[hairColorIndex]);
		else
			newCharLook.set_hair(hairStyle);

		newCharLook.set_body(skinColors[female][skinColorIndex]);
		newCharLook.add_equip(tops[female][topIndex]);

		if (bottoms[female].size() > 0)
			newCharLook.add_equip(bottoms[female][bottomIndex]);

		if (capes[female].size() > 0)
			newCharLook.add_equip(capes[female][capeIndex]);

		newCharLook.add_equip(shoes[female][shoeIndex]);
		newCharLook.add_equip(weapons[female][weaponIndex]);

		faceName.change_text(newCharLook.get_face()->get_name());
		hairStyleName.change_text(newCharLook.get_hair()->get_name());
		skinColorName.change_text(newCharLook.get_body()->get_name());
		topName.change_text(get_equipname(EquipSlot::Id::TOP));
		bottomName.change_text(get_equipname(EquipSlot::Id::BOTTOM));
		capeName.change_text(get_equipname(EquipSlot::Id::CAPE));
		shoeName.change_text(get_equipname(EquipSlot::Id::SHOES));
		weaponName.change_text(get_equipname(EquipSlot::Id::WEAPON));

		check_names();
	}

	void UICommonCreation::load_char_look()
	{
		size_t color_count = 0, bottom_count = 0, cape_count = 0;

		nl::node MakeCharInfo = nl::nx::etc["MakeCharInfo.img"][classType];

		for (nl::node gender_node : MakeCharInfo)
		{
			std::string gender_node_name = gender_node.name();

			if (gender_node_name == "info")
			{
				choosableGender = gender_node["choosableGender"].get_bool();

				if (!choosableGender)
					button_pressed(Buttons::BtYes);

				continue;
			}

			bool f = gender_node_name == "female";

			for (size_t type = 0; type < gender_node.size(); type++)
			{
				for (nl::node id_node : gender_node[type])
				{
					std::string id_node_name = id_node.name();
					int64_t id_node_value = id_node.get_integer();

					if (id_node_name == "color")
						continue;

					switch (type)
					{
						case 0:
						{
							faces[f].push_back(id_node_value);
							break;
						}
						case 1:
						{
							hairStyles[f].push_back(id_node_value);

							for (nl::node color_node : gender_node[type]["color"][id_node_value])
							{
								hairColors[f][id_node_value].push_back(color_node);

								color_count++;
							}

							break;
						}
						case 2:
						{
							skinColors[f].push_back(id_node_value);
							break;
						}
						case 3:
						{
							tops[f].push_back(id_node_value);
							break;
						}
						default:
						{
							int64_t id_node_prefix = id_node_value / 10000;

							if (id_node_prefix == 106)
							{
								bottoms[f].push_back(id_node_value);

								bottom_count++;
								break;
							}
							else if (id_node_prefix == 107)
							{
								shoes[f].push_back(id_node_value);
								break;
							}
							else if (id_node_prefix == 110)
							{
								capes[f].push_back(id_node_value);

								cape_count++;
								break;
							}
							else
							{
								weapons[f].push_back(id_node_value);
								break;
							}
						}
					}
				}
			}
		}

		if (color_count > 0)
			hairColorEnabled = true;

		if (bottom_count > 0)
			bottomEnabled = true;

		if (cape_count > 0)
			capeEnabled = true;
	}

	void UICommonCreation::enable_char_look()
	{
		buttons[Buttons::BtYes]->set_position(getBtYesPos(1));
		buttons[Buttons::BtNo]->set_position(getBtNoPos(1));

		if (faces[female].size() > 1)
		{
			buttons[Buttons::BtFaceLeft]->set_active(true);
			buttons[Buttons::BtFaceRight]->set_active(true);
		}

		if (hairStyles[female].size() > 1)
		{
			buttons[Buttons::BtHairLeft]->set_active(true);
			buttons[Buttons::BtHairRight]->set_active(true);
		}

		if (skinColors[female].size() > 1)
		{
			buttons[Buttons::BtSkinLeft]->set_active(true);
			buttons[Buttons::BtSkinRight]->set_active(true);
		}

		if (tops[female].size() > 1)
		{
			buttons[Buttons::BtTopLeft]->set_active(true);
			buttons[Buttons::BtTopRight]->set_active(true);
		}

		if (bottomEnabled && bottoms[female].size() > 1)
		{
			buttons[Buttons::BtBottomLeft]->set_active(true);
			buttons[Buttons::BtBottomRight]->set_active(true);
		}

		if (capeEnabled && capes[female].size() > 1)
		{
			buttons[Buttons::BtCapeLeft]->set_active(true);
			buttons[Buttons::BtCapeRight]->set_active(true);
		}

		if (shoes[female].size() > 1)
		{
			buttons[Buttons::BtShoesLeft]->set_active(true);
			buttons[Buttons::BtShoesRight]->set_active(true);
		}

		if (weapons[female].size() > 1)
		{
			buttons[Buttons::BtWeaponLeft]->set_active(true);
			buttons[Buttons::BtWeaponRight]->set_active(true);
		}

		if (hairColorEnabled)
			for (size_t i = 0; i < hairSelectSize; i++)
				buttons[Buttons::BtHairSelect0 + i]->set_active(true);
	}

	void UICommonCreation::disable_char_look()
	{
		buttons[Buttons::BtFaceLeft]->set_active(false);
		buttons[Buttons::BtFaceRight]->set_active(false);
		buttons[Buttons::BtHairLeft]->set_active(false);
		buttons[Buttons::BtHairRight]->set_active(false);
		buttons[Buttons::BtSkinLeft]->set_active(false);
		buttons[Buttons::BtSkinRight]->set_active(false);
		buttons[Buttons::BtTopLeft]->set_active(false);
		buttons[Buttons::BtTopRight]->set_active(false);

		if (bottomEnabled)
		{
			buttons[Buttons::BtBottomLeft]->set_active(false);
			buttons[Buttons::BtBottomRight]->set_active(false);
		}

		if (capeEnabled)
		{
			buttons[Buttons::BtCapeLeft]->set_active(false);
			buttons[Buttons::BtCapeRight]->set_active(false);
		}

		buttons[Buttons::BtShoesLeft]->set_active(false);
		buttons[Buttons::BtShoesRight]->set_active(false);
		buttons[Buttons::BtWeaponLeft]->set_active(false);
		buttons[Buttons::BtWeaponRight]->set_active(false);

		for (size_t i = 0; i < hairSelectSize; i++)
			buttons[Buttons::BtHairSelect0 + i]->set_active(false);
	}

	void UICommonCreation::check_names()
	{
		int16_t max_width = getMaxWidth();

		string_format::format_with_ellipsis(faceName, max_width, 3, true);
		string_format::format_with_ellipsis(hairStyleName, max_width, 3, true);
		string_format::format_with_ellipsis(skinColorName, max_width, 3, true);
		string_format::format_with_ellipsis(topName, max_width, 3, true);
		string_format::format_with_ellipsis(bottomName, max_width, 3, true);
		string_format::format_with_ellipsis(capeName, max_width, 3, true);
		string_format::format_with_ellipsis(shoeName, max_width, 3, true);
		string_format::format_with_ellipsis(weaponName, max_width, 3, true);
	}

	const std::string& UICommonCreation::get_equipname(EquipSlot::Id slot) const
	{
		if (int32_t item_id = newCharLook.get_equips().get_equip(slot))
		{
			return ItemData::get(item_id).get_name();
		}
		else
		{
			static const std::string& nullstr = "NULL";

			return nullstr;
		}
	}

	Point<int16_t> UICommonCreation::getGenderTopPos()
	{
		if (classType == "000")
			return Point<int16_t>(486, 95);
		else if (classType == "2000")
			return Point<int16_t>(491, 168);
		else if (classType == "1000")
			return Point<int16_t>(423, 104);
		else
			return Point<int16_t>(0, 0);
	}

	Point<int16_t> UICommonCreation::getBoardMidPos()
	{
		if (classType == "000")
			return Point<int16_t>(486, 209);
		else if (classType == "2000")
			return Point<int16_t>(491, 220);
		else if (classType == "1000")
			return Point<int16_t>(423, 222);
		else
			return Point<int16_t>(0, 0);
	}

	Point<int16_t> UICommonCreation::getBoardBottomPos()
	{
		if (classType == "000")
			return Point<int16_t>(486, 329);
		else if (classType == "2000")
			return Point<int16_t>(491, 313);
		else if (classType == "1000")
			return Point<int16_t>(423, 348);
		else
			return Point<int16_t>(0, 0);
	}

	Point<int16_t> UICommonCreation::getAvatarSelPos(size_t index)
	{
		if (classType == "000")
		{
			if (index >= 2)
				index++;

			return Point<int16_t>(497, 197 + index * 18);
		}
		else if (classType == "2000")
		{
			return Point<int16_t>(504, 187 + index * 18);
		}
		else if (classType == "1000")
		{
			int16_t y = 0;

			if (index == 3)
				y = 2;

			return Point<int16_t>(416, 98 + y);
		}
		else
		{
			return Point<int16_t>(0, 0);
		}
	}

	Point<int16_t> UICommonCreation::getGenderSelectMalePos()
	{
		if (classType == "000")
			return Point<int16_t>(487, 109);
		else if (classType == "2000")
			return Point<int16_t>(439, 106);
		else if (classType == "1000")
			return Point<int16_t>(425, 107);
		else
			return Point<int16_t>(0, 0);
	}

	Point<int16_t> UICommonCreation::getGenderSelectFemalePos()
	{
		if (classType == "000")
			return Point<int16_t>(485, 109);
		else if (classType == "2000")
			return Point<int16_t>(437, 106);
		else if (classType == "1000")
			return Point<int16_t>(423, 107);
		else
			return Point<int16_t>(0, 0);
	}

	int16_t UICommonCreation::getCharLookY()
	{
		if (classType == "000")
			return 198;
		else if (classType == "2000")
			return 187;
		else if (classType == "1000")
			return 81;
		else
			return 0;
	}

	Point<int16_t> UICommonCreation::getCharLookLeftPos()
	{
		int16_t charLookY = getCharLookY();

		if (classType == "000")
			return Point<int16_t>(552, charLookY);
		else if (classType == "2000")
			return Point<int16_t>(562, charLookY);
		else if (classType == "1000")
			return Point<int16_t>(418, charLookY);
		else
			return Point<int16_t>(0, 0);
	}

	Point<int16_t> UICommonCreation::getCharLookRightPos()
	{
		int16_t charLookY = getCharLookY();

		if (classType == "000")
			return Point<int16_t>(684, charLookY);
		else if (classType == "2000")
			return Point<int16_t>(699, charLookY);
		else if (classType == "1000")
			return Point<int16_t>(415, charLookY);
		else
			return Point<int16_t>(0, 0);
	}

	Point<int16_t> UICommonCreation::getBtYesPos(uint8_t step)
	{
		if (classType == "000")
		{
			switch (step)
			{
				case 0:
					return Point<int16_t>(514, 394);
				case 1:
					return Point<int16_t>(523, 425);
				case 2:
					return Point<int16_t>(513, 273);
			}
		}
		else if (classType == "2000")
		{
			switch (step)
			{
				case 0:
					return Point<int16_t>(520, 397);
				case 1:
					return Point<int16_t>(533, 368);
				case 2:
					return Point<int16_t>(523, 243);
			}
		}
		else if (classType == "1000")
		{
			switch (step)
			{
				case 0:
					return Point<int16_t>(510, 396);
				case 1:
					return Point<int16_t>(502, 381);
				case 2:
					return Point<int16_t>(510, 289);
			}
		}

		return Point<int16_t>(0, 0);
	}

	Point<int16_t> UICommonCreation::getBtNoPos(uint8_t step)
	{
		if (classType == "000")
		{
			switch (step)
			{
				case 0:
					return Point<int16_t>(590, 394);
				case 1:
					return Point<int16_t>(597, 425);
				case 2:
					return Point<int16_t>(587, 273);
			}
		}
		else if (classType == "2000")
		{
			switch (step)
			{
				case 0:
					return Point<int16_t>(594, 397);
				case 1:
					return Point<int16_t>(607, 368);
				case 2:
					return Point<int16_t>(597, 243);
			}
		}
		else if (classType == "1000")
		{
			switch (step)
			{
				case 0:
					return Point<int16_t>(615, 396);
				case 1:
					return Point<int16_t>(607, 381);
				case 2:
					return Point<int16_t>(615, 289);
			}
		}

		return Point<int16_t>(0, 0);
	}

	Point<int16_t> UICommonCreation::getHairSelectPos(size_t index)
	{
		if (classType == "000" || classType == "2000")
			return Point<int16_t>(549 + (index * 15), 234);
		else if (classType == "1000")
			return Point<int16_t>(553 + (index * 15), 238);
		else
			return Point<int16_t>(0, 0);
	}

	Color::Name UICommonCreation::getNewCharNameColor()
	{
		if (classType == "1000")
			return Color::Name::BLACK;
		else
			return Color::Name::WHITE;
	}

	Rectangle<int16_t> UICommonCreation::getNewCharNameRect()
	{
		if (classType == "000")
		{
			Point<int16_t> pos = Point<int16_t>(514, 198);

			return Rectangle<int16_t>(pos, pos + Point<int16_t>(148, 24));
		}
		else if (classType == "2000")
		{
			Point<int16_t> pos = Point<int16_t>(522, 197);

			return Rectangle<int16_t>(pos, pos + Point<int16_t>(146, 25));
		}
		else if (classType == "1000")
		{
			Point<int16_t> pos = Point<int16_t>(533, 213);

			return Rectangle<int16_t>(pos, pos + Point<int16_t>(147, 19));
		}
		else
		{
			return Rectangle<int16_t>(Point<int16_t>(0, 0), Point<int16_t>(0, 0));
		}
	}

	size_t UICommonCreation::getVerticalCount() const
	{
		if (classType == "000" || classType == "2000")
			return 4;
		else if (classType == "1000")
			return 6;
		else
			return 0;
	}

	int16_t UICommonCreation::getVerticalAdj() const
	{
		if (classType == "000" || classType == "2000")
			return 24;
		else if (classType == "1000")
			return 18;
		else
			return 0;
	}

	Point<int16_t> UICommonCreation::getNamePos(size_t index) const
	{
		int16_t name_x = 0;
		int16_t name_y = 0;

		if (classType == "000")
			name_x = 626;
		else if (classType == "2000")
			name_x = 647;
		else if (classType == "1000")
			name_x = 618;

		if (classType == "000")
			name_y = 196;
		else if (classType == "2000")
			name_y = 186;
		else if (classType == "1000")
			name_y = 203;

		return Point<int16_t>(name_x, name_y + index * 18);
	}

	Point<int16_t> UICommonCreation::getNameboardPos() const
	{
		if (classType == "000")
			return Point<int16_t>(486, 95);
		else if (classType == "2000")
			return Point<int16_t>(489, 106);
		else if (classType == "1000")
			return Point<int16_t>(423, 104);
		else
			return Point<int16_t>(0, 0);
	}

	Point<int16_t> UICommonCreation::getTextfieldPos() const
	{
		if (classType == "000")
			return Point<int16_t>(8, -2);
		else if (classType == "2000")
			return Point<int16_t>(3, 0);
		else if (classType == "1000")
			return Point<int16_t>(6, -3);
		else
			return Point<int16_t>(0, 0);
	}

	int16_t UICommonCreation::getMaxWidth() const
	{
		if (classType == "2000")
			return 107;
		else
			return 0;
	}
}