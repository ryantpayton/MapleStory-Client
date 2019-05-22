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
#include "UILoginwait.h"

#include "../Components/MapleButton.h"
#include "../Net/Session.h"

#include "nlnx/nx.hpp"

namespace jrc
{
	UILoginwait::UILoginwait(std::function<void()> okhandler) : okhandler(okhandler)
	{
		nl::node Loading = nl::nx::ui["Login.img"]["Notice"]["Loading"];
		nl::node backgrnd = Loading["backgrnd"];

		sprites.emplace_back(backgrnd);
		sprites.emplace_back(Loading["circle"], Point<int16_t>(127, 70));

		buttons[Buttons::CANCEL] = std::make_unique<MapleButton>(Loading["BtCancel"], Point<int16_t>(101, 106));

		position = Point<int16_t>(276, 219);
		dimension = Texture(backgrnd).get_dimensions();
		active = true;
	}

	UILoginwait::UILoginwait() : UILoginwait([]() {}) {}

	void UILoginwait::draw(float alpha) const
	{
		UIElement::draw(alpha);
	}

	void UILoginwait::update()
	{
		UIElement::update();
	}

	void UILoginwait::close()
	{
		active = false;

		okhandler();
	}

	std::function<void()> UILoginwait::get_handler()
	{
		return okhandler;
	}

	Button::State UILoginwait::button_pressed(uint16_t id)
	{
		Session::get().reconnect();

		close();

		return Button::State::NORMAL;
	}
}