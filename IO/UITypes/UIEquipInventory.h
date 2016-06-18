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

#include "..\Components\EquipTooltip.h"
#include "..\Components\Icon.h"

#include "..\..\Character\Inventory\Inventory.h"

namespace jrc
{
	// The Equip inventory.
	class UIEquipInventory : public UIDragElement<PosEQINV>
	{
	public:
		static constexpr Type TYPE = EQUIPINVENTORY;
		static constexpr bool FOCUSED = false;
		static constexpr bool TOGGLED = true;

		UIEquipInventory();

		void draw(float inter) const override;

		void toggle_active() override;
		void doubleclick(Point<int16_t> position) override;
		void send_icon(const Icon& icon, Point<int16_t> position) override;
		Cursor::State send_cursor(bool pressed, Point<int16_t> position) override;

		void modify(int16_t pos, int8_t mode, int16_t arg);

	protected:
		Button::State button_pressed(uint16_t buttonid) override;

	private:
		void show_equip(int16_t slot);
		void clear_tooltip();
		void load_icons();
		void update_slot(int16_t slot);
		int16_t slot_by_position(Point<int16_t> position) const;
		Optional<Icon> geticon(int16_t slot) const;

		class EquipIcon : public Icon::Type
		{
		public:
			EquipIcon(int16_t source);

			void drop_on_stage() const override;
			void drop_on_equips(Equipslot::Value) const override {}
			void drop_on_items(Inventory::Type tab, Equipslot::Value eqslot, int16_t slot, bool equip) const override;

		private:
			int16_t source;
		};

		enum Buttons
		{
			BT_TOGGLEPETS
		};

		const Inventory& inventory;

		std::vector<Texture> textures_pet;
		std::map<int16_t, Point<int16_t>> iconpositions;
		std::map<int16_t, std::unique_ptr<Icon>> icons;

		bool showpetequips;
	};
}