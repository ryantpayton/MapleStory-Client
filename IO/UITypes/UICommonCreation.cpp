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
#include "UIWorldSelect.h"

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
	UICommonCreation::UICommonCreation(const std::string& c) : UIElement(Point<int16_t>(0, 0), Point<int16_t>(800, 600)), classType(c), className(class_map[c]), genderSelected(false), charLookSelected(false), charNameSelected(false), newCharNameDisabled(true), female(false)
	{
		load_char_look();

		std::string version_text = Configuration::get().get_version();
		version = Text(Text::Font::A12B, Text::Alignment::LEFT, Color::Name::LEMONGRASS, "Ver. " + version_text);

		nl::node Login = nl::nx::UI["Login.img"];
		nl::node Common = Login["Common"];
		version_pos = Common["version"]["pos"];

		nl::node createLimit = Login["NewChar"]["createLimit"];
		nl::node CustomizeChar = Login["CustomizeChar"][classType];
		nl::node board = CustomizeChar["board"];
		nl::node genderSelect = CustomizeChar["genderSelect"];
		nl::node CustomizeCharInfo = CustomizeChar["info"];

		avatarOrigin = CustomizeCharInfo["avatarOrigin"];
		Point<int16_t> avatarSize = CustomizeCharInfo["avatarSize"];
		charNameOrigin = CustomizeCharInfo["charNameOrigin"];
		Point<int16_t> genderFemale = CustomizeCharInfo["genderFemale"];
		Point<int16_t> genderMale = CustomizeCharInfo["genderMale"];
		int16_t genderMarginX = CustomizeCharInfo["genderMarginX"];
		int16_t genderMarginY = CustomizeCharInfo["genderMarginY"];
		genderOrigin = CustomizeCharInfo["genderOrigin"];
		Point<int16_t> genderSize = CustomizeCharInfo["genderSize"];
		Point<int16_t> nameEditCtrlPos = CustomizeCharInfo["nameEditCtrlPos"];
		std::string nameEditFontColor = CustomizeCharInfo["nameEditFontColor"];
		newAvatarPos = CustomizeCharInfo["newAvatarPos"];
		Point<int16_t> offsetForElemBack = CustomizeCharInfo["offsetForElemBack"];
		vCanvasCount = CustomizeCharInfo["vCanvasCount"];

		if (vCanvasCount == 0)
		{
			if (className == "adventurer")
				vCanvasCount = 6;
			else if (className == "cygnusKnight")
				vCanvasCount = 7;
		}

		nl::node CustomizeCharObj = nl::nx::Map["Obj"]["login.img"]["CustomizeChar"][class_map[classType]];

		int16_t genderHeight = board["genderHeight"];
		int16_t avatarHeight = board["avatarHeight"];

		nl::node genderTop = board["genderTop"];
		genderTopHeight = Texture(genderTop).height();

		nl::node boardBottom = board["boardBottom"];
		int16_t boardBottomHeight = Texture(boardBottom).height();

		nl::node boardMid = board["boardMid"];
		boardMidHeight = Texture(boardMid).height();

		sprites_background.emplace_back(CustomizeCharObj["0"], Point<int16_t>(512, 384));

		sprites.emplace_back(createLimit["backgrnd"]);

		if (className == "adventurer" || className == "cygnusKnight")
			sprites_gender_select.emplace_back(genderTop, genderOrigin);
		else
			sprites_gender_select.emplace_back(genderTop, genderOrigin - Point<int16_t>(1, 0));

		if (className == "cygnusKnight")
			sprites_gender_select.emplace_back(boardMid, genderOrigin - Point<int16_t>(0, 112));
		else
			sprites_gender_select.emplace_back(boardMid, genderOrigin);

		if (className == "cygnusKnight")
			sprites_gender_select.emplace_back(boardBottom, genderOrigin + Point<int16_t>(0, genderHeight - boardBottomHeight) - Point<int16_t>(0, 238));
		else
			sprites_gender_select.emplace_back(boardBottom, genderOrigin + Point<int16_t>(0, genderHeight - boardBottomHeight));

		if (className == "adventurer")
			sprites_lookboard.emplace_back(board["avatarTop"], genderOrigin);
		else
			sprites_lookboard.emplace_back(board["avatarTop"], genderOrigin + Point<int16_t>(1, 0));

		sprites_lookboard.emplace_back(boardMid, genderOrigin);

		if (className == "cygnusKnight")
			sprites_lookboard.emplace_back(boardBottom, genderOrigin);
		else
			sprites_lookboard.emplace_back(boardBottom, genderOrigin + Point<int16_t>(0, avatarHeight - boardBottomHeight));

		nl::node avatarSel = CustomizeChar["avatarSel"];

		Point<int16_t> avatarSelAdj = Point<int16_t>(0, 0);

		if (className == "adventurer")
			avatarSelAdj = Point<int16_t>(44, 108);
		else if (className == "aran")
			avatarSelAdj = Point<int16_t>(36, 92);
		else if (className == "cygnusKnight")
			avatarSelAdj = Point<int16_t>(0, 18);
		else
			avatarSelAdj = Point<int16_t>(0, 0);

		if (className == "cygnusKnight")
			sprites_char_look.emplace_back(avatarSel["WZ2_skin"], avatarOrigin - Point<int16_t>(0, 18 * 3));
		else
			sprites_char_look.emplace_back(avatarSel["WZ2_skin"], avatarOrigin + avatarSelAdj);

		for (size_t i = 0; i < avatarSel.size() - 1; i++)
		{
			size_t t = i;

			if (className == "adventurer" && i >= 2)
				t++;

			if (className == "cygnusKnight")
			{
				if (i == 2)
					sprites_char_look.emplace_back(avatarSel[i]["normal"], avatarOrigin + avatarSelAdj - Point<int16_t>(0, 16));
				else if (i > 2)
					sprites_char_look.emplace_back(avatarSel[i]["normal"], avatarOrigin + avatarSelAdj - Point<int16_t>(0, 18));
				else
					sprites_char_look.emplace_back(avatarSel[i]["normal"], avatarOrigin + avatarSelAdj);
			}
			else
			{
				sprites_char_look.emplace_back(avatarSel[i]["normal"], avatarOrigin + avatarSelAdj + Point<int16_t>(0, 18 * (t + 1)));
			}
		}

		nameboard = CustomizeChar["charName"];
		createLimitPos = Texture(createLimit["backgrnd"]).get_origin();
		createLimitNum = Charset(createLimit["num"], Charset::Alignment::RIGHT);

		buttons[Buttons::BtStart] = std::make_unique<MapleButton>(Common["BtStart"], Point<int16_t>(0, 1));
		buttons[Buttons::BtPreview] = std::make_unique<MapleButton>(Common["BtPreview"]);
		buttons[Buttons::BtGenderMale] = std::make_unique<MapleButton>(genderSelect["male"], getGenderSelectMalePos());
		buttons[Buttons::BtGenderFemale] = std::make_unique<MapleButton>(genderSelect["female"], getGenderSelectFemalePos());

		Point<int16_t> charLookAdj = Point<int16_t>(0, 0);

		if (className == "adventurer")
			charLookAdj = Point<int16_t>(75, 108);
		else if (className == "aran")
			charLookAdj = Point<int16_t>(69, 92);
		else
			charLookAdj = Point<int16_t>(0, 0);

		Point<int16_t> charLookLeft = avatarOrigin + charLookAdj;
		Point<int16_t> charLookRight = charLookLeft;

		if (className == "adventurer")
			charLookRight += Point<int16_t>(118, 0);
		else if (className == "aran")
			charLookRight += Point<int16_t>(110, 0);
		else
			charLookRight += Point<int16_t>(0, 0);

		buttons[Buttons::BtSkinLeft] = std::make_unique<MapleButton>(CustomizeChar["BtLeft"], charLookLeft);
		buttons[Buttons::BtSkinRight] = std::make_unique<MapleButton>(CustomizeChar["BtRight"], charLookRight);

		charLookLeft.shift_y(18);
		charLookRight.shift_y(18);

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
		}

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

		nl::node BtYes = CustomizeChar["BtYes"];
		BtYesPos = BtYes["pos"];

		nl::node BtNo = CustomizeChar["BtNo"];
		BtNoPos = BtNo["pos"];

		buttons[Buttons::BtYes] = std::make_unique<MapleButton>(BtYes, genderOrigin + Point<int16_t>(BtYesPos["gender"]));
		buttons[Buttons::BtNo] = std::make_unique<MapleButton>(BtNo, genderOrigin + Point<int16_t>(BtNoPos["gender"]));

		nl::node hairSelect = CustomizeChar["hairSelect"];
		hairSelectSize = hairSelect.size();

		for (size_t i = 0; i < hairSelectSize; i++)
			buttons[Buttons::BtHairSelect0 + i] = std::make_unique<MapleButton>(hairSelect[i], getHairSelectPos(i));

		disable_char_look();

		newCharNamePos = charNameOrigin + nameEditCtrlPos - Point<int16_t>(5, 5);

		if (className == "adventurer")
			newCharNamePos += Point<int16_t>(3, 0);
		else if (className == "cygnusKnight")
			newCharNamePos += Point<int16_t>(-1, 1);

		Point<int16_t> newCharNameDim = Point<int16_t>(0, 0);

		if (className == "adventurer")
			newCharNameDim = Point<int16_t>(163, 24);
		else if (className == "aran")
			newCharNameDim = Point<int16_t>(149, 25);
		else if (className == "cygnusKnight")
			newCharNameDim = Point<int16_t>(147, 19);
		else
			newCharNameDim = Point<int16_t>(0, 0);

		newCharName = Textfield(Text::Font::A12M, Text::Alignment::LEFT, getNewCharNameColor(), Rectangle<int16_t>(newCharNamePos, newCharNamePos + newCharNameDim), 12);

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

		skinColorName = Text(Text::Font::A11M, Text::Alignment::CENTER, Color::Name::BLACK);
		faceName = Text(Text::Font::A11M, Text::Alignment::CENTER, Color::Name::BLACK);
		hairStyleName = Text(Text::Font::A11M, Text::Alignment::CENTER, Color::Name::BLACK);
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
					for (size_t f = 0; f < vCanvasCount; f++)
						sprites_gender_select[i].draw(position + Point<int16_t>(0, genderTopHeight + (boardMidHeight * f)), inter);
				}
				else
				{
					sprites_gender_select[i].draw(position, inter);
				}
			}

			UIElement::draw(inter);

			newCharLook.draw(newAvatarPos, inter);
		}
		else
		{
			if (!charLookSelected)
			{
				UIElement::draw_sprites(inter);

				for (size_t i = 0; i < sprites_lookboard.size(); i++)
				{
					if (i == 0 && className == "cygnusKnight")
					{
						sprites_lookboard[i].draw(position - Point<int16_t>(1, 0), inter);
					}
					else if (i == 1)
					{
						for (size_t f = 0; f < vCanvasCount; f++)
						{
							if (className == "cygnusKnight")
								sprites_lookboard[i].draw(position + Point<int16_t>(0, boardMidHeight * f), inter);
							else
								sprites_lookboard[i].draw(position + Point<int16_t>(0, genderTopHeight + (boardMidHeight * f)), inter);
						}
					}
					else
					{
						sprites_lookboard[i].draw(position, inter);
					}
				}

				for (auto& sprite : sprites_char_look)
					sprite.draw(position, inter);

				Point<int16_t> label_pos = avatarOrigin;

				if (className == "adventurer")
					label_pos += Point<int16_t>(140, 107);
				else if (className == "aran")
					label_pos += Point<int16_t>(132, 91);
				else if (className == "cygnusKnight")
					label_pos += Point<int16_t>(199, 105);
				else
					label_pos += Point<int16_t>(0, 0);

				skinColorName.draw(label_pos);

				label_pos.shift_y(18);

				faceName.draw(label_pos);

				label_pos.shift_y(18);

				hairStyleName.draw(label_pos);

				label_pos.shift_y(18);

				if (hairColorEnabled)
					label_pos.shift_y(18);

				topName.draw(label_pos);

				label_pos.shift_y(18);

				if (bottomEnabled)
				{
					bottomName.draw(label_pos);

					label_pos.shift_y(18);
				}

				if (capeEnabled)
				{
					capeName.draw(label_pos);

					label_pos.shift_y(18);
				}

				shoeName.draw(label_pos);

				label_pos.shift_y(18);

				weaponName.draw(label_pos);
				newCharLook.draw(newAvatarPos, inter);

				UIElement::draw_buttons(inter);
			}
			else
			{
				if (!charNameSelected)
				{
					UIElement::draw_sprites(inter);

					nameboard.draw(charNameOrigin);

					if (className == "adventurer")
						newCharName.draw(position + Point<int16_t>(2, -2), Point<int16_t>(1, -1));
					else if (className == "cygnusKnight")
						newCharName.draw(position + Point<int16_t>(6, -3), Point<int16_t>(1, -1));
					else if (className == "aran")
						newCharName.draw(position + Point<int16_t>(6, -2), Point<int16_t>(1, -1));
					else
						newCharName.draw(position);

					newCharLook.draw(newAvatarPos, inter);

					UIElement::draw_buttons(inter);
				}
				else
				{
					UIElement::draw_sprites(inter);

					nameboard.draw(charNameOrigin);

					UIElement::draw_buttons(inter);
				}
			}
		}

		// TOOD: Get number of character able to create
		createLimitNum.draw("10", 5, position - createLimitPos + Point<int16_t>(7, 0));

		version.draw(position + version_pos - Point<int16_t>(0, 5));
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

				int64_t skin = skinColors[skinColorIndex];
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

				if (className == "adventurer")
					job = 1;
				else if (className == "aran")
					job = 2;
				else if (className == "cygnusKnight")
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
			UI::get().remove(UIElement::Type::RACESELECT);
			UI::get().remove(UIElement::Type::CHARSELECT);

			if (auto worldselect = UI::get().get_element<UIWorldSelect>())
				worldselect->makeactive();

			return Button::State::NORMAL;
		}
		else if (buttonid == Buttons::BtPreview)
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

					buttons[Buttons::BtYes]->set_position(genderOrigin + Point<int16_t>(BtYesPos["name"]));
					buttons[Buttons::BtNo]->set_position(genderOrigin + Point<int16_t>(BtNoPos["name"]));

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

					buttons[Buttons::BtYes]->set_position(genderOrigin + Point<int16_t>(BtYesPos["gender"]));
					buttons[Buttons::BtNo]->set_position(genderOrigin + Point<int16_t>(BtNoPos["gender"]));

					return Button::State::NORMAL;
				}
				else
				{
					button_pressed(Buttons::BtPreview);

					return Button::State::NORMAL;
				}
			}
		}
		else if (buttonid == Buttons::BtSkinLeft)
		{
			skinColorIndex = (skinColorIndex > 0) ? skinColorIndex - 1 : skinColors.size() - 1;

			newCharLook.set_body(skinColors[skinColorIndex]);
			skinColorName.change_text(newCharLook.get_body()->get_name());

			check_names();

			return Button::State::NORMAL;
		}
		else if (buttonid == Buttons::BtSkinRight)
		{
			skinColorIndex = (skinColorIndex < skinColors.size() - 1) ? skinColorIndex + 1 : 0;

			newCharLook.set_body(skinColors[skinColorIndex]);
			skinColorName.change_text(newCharLook.get_body()->get_name());

			check_names();

			return Button::State::NORMAL;
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
		skinColorIndex = randomizer.next_int(skinColors.size());
		faceIndex = randomizer.next_int(faces[female].size());
		hairStyleIndex = randomizer.next_int(hairStyles[female].size());

		int64_t hairStyle = hairStyles[female][hairStyleIndex];
		auto& hairColors_ = hairColors[female][hairStyle];
		hairColorIndex = randomizer.next_int(hairColors_.size());

		topIndex = randomizer.next_int(tops[female].size());
		bottomIndex = randomizer.next_int(bottoms[female].size());
		capeIndex = randomizer.next_int(capes[female].size());
		shoeIndex = randomizer.next_int(shoes[female].size());
		weaponIndex = randomizer.next_int(weapons[female].size());

		newCharLook.set_body(skinColors[skinColorIndex]);
		newCharLook.set_face(faces[female][faceIndex]);

		if (hairColors_.size() > 0)
			newCharLook.set_hair(hairColors_[hairColorIndex]);
		else
			newCharLook.set_hair(hairStyle);

		newCharLook.add_equip(tops[female][topIndex]);

		if (bottoms[female].size() > 0)
			newCharLook.add_equip(bottoms[female][bottomIndex]);

		if (capes[female].size() > 0)
			newCharLook.add_equip(capes[female][capeIndex]);

		newCharLook.add_equip(shoes[female][shoeIndex]);
		newCharLook.add_equip(weapons[female][weaponIndex]);

		skinColorName.change_text(newCharLook.get_body()->get_name());
		faceName.change_text(newCharLook.get_face()->get_name());
		hairStyleName.change_text(newCharLook.get_hair()->get_name());
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

		nl::node MakeCharInfo = nl::nx::Etc["MakeCharInfo.img"][classType];

		for (nl::node gender_node : MakeCharInfo)
		{
			std::string gender_node_name = gender_node.name();

			if (gender_node_name == "info")
			{
				choosableGender = gender_node["choosableGender"].get_bool();

				if (!choosableGender)
					button_pressed(Buttons::BtYes);

				nl::node skins = gender_node["WZ2_skin"];

				for (nl::node skin : skins)
					skinColors.push_back(skin["id"].get_integer() - 12000);

				continue;
			}

			bool f = gender_node_name == "female";

			for (size_t type = 0; type < gender_node.size(); type++)
			{
				for (nl::node id_node : gender_node[type])
				{
					std::string id_node_name = id_node.name();

					if (id_node_name == "color" || id_node_name == "name")
						continue;

					int64_t id_node_value = id_node.get_integer();

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
		buttons[Buttons::BtYes]->set_position(genderOrigin + Point<int16_t>(BtYesPos["avatar"]));
		buttons[Buttons::BtNo]->set_position(genderOrigin + Point<int16_t>(BtNoPos["avatar"]));

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

		if (skinColors.size() > 1)
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

		string_format::format_with_ellipsis(skinColorName, max_width, 3, false);
		string_format::format_with_ellipsis(faceName, max_width, 3, false);
		string_format::format_with_ellipsis(hairStyleName, max_width, 3, false);
		string_format::format_with_ellipsis(topName, max_width, 3, false);
		string_format::format_with_ellipsis(bottomName, max_width, 3, false);
		string_format::format_with_ellipsis(capeName, max_width, 3, false);
		string_format::format_with_ellipsis(shoeName, max_width, 3, false);
		string_format::format_with_ellipsis(weaponName, max_width, 3, false);
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
		if (className == "adventurer")
			return Point<int16_t>(486, 95);
		else if (className == "aran")
			return Point<int16_t>(491, 168);
		else if (className == "cygnusKnight")
			return Point<int16_t>(423, 104);
		else
			return Point<int16_t>(0, 0);
	}

	Point<int16_t> UICommonCreation::getBoardMidPos()
	{
		if (className == "adventurer")
			return Point<int16_t>(486, 209);
		else if (className == "aran")
			return Point<int16_t>(491, 220);
		else if (className == "cygnusKnight")
			return Point<int16_t>(423, 222);
		else
			return Point<int16_t>(0, 0);
	}

	Point<int16_t> UICommonCreation::getBoardBottomPos()
	{
		if (className == "adventurer")
			return Point<int16_t>(486, 329);
		else if (className == "aran")
			return Point<int16_t>(491, 313);
		else if (className == "cygnusKnight")
			return Point<int16_t>(423, 348);
		else
			return Point<int16_t>(0, 0);
	}

	Point<int16_t> UICommonCreation::getAvatarSelPos(size_t index)
	{
		if (className == "adventurer")
		{
			if (index >= 2)
				index++;

			return Point<int16_t>(497, 197 + index * 18);
		}
		else if (className == "aran")
		{
			return Point<int16_t>(504, 187 + index * 18);
		}
		else if (className == "cygnusKnight")
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
		Point<int16_t> genderSelectAdj = Point<int16_t>(-33, -13);

		if (className == "adventurer")
			genderSelectAdj = Point<int16_t>(0, -4);
		else if (className == "aran")
			genderSelectAdj = Point<int16_t>(-33, -13);
		else if (className == "cygnusKnight")
			genderSelectAdj = Point<int16_t>(2, 3);

		return genderOrigin + genderSelectAdj;
	}

	Point<int16_t> UICommonCreation::getGenderSelectFemalePos()
	{
		Point<int16_t> genderSelectMalePos = getGenderSelectMalePos();

		if (className == "adventurer")
			return genderSelectMalePos;
		else
			return genderSelectMalePos - Point<int16_t>(2, 0);
	}

	int16_t UICommonCreation::getCharLookY()
	{
		if (className == "adventurer")
			return 198;
		else if (className == "aran")
			return 187;
		else if (className == "cygnusKnight")
			return 81;
		else
			return 0;
	}

	Point<int16_t> UICommonCreation::getCharLookLeftPos()
	{
		int16_t charLookY = getCharLookY();

		if (className == "adventurer")
			return Point<int16_t>(552, charLookY);
		else if (className == "aran")
			return Point<int16_t>(562, charLookY);
		else if (className == "cygnusKnight")
			return Point<int16_t>(418, charLookY);
		else
			return Point<int16_t>(0, 0);
	}

	Point<int16_t> UICommonCreation::getCharLookRightPos()
	{
		int16_t charLookY = getCharLookY();

		if (className == "adventurer")
			return Point<int16_t>(684, charLookY);
		else if (className == "aran")
			return Point<int16_t>(699, charLookY);
		else if (className == "cygnusKnight")
			return Point<int16_t>(415, charLookY);
		else
			return Point<int16_t>(0, 0);
	}

	Point<int16_t> UICommonCreation::getBtYesPos(uint8_t step)
	{
		if (className == "adventurer")
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
		else if (className == "aran")
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
		else if (className == "cygnusKnight")
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
		if (className == "adventurer")
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
		else if (className == "aran")
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
		else if (className == "cygnusKnight")
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
		if (className == "adventurer")
			return Point<int16_t>(677 + (index * 15), 339);
		else if (className == "cygnusKnight")
			return Point<int16_t>(674 + (index * 15), 349);
		else
			return Point<int16_t>(0, 0);
	}

	Color::Name UICommonCreation::getNewCharNameColor()
	{
		if (className == "aran")
			return Color::Name::WHITE;
		else
			return Color::Name::BLACK;
	}

	Rectangle<int16_t> UICommonCreation::getNewCharNameRect()
	{
		if (className == "adventurer")
		{
			Point<int16_t> pos = Point<int16_t>(514, 198);

			return Rectangle<int16_t>(pos, pos + Point<int16_t>(148, 24));
		}
		else if (className == "aran")
		{
			Point<int16_t> pos = Point<int16_t>(522, 197);

			return Rectangle<int16_t>(pos, pos + Point<int16_t>(146, 25));
		}
		else if (className == "cygnusKnight")
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
		if (className == "adventurer" || className == "aran")
			return 4;
		else if (className == "cygnusKnight")
			return 6;
		else
			return 0;
	}

	int16_t UICommonCreation::getVerticalAdj() const
	{
		if (className == "adventurer" || className == "aran")
			return 24;
		else if (className == "cygnusKnight")
			return 18;
		else
			return 0;
	}

	Point<int16_t> UICommonCreation::getNamePos(size_t index) const
	{
		int16_t name_x = 0;
		int16_t name_y = 0;

		if (className == "adventurer")
			name_x = 626;
		else if (className == "aran")
			name_x = 647;
		else if (className == "cygnusKnight")
			name_x = 618;

		if (className == "adventurer")
			name_y = 196;
		else if (className == "aran")
			name_y = 186;
		else if (className == "cygnusKnight")
			name_y = 203;

		return Point<int16_t>(name_x, name_y + index * 18);
	}

	Point<int16_t> UICommonCreation::getNameboardPos() const
	{
		if (className == "adventurer")
			return Point<int16_t>(486, 95);
		else if (className == "aran")
			return Point<int16_t>(489, 106);
		else if (className == "cygnusKnight")
			return Point<int16_t>(423, 104);
		else
			return Point<int16_t>(0, 0);
	}

	Point<int16_t> UICommonCreation::getTextfieldPos() const
	{
		if (className == "adventurer")
			return Point<int16_t>(8, -2);
		else if (className == "aran")
			return Point<int16_t>(3, 0);
		else if (className == "cygnusKnight")
			return Point<int16_t>(6, -3);
		else
			return Point<int16_t>(0, 0);
	}

	int16_t UICommonCreation::getMaxWidth() const
	{
		if (className == "adventurer")
			return 89;
		else if (className == "aran")
			return 94;
		else if (className == "cygnusKnight")
			return 97;
		else
			return 0;
	}
}