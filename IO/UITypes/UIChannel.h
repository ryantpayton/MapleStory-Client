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
#include "../UIDragElement.h"

#include "../Template/BoolPair.h"

namespace jrc
{
	class UIChannel : public UIDragElement<PosCHANNEL>
	{
	public:
		static constexpr Type TYPE = CHANNEL;
		static constexpr bool FOCUSED = false;
		static constexpr bool TOGGLED = true;

		UIChannel();

		void draw(float inter) const override;
		void update() override;

		void send_key(int32_t keycode, bool pressed) override;
		Cursor::State send_cursor(bool clicked, Point<int16_t> cursorpos) override;

	protected:
		Button::State button_pressed(uint16_t buttonid) override;

	private:
		void cancel();
		void change_channel();
		void update_selected_channel_position();

		enum Buttons : uint16_t
		{
			CANCEL,
			CHANGE,
			CH
		};

		uint8_t current_channel;
		uint8_t selected_channel;
		uint8_t channel_count;
		BoolPair<Texture> channel;
		std::vector<Sprite> ch;
		int16_t current_channel_x;
		int16_t current_channel_y;
		int16_t selected_channel_x;
		int16_t selected_channel_y;
	};
}