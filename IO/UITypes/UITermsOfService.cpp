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
#include "UITermsOfService.h"
#include "UILoginwait.h"

#include "../UI.h"

#include "../Components/MapleButton.h"

#include "../Net/Packets/LoginPackets.h"

#include "nlnx/nx.hpp"

namespace jrc
{
	UITermsOfService::UITermsOfService(std::function<void()> oh) : UIElement(), okhandler(oh)
	{
		offset = 0;
		unit_rows = 1;

		nl::node Login = nl::nx::ui["Login.img"];
		nl::node TOS = Login["TOS"];

		Point<int16_t> TOS_dimensions = Texture(TOS).get_dimensions();

		sprites.emplace_back(TOS, Point<int16_t>(399, 250));

		buttons[Buttons::OK] = std::make_unique<MapleButton>(Login["BtOk"], Point<int16_t>(483, 425));
		buttons[Buttons::CANCEL] = std::make_unique<MapleButton>(Login["BtCancel"], Point<int16_t>(557, 425));

		std::string linebreak = "\\r\\n";
		std::string EULA = "End User License Agreement for MapleStoryTM Software";

		EULA += linebreak;

		EULA += "IMPORTANT NOTICE: The MapleStoryTM software and related ";
		EULA += "documentation are the property of Nexon America Inc ('Nexon') and ";
		EULA += "its licensors, and are protected by international copyright and other ";
		EULA += "intellectual property laws. Unauthorized installation or use of the ";
		EULA += "MapleStory software is strictly prohibited.";

		EULA += linebreak;

		EULA += "This End User License Agreement ('EULA') is an agreement between ";
		EULA += "you and Nexon, and governs your installation and use of the ";
		EULA += "MapleStory software. This EULA limits and excludes warranties and ";
		EULA += "remedies regarding the MapleStory software, exempts NEXON and ";
		EULA += "other persons from liability, specifies the jurisdiciton for resolution of ";
		EULA += "disputes, and contains other important provisions that you should read.";

		EULA += linebreak;

		EULA += "By installing or using the MapleStory Software, you acknowledge and ";
		EULA += "signify that you have read, understood, and agreed to be bound by ";
		EULA += "this EULA. If you do not unconditionally accept and agree to all of ";
		EULA += "the provisions of this EULA, you may not install or use the MapleStory ";
		EULA += "software, and you must permanetly delete and destroy all copies of ";
		EULA += "the MapleStory software in your possession or control.";

		text = Text(Text::Font::A11M, Text::Alignment::LEFT, Text::Color::BLACK, EULA, 340, true, 2);
		max_rows = std::floor(text.height() / 50) - 5;

		int16_t slider_y = 77;

		slider = Slider(
			Slider::Type::GOLD, Range<int16_t>(slider_y, slider_y + 305), 574, unit_rows, max_rows,
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

		position = Point<int16_t>(0, 0);
		dimension = TOS_dimensions;
		active = true;
	}

	void UITermsOfService::draw(float inter) const
	{
		UIElement::draw(inter);

		text.draw(position + Point<int16_t>(226, 84 - offset * 50));
		slider.draw(position);
	}

	void UITermsOfService::update()
	{
		UIElement::update();
	}

	bool UITermsOfService::remove_cursor(bool clicked, Point<int16_t> cursorpos)
	{
		if (slider.remove_cursor(clicked))
			return true;

		return UIElement::remove_cursor(clicked, cursorpos);
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

	Button::State UITermsOfService::button_pressed(uint16_t buttonid)
	{
		switch (buttonid)
		{
		case Buttons::OK:
			UI::get().emplace<UILoginwait>();

			TOSPacket().dispatch();
			break;
		case Buttons::CANCEL:
			deactivate();
			okhandler();
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
	}
}