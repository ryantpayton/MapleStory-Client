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
#include "..\UIDragElement.h"
#include "..\Components\Icon.h"
#include "..\Components\EquipTooltip.h"

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

		enum Buttons
		{
			BT_TOGGLEPETS
		};

		UIEquipInventory();

		void draw(float inter) const override;
		void buttonpressed(uint16_t buttonid) override;
		void togglehide() override;
		void doubleclick(Point<int16_t> position) override;
		void sendicon(const Icon& icon, Point<int16_t> position) override;
		Cursor::State sendmouse(bool pressed, Point<int16_t> position) override;

		void modify(int16_t pos, int8_t mode, int16_t arg);

	private:
		void showequip(int16_t slot);
		void clear_tooltip();

		class EquipIcon : public Icon::Type
		{
		public:
			EquipIcon(int16_t source);

			void ondrop() const override;
			void ondropequips(Equipslot::Value) const override {}
			void ondropitems(Inventory::Type tab, Equipslot::Value eqslot, int16_t slot, bool equip) const override;

		private:
			int16_t source;
		};

		UIEquipInventory& operator = (const UIEquipInventory&) = delete;

		void loadicons();
		void updateslot(int16_t slot);
		int16_t slotbypos(Point<int16_t> position) const;
		Optional<Icon> geticon(int16_t slot) const;

		const Inventory& inventory;

		std::map<int16_t, Point<int16_t>> iconpositions;
		std::map<int16_t, std::unique_ptr<Icon>> icons;
		std::vector<Texture> pettextures;

		bool showpetequips;
	};
}