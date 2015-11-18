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
#include "IO\Components\Charset.h"
#include "IO\Components\Bar.h"
#include "Character\Charstats.h"
#include "Graphics\Animation.h"
#include "Graphics\Textlabel.h"

namespace IO
{
	using Character::Charstats;
	using Graphics::Textlabel;
	using Graphics::Animation;

	class UIStatusbar : public UIElement
	{
	public:
		enum Buttons
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

		UIStatusbar(const Charstats&);

		void draw(float) const override;
		void buttonpressed(uint16_t) override;
		rectangle2d<int16_t> bounds() const override;

	private:
		UIStatusbar& operator = (const UIStatusbar&) = delete;

		const Charstats& stats;

		Bar expbar;
		Bar hpbar;
		Bar mpbar;
		Charset statset;
		Charset levelset;
		Textlabel namelabel;
		Textlabel joblabel;
		Animation hpanimation;
		Animation mpanimation;
	};

	class ElementStatusbar : public Element
	{
	public:
		ElementStatusbar(const Charstats& st) : stats(st) {}

		bool isunique() const override
		{
			return true;
		}

		UIType type() const override
		{
			return STATUSBAR;
		}

		UIElement* instantiate() const override
		{
			return new UIStatusbar(stats);
		}

	private:
		ElementStatusbar& operator = (const ElementStatusbar&) = delete;

		const Charstats& stats;
	};
}