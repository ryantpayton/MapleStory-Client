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
#include "IO\Element.h"
#include "IO\Messages.h"
#include "IO\Components\Chatbar.h"
#include "IO\Components\Charset.h"
#include "IO\Components\Bar.h"
#include "IO\Components\Textfield.h"
#include "Character\CharStats.h"
#include "Character\Inventory\Inventory.h"
#include "Graphics\Animation.h"
#include "Graphics\Text.h"

namespace IO
{
	using Character::CharStats;
	using Character::Inventory;
	using Graphics::Text;
	using Graphics::Animation;

	class UIStatusbar : public UIElement
	{
	public:
		enum Buttons : uint16_t
		{
			// Main
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
		rectangle2d<int16_t> bounds() const override;
		Cursor::State sendmouse(bool pressed, Point<int16_t> position) override;

		void sendchatline(string line, Chatbar::LineType type);
		void displaymessage(Messages::Type line, Chatbar::LineType type);

	private:
		UIStatusbar& operator = (const UIStatusbar&) = delete;

		float getexppercent() const;
		float gethppercent() const;
		float getmppercent() const;

		const CharStats& stats;

		Messages messages;

		unique_ptr<Chatbar> chatbar;
		Bar expbar;
		Bar hpbar;
		Bar mpbar;
		Charset statset;
		Charset levelset;
		Text namelabel;
		Text joblabel;
		Animation hpanimation;
		Animation mpanimation;
	};

	class ElementStatusbar : public Element
	{
		bool isunique() const override
		{
			return true;
		}

		UIElement::Type type() const override
		{
			return UIElement::STATUSBAR;
		}

		UIElement* instantiate() const override
		{
			return new UIStatusbar();
		}
	};
}