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

#include "../Components/Textfield.h"

namespace ms
{
	class UIWorldMap : public UIDragElement<PosMAP>
	{
	public:
		static constexpr Type TYPE = UIElement::Type::WORLDMAP;
		static constexpr bool FOCUSED = false;
		static constexpr bool TOGGLED = true;

		UIWorldMap();

		void draw(float inter) const override;
		void update() override;

		void send_key(std::int32_t keycode, bool pressed, bool escape) override;

	protected:
		Button::State button_pressed(std::uint16_t buttonid) override;
		Cursor::State send_cursor(bool clicked, Point<int16_t> cursor_pos) override;

	private:
		void set_search(bool enable);

		enum Buttons
		{
			BT_CLOSE,
			BT_SEARCH,
			BT_AUTOFLY,
			BT_NAVIREG,
			BT_ALLSEARCH,
			BT_SEARCH_CLOSE
		};

		bool search;
		Texture search_background;
		Texture search_notice;
		Textfield search_text;
		Point<int16_t> bg_dimensions;
		Point<int16_t> bg_search_dimensions;
		Point<int16_t> background_dimensions;
	};
}