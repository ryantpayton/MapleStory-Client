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

#include "../../Graphics/Geometry.h"
#include "../../Template/BoolPair.h"

namespace ms
{
	class UILogin : public UIElement
	{
	public:
		static constexpr Type TYPE = UIElement::Type::LOGIN;
		static constexpr bool FOCUSED = false;
		static constexpr bool TOGGLED = false;

		UILogin();

		void draw(float alpha) const override;
		void update() override;

		Cursor::State send_cursor(bool clicked, Point<int16_t> cursor_pos) override;

		UIElement::Type get_type() const override;

	protected:
		Button::State button_pressed(uint16_t id) override;

	private:
		static constexpr int16_t TEXTFIELD_LIMIT = 12;

		void login();
		void open_url(uint16_t id);

		enum Buttons
		{
			BtLogin,
			BtNew,
			BtHomePage,
			BtPasswdLost,
			BtEmailLost,
			BtEmailSave,
			BtQuit,
			BtMapleID,
			BtNexonID,
			NUM_BUTTONS
		};

		Text version;
		Point<int16_t> version_pos;
		Textfield account;
		Point<int16_t> account_src_dim;
		BoolPair<Texture> account_bg;
		Textfield password;
		Point<int16_t> password_src_dim;
		Texture password_bg;
		BoolPair<Texture> check;
		Texture capslock;
		ColorBox background;
		Point<int16_t> title_pos;

		bool saveid;
		bool nexon;
	};
}