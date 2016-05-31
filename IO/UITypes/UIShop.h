//////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2016 Daniel Allendorf                                        //
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
#include "..\Components\Slider.h"

#include "..\..\Character\Inventory\Inventory.h"
#include "..\..\Graphics\Text.h"
#include "..\..\Graphics\Texture.h"

namespace jrc
{
	class UIShop : public UIElement
	{
	public:
		static constexpr Type TYPE = SHOP;
		static constexpr bool FOCUSED = true;
		static constexpr bool TOGGLED = true;

		UIShop();

		void draw(float alpha) const override;
		void update() override;
		Cursor::State sendmouse(bool clicked, Point<int16_t> position) override;

		void reset(int32_t npcid);
		void additem(int32_t id, int32_t price, 
			int32_t pitch, int32_t time, int16_t buyable);
		void addrechargable(int32_t id, int32_t price, int32_t pitch, 
			int32_t time, int16_t chargeprice, int16_t buyable);

		void modify(Inventory::Type type);

	protected:
		enum Buttons : int16_t
		{
			BUY_ITEM = 0,
			SELL_ITEM = 1,
			EXIT = 2,
			EQUIP = 3,
			USE = 4,
			ETC = 5,
			SETUP = 6,
			CASH = 7,
			BUY0 = 8,
			BUY4 = 12,
			SELL0 = 13,
			SELL4 = 17
		};

		void buttonpressed(uint16_t buttonid) override;

	private:
		void clear_tooltip();
		void showitem(int16_t slot, bool sale);
		void changeselltab(Inventory::Type tab);
		int16_t slotbypos(int16_t y);
		uint16_t tabbyinventory(Inventory::Type type);

		Texture npc;
		Texture selection;
		Texture impossible;
		Texture meso;
		Text mesolabel;

		Slider buyslider;
		Slider sellslider;

		class BuyItem
		{
		public:
			BuyItem(Texture cur, int32_t i, int32_t p, int32_t pt, int32_t t, int16_t cp, int16_t b);

			void draw(Point<int16_t> position) const;

			int32_t getid() const;
			int16_t getbuyable() const;

		private:
			Texture icon;
			Texture currency;
			int32_t id;
			int32_t price;
			int32_t pitch;
			int32_t time;
			int16_t chargeprice;
			int16_t buyable;
			Text namelabel;
			Text pricelabel;
		};

		class SellItem
		{
		public:
			SellItem(const Item& item, int16_t slot, bool showcount, Texture cur);

			void draw(Point<int16_t> position) const;

			int32_t getid() const;
			int16_t getslot() const;
			int16_t getsellable() const;

		private:
			Texture icon;
			Texture currency;
			int32_t id;
			int16_t slot;
			int16_t sellable;
			bool showcount;
			Text namelabel;
			Text pricelabel;
		};

		struct BuyState
		{
			std::vector<BuyItem> items;
			int16_t offset;
			int16_t lastslot;
			int16_t selection;

			void reset();
			void draw(Point<int16_t> position, const Texture& selected) const;
			void showitem(int16_t slot);
			void add(BuyItem item);
			void buy() const;
			void select(int16_t selected);
		};
		BuyState buystate;

		struct SellState
		{
			std::vector<SellItem> items;
			int16_t offset;
			Inventory::Type tab;
			int16_t lastslot;
			int16_t selection;

			void reset();
			void changetab(Inventory::Type type, Texture meso);
			void draw(Point<int16_t> position, const Texture& selected) const;
			void showitem(int16_t slot);
			void sell() const;
			void select(int16_t selected);
		};
		SellState sellstate;
	};
}