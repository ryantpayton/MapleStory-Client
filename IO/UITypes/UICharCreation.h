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
#include "../UIElement.h"

#include "../Components/Textfield.h"

namespace jrc
{
	// Character creation screen.
	class UICharCreation : public UIElement
	{
	public:
		static constexpr Type TYPE = CHARCREATION;
		static constexpr bool FOCUSED = false;
		static constexpr bool TOGGLED = false;

		UICharCreation();

		void draw(float) const override;
		void update() override;

		Cursor::State send_cursor(bool down, Point<int16_t> pos) override;

		void send_naming_result(bool success);
		void change_class(uint8_t id);

	protected:
		Button::State button_pressed(uint16_t button_id) override;

	private:
		enum Buttons
		{
			BT_BACK,
			BT_CREATE,
			BT_PAGE_L,
			BT_PAGE_R,
			BT_CLASS_SEL0,
			BT_CLASS_SEL1,
			BT_CLASS_SEL2,
			BT_CLASS_SEL3,
			BT_CLASS_SEL4
		};

		enum Classes
		{
			EXPLORERS = 1,
			CYGNUS,
			ARAN,
			EVAN,
			DUAL_BLADE = 8
		};

		Text version;
		nl::node raceSelect;
		std::map<int8_t, Classes> classMap;
		int8_t activeClass;
	};
}
