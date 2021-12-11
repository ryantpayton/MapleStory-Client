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
#include "UITermsOfService.h"

#include "UILoginWait.h"

#include "../UI.h"

#include "../Components/MapleButton.h"

#include "../../Net/Packets/LoginPackets.h"

#ifdef USE_NX
#include <nlnx/nx.hpp>
#endif

namespace ms
{
	UITermsOfService::UITermsOfService(std::function<void()> oh) : okhandler(oh), offset(0), unit_rows(1)
	{
		nl::node Login = nl::nx::UI["Login.img"];
		nl::node TOS = Login["TOS"];

		sprites.emplace_back(TOS, Point<int16_t>(399, 250));

		buttons[Buttons::OK] = std::make_unique<MapleButton>(Login["BtOk"], Point<int16_t>(483, 425));
		buttons[Buttons::CANCEL] = std::make_unique<MapleButton>(Login["BtCancel"], Point<int16_t>(557, 425));

		EULA = nl::nx::String["TrialEULA.img"]["EULA"];
		max_rows = EULA.size();
		
		text = Text(Text::Font::A11M, Text::Alignment::LEFT, Color::Name::BLACK, "", 340, true, 2);

		int16_t slider_y = 77;

		slider = Slider(
			Slider::Type::LINE_PUNGA, Range<int16_t>(slider_y, slider_y + 305), 574, unit_rows, max_rows,
			[&](bool upwards)
			{
				int16_t shift = upwards ? -1 : 1;
				bool above = offset + shift >= 0;
				bool below = offset + shift <= max_rows - unit_rows;

				if (above && below)
				{
					offset += shift;
					update_accept(offset);
				}
			}
		);

		update_accept(offset);

		position = Point<int16_t>(0, 10);
		dimension = Texture(TOS).get_dimensions();
	}

	void UITermsOfService::draw(float inter) const
	{
		UIElement::draw(inter);

		int16_t range_min = 80;
		text.draw(position + Point<int16_t>(226, 84), Range<int16_t>(range_min, range_min + 316));
		slider.draw(position);
	}

	Cursor::State UITermsOfService::send_cursor(bool clicked, Point<int16_t> cursorpos)
	{
		Point<int16_t> cursoroffset = cursorpos - position;

		if (slider.isenabled())
		{
			Cursor::State state = slider.send_cursor(cursoroffset, clicked);

			if (state != Cursor::State::IDLE)
				return state;
		}

		return UIElement::send_cursor(clicked, cursorpos);
	}

	UIElement::Type UITermsOfService::get_type() const
	{
		return TYPE;
	}

	Button::State UITermsOfService::button_pressed(uint16_t buttonid)
	{
		switch (buttonid)
		{
			case Buttons::OK:
				UI::get().emplace<UILoginWait>();

				TOSPacket().dispatch();
				break;
			case Buttons::CANCEL:
				UI::get().quit();
				break;
			default:
				break;
		}

		return Button::State::NORMAL;
	}

	void UITermsOfService::update_accept(uint16_t offset)
	{
		if (offset == max_rows - unit_rows)
			buttons[Buttons::OK]->set_state(Button::State::NORMAL);
		else
			buttons[Buttons::OK]->set_state(Button::State::DISABLED);

		std::string shownText = "";

		for (nl::node text : EULA)
		{
			std::string name = text.name();
			name = name.substr(4, 6);

			int32_t i = std::stoi(name);

			if (i >= offset && i <= offset + 5)
				shownText += text;
		}

		text.change_text(shownText);
	}
}