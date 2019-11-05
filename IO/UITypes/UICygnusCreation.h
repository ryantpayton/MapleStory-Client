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
#pragma once

#include "../UIElement.h"

#include "../Components/Textfield.h"
#include "../../Template/BoolPair.h"

#include "../../Character/Look/CharLook.h"

namespace ms
{
	class UICygnusCreation : public UIElement
	{
	public:
		static constexpr Type TYPE = UIElement::Type::CLASSCREATION;
		static constexpr bool FOCUSED = false;
		static constexpr bool TOGGLED = false;

		UICygnusCreation();

		void draw(float inter) const override;
		void update() override;

		Cursor::State send_cursor(bool clicked, Point<int16_t> cursorpos) override;
		void send_key(std::int32_t keycode, bool pressed, bool escape) override;

		void send_naming_result(bool nameused);

	protected:
		Button::State button_pressed(std::uint16_t buttonid) override;

	private:
		void randomize_look();
		const std::string& get_equipname(Equipslot::Id slot) const;

		enum Buttons : std::uint16_t
		{
			BT_BACK,
			BT_CHARC_OK,
			BT_CHARC_CANCEL,
			BT_CHARC_SKINL,
			BT_CHARC_SKINR,
			BT_CHARC_WEPL,
			BT_CHARC_WEPR,
			BT_CHARC_GENDER_M,
			BT_CHARC_GEMDER_F,
			BT_CHARC_HAIRC0,
			BT_CHARC_HAIRC1,
			BT_CHARC_HAIRC2,
			BT_CHARC_HAIRC3,
			BT_CHARC_HAIRC4,
			BT_CHARC_HAIRC5,
			BT_CHARC_HAIRC6,
			BT_CHARC_HAIRC7
		};

		enum GenderButtons : std::uint8_t
		{
			GENDER_BACKGROUND,
			GENDER_HEAD,
			GENDER_TOP,
			GENDER_MID,
			GENDER_BOTTOM
		};

		std::vector<Sprite> sprites_lookboard;
		std::vector<Sprite> sprites_gender_select;
		std::vector<Sprite> sprites_keytype;
		Texture sky;
		Texture cloud;
		float cloudfx;
		Texture nameboard;
		Textfield namechar;
		CharLook newchar;
		Randomizer randomizer;

		BoolPair<std::vector<std::uint8_t>> skins;
		BoolPair<std::vector<std::uint8_t>> haircolors;
		BoolPair<std::vector<std::int32_t>> faces;
		BoolPair<std::vector<std::int32_t>> hairs;
		BoolPair<std::vector<std::int32_t>> tops;
		BoolPair<std::vector<std::int32_t>> bots;
		BoolPair<std::vector<std::int32_t>> shoes;
		BoolPair<std::vector<std::int32_t>> weapons;

		bool gender;
		bool charSet;
		bool named;
		bool female;
		std::size_t skin;
		std::size_t haircolor;
		std::size_t face;
		std::size_t hair;
		std::size_t top;
		std::size_t bot;
		std::size_t shoe;
		std::size_t weapon;
		Text facename;
		Text hairname;
		Text bodyname;
		Text topname;
		Text botname;
		Text shoename;
		Text wepname;
		Text version;
	};
}