/////////////////////////////////////////////////////////////////////////////
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

#include "../../Graphics/Text.h"
#include "../../Util/Randomizer.h"

#include <functional>

namespace jrc
{
	// Keyboard which is used via the mouse. The game uses this for pic/pin input.
	class UISoftkey : public UIElement
	{
	public:
		using Callback = std::function<void(const std::string&)>;

		static constexpr Type TYPE = SOFTKEYBOARD;
		static constexpr bool FOCUSED = true;
		static constexpr bool TOGGLED = false;

		UISoftkey(Callback callback);

		void draw(float alpha) const override;

	protected:
		Button::State button_pressed(uint16_t) override;

	private:
		void shufflekeys();
		Point<int16_t> keypos(uint8_t) const;

		enum Buttons
		{
			BT_0,
			BT_1,
			BT_2,
			BT_3,
			BT_4,
			BT_5,
			BT_6,
			BT_7,
			BT_8,
			BT_9,
			BT_NEXT,
			BT_BACK,
			BT_CANCEL,
			BT_OK
		};

		static constexpr size_t MIN_SIZE = 6;
		static constexpr size_t MAX_SIZE = 12;
		static constexpr uint8_t NUM_KEYS = 10;

		Callback callback;
		Text entry;
		Randomizer random;
	};
}

