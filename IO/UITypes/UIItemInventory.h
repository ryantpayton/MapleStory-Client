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
#include "..\UIDragElement.h"

#include "..\Components\Icon.h"
#include "..\Components\EquipTooltip.h"
#include "..\Components\ItemTooltip.h"
#include "..\Components\Slider.h"

#include "..\..\Character\Inventory\Inventory.h"

namespace jrc
{
	// The Item inventory.
	class UIItemInventory : public UIDragElement<PosINV>
	{
	public:
		static constexpr Type TYPE = ITEMINVENTORY;
		static constexpr bool FOCUSED = false;
		static constexpr bool TOGGLED = true;

		UIItemInventory();

		void draw(float inter) const override;
		void update() override;

		void doubleclick(Point<int16_t> position) override;
		void send_icon(const Icon& icon, Point<int16_t> position) override;
		void toggle_active() override;
		bool remove_cursor(bool clicked, Point<int16_t> cursorpos) override;
		Cursor::State send_cursor(bool pressed, Point<int16_t> position) override;

		void modify(Inventory::Type type, int16_t pos, int8_t mode, int16_t arg);
		void enable_sort();
		void enable_gather();

	protected:
		Button::State button_pressed(uint16_t buttonid) override;

	private:
		void show_item(int16_t slot);
		void clear_tooltip();
		void load_icons();
		void update_slot(int16_t slot);
		bool is_visible(int16_t slot) const;
		bool is_not_visible(int16_t slot) const;
		int16_t slot_by_position(Point<int16_t> position) const;
		uint16_t button_by_tab(Inventory::Type tab) const;
		Point<int16_t> get_slotpos(int16_t slot) const;
		Point<int16_t> get_tabpos(Inventory::Type tab) const;
		Optional<Icon> geticon(int16_t slot) const;

		class ItemIcon : public Icon::Type
		{
		public:
			ItemIcon(Inventory::Type sourcetab, Equipslot::Value eqsource, int16_t source);

			void drop_on_stage() const override;
			void drop_on_equips(Equipslot::Value eqslot) const override;
			void drop_on_items(Inventory::Type tab, Equipslot::Value, int16_t slot, bool) const override;

		private:
			Inventory::Type sourcetab;
			Equipslot::Value eqsource;
			int16_t source;
		};

		enum Buttons
		{
			BT_TAB_EQUIP,
			BT_TAB_USE,
			BT_TAB_ETC,
			BT_TAB_SETUP,
			BT_TAB_CASH,
			BT_DROPMESO,
			BT_POINTS,
			BT_GATHER,
			BT_SORT,
			BT_EXPAND,
			BT_ITEMPOT,
			BT_UPGRADE,
			BT_MAGNIFY,
			BT_BITCASE
		};

		const Inventory& inventory;

		Animation newitemslot;
		Animation newitemtab;
		Texture projectile;
		Text mesolabel;
		Slider slider;

		std::map<int16_t, std::unique_ptr<Icon>> icons;

		Inventory::Type tab;
		std::pair<int16_t, int16_t> slotrange;
		Inventory::Type newtab;
		int16_t newslot;
	};
}