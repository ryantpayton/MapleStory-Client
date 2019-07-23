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

#include "../Graphics/Text.h"

#include <deque>

namespace jrc
{
	class StatusInfo
	{
	public:
		StatusInfo(const std::string& str, Color::Name color);

		void draw(Point<int16_t> position, float alpha) const;
		bool update();

	private:
		Text text;
		Text shadow;
		Linear<float> opacity;

		// 8 seconds.
		static constexpr int64_t FADE_DURATION = 8'000;
	};


	class UIStatusMessenger : public UIElement
	{
	public:
		static constexpr Type TYPE = UIElement::Type::STATUSMESSENGER;
		static constexpr bool FOCUSED = false;
		static constexpr bool TOGGLED = false;

		UIStatusMessenger();

		void draw(float alpha) const override;
		void update() override;

		void show_status(Color::Name color, const std::string& message);

	private:
		static constexpr size_t MAX_MESSAGES = 5;

		std::deque<StatusInfo> statusinfos;
	};
}