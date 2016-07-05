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

#include "../Components/Slider.h"
#include "../Components/Textfield.h"

#include "../../Graphics/Geometry.h"
#include "../../Graphics/Texture.h"

#include <vector>
#include <unordered_map>

namespace jrc
{
	class UIChatbar : public UIElement
	{
	public:
		enum ChatTarget
		{
			CHT_ALL,
			CHT_BUDDY,
			CHT_GUILD,
			CHT_ALLIANCE,
			CHT_PARTY,
			CHT_SQUAD,
			NUM_TARGETS
		};

		enum LineType
		{
			UNK0,
			WHITE,
			RED,
			BLUE,
			YELLOW
		};

		UIChatbar(Point<int16_t> position);

		void draw(float inter) const override;
		void update() override;

		bool is_in_range(Point<int16_t> cursorpos) const override;
		bool remove_cursor(bool clicked, Point<int16_t> cursorpos) override;
		Cursor::State send_cursor(bool pressed, Point<int16_t> cursorpos) override;

		void send_line(const std::string& line, LineType type);

	protected:
		Button::State button_pressed(uint16_t buttonid) override;

	private:
		int16_t getchattop() const;

		enum Buttons : uint16_t
		{
			BT_OPENCHAT,
			BT_CLOSECHAT,
			BT_SCROLLUP,
			BT_SCROLLDOWN,
			BT_CHATTARGETS
		};

		static constexpr int16_t CHATYOFFSET = 65;
		static constexpr int16_t CHATROWHEIGHT = 16;
		static constexpr int16_t MAXCHATROWS = 16;
		static constexpr int16_t MINCHATROWS = 1;

		Textfield chatfield;
		Texture chatspace[2];
		Texture chattargets[NUM_TARGETS];
		Texture chatenter;
		Texture chatcover;
		Texture tapbar;
		Texture tapbartop;

		bool chatopen;
		ChatTarget chattarget;

		std::vector<std::string> lastentered;
		size_t lastpos;

		std::unordered_map<int16_t, Text> rowtexts;
		ColorBox chatbox;
		int16_t chatrows;
		int16_t rowpos;
		int16_t rowmax;
		Slider slider;
		bool dragchattop;
	};
}