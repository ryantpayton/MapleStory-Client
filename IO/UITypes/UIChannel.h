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

#include "../UIDragElement.h"

#include "../../Template/BoolPair.h"

namespace ms
{
	class UIChannel : public UIDragElement<PosCHANNEL>
	{
	public:
		static constexpr Type TYPE = UIElement::Type::CHANNEL;
		static constexpr bool FOCUSED = false;
		static constexpr bool TOGGLED = true;

		UIChannel();

		void draw(float inter) const override;
		void update() override;

		void send_key(std::int32_t keycode, bool pressed, bool escape) override;
		Cursor::State send_cursor(bool clicked, Point<int16_t> cursorpos) override;

	protected:
		Button::State button_pressed(std::uint16_t buttonid) override;

	private:
		void cancel();
		void change_channel();
		void update_selected_channel_position();

		enum Buttons : std::uint16_t
		{
			CANCEL,
			CHANGE,
			CH
		};

		std::uint8_t current_channel;
		std::uint8_t selected_channel;
		std::uint8_t channel_count;
		BoolPair<Texture> channel;
		std::vector<Sprite> ch;
		std::int16_t current_channel_x;
		std::int16_t current_channel_y;
		std::int16_t selected_channel_x;
		std::int16_t selected_channel_y;
	};
}