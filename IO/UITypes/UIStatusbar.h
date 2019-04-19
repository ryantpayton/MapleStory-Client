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

#include "../Components/Charset.h"
#include "../Components/Gauge.h"
#include "../Components/Textfield.h"

#include "../../Character/CharStats.h"
#include "../../Character/Inventory/Inventory.h"
#include "../../Character/Job.h"
#include "../../Graphics/Animation.h"
#include "../../Graphics/Text.h"
#include "../../Graphics/OutlinedText.h"

namespace jrc
{
	class UIStatusbar : public UIElement
	{
	public:
		static constexpr Type TYPE = STATUSBAR;
		static constexpr bool FOCUSED = false;
		static constexpr bool TOGGLED = true;

		UIStatusbar(const CharStats& stats);

		void draw(float alpha) const override;
		void update() override;

		bool is_in_range(Point<int16_t> cursorpos) const override;

		void toggle_qs();

	protected:
		Button::State button_pressed(uint16_t buttonid) override;

	private:
		float getexppercent() const;
		float gethppercent() const;
		float getmppercent() const;

		enum Buttons : uint16_t
		{
			BT_CASHSHOP,
			BT_MENU,
			BT_OPTIONS,
			BT_CHARACTER,
			BT_COMMUNITY,
			BT_EVENT,
			BT_FOLD_QS,
			BT_EXTEND_QS
		};

		const CharStats& stats;

		Gauge expbar;
		Gauge hpbar;
		Gauge mpbar;
		Charset statset;
		Charset hpmpset;
		Charset levelset;
		Texture quickslot[2];
		OutlinedText namelabel;
		std::vector<Sprite> hpmp_sprites;

		Point<int16_t> exp_pos;
		Point<int16_t> hpmp_pos;
		Point<int16_t> quickslot_pos;

		bool quickslot_active;
	};
}