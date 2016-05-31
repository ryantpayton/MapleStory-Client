/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015 Daniel Allendorf                                        //
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
#include "..\Element.h"
#include "..\Messages.h"
#include "..\Components\Chatbar.h"
#include "..\Components\Charset.h"
#include "..\Components\Gauge.h"
#include "..\Components\Textfield.h"

#include "..\..\Character\CharStats.h"
#include "..\..\Graphics\Animation.h"
#include "..\..\Graphics\Text.h"

namespace jrc
{
	class UIStatusbar : public UIElement
	{
	public:
		static constexpr Type TYPE = STATUSBAR;
		static constexpr bool FOCUSED = false;
		static constexpr bool TOGGLED = true;

		enum Buttons : uint16_t
		{
			BT_WHISPER,
			BT_CALLGM,
			BT_CASHSHOP,
			BT_TRADE,
			BT_MENU,
			BT_OPTIONS,
			BT_CHARACTER,
			BT_STATS,
			BT_QUEST,
			BT_INVENTORY,
			BT_EQUIPS,
			BT_SKILL
		};

		UIStatusbar();

		void draw(float inter) const override;
		void update() override;
		void buttonpressed(uint16_t buttonid) override;
		bool isinrange(Point<int16_t> cursorpos) const override;
		Cursor::State sendmouse(bool pressed, Point<int16_t> cursorpos) override;

		void sendchatline(const std::string& line, Chatbar::LineType type);
		void displaymessage(Messages::Type line, Chatbar::LineType type);

	private:
		float getexppercent() const;
		float gethppercent() const;
		float getmppercent() const;

		static constexpr Point<int16_t> POSITION = { 512, 590 };
		static constexpr Point<int16_t> DIMENSION = { 1366, 80 };

		const CharStats& stats;

		Messages messages;
		EnumMap<Messages::Type, time_t> message_cooldowns;
		static constexpr time_t MESSAGE_COOLDOWN = 1'000;

		Chatbar chatbar;
		Gauge expbar;
		Gauge hpbar;
		Gauge mpbar;
		Charset statset;
		Charset levelset;
		Text namelabel;
		Text joblabel;
		Animation hpanimation;
		Animation mpanimation;
	};
}