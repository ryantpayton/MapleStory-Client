//////////////////////////////////////////////////////////////////////////////////
//	This file is part of the continued Journey MMORPG client					//
//	Copyright (C) 2015-2019  Daniel Allendorf, Ryan Payton						//
//																				//
//	This program is free software: you can redistribute it and/or modify		//
//	it under the terms of the GNU Affero General Public License as published by	//
//	the Free Software Foundation, either version 3 of the License, or			//
//	(at your option) any later version.											//
//																				//
//	This program is distributed in the hope that it will be useful,				//
//	but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				//
//	GNU Affero General Public License for more details.							//
//																				//
//	You should have received a copy of the GNU Affero General Public License	//
//	along with this program.  If not, see <https://www.gnu.org/licenses/>.		//
//////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "../UIDragElement.h"

#include "../Components/Slider.h"
#include "../../Graphics/Text.h"

#include "../../Character/Look/CharLook.h"

namespace ms
{
	class UIShop : public UIDragElement<PosSHOP>
	{
	public:
		static constexpr Type TYPE = UIElement::Type::SHOP;
		static constexpr bool FOCUSED = true;
		static constexpr bool TOGGLED = true;

		UIShop(const CharLook& charlook, const Inventory& inventory);

		void draw(float alpha) const override;
		void update() override;

		bool remove_cursor(bool clicked, Point<int16_t> cursorpos) override;
		Cursor::State send_cursor(bool clicked, Point<int16_t> position) override;
		void send_scroll(double yoffset) override;
		void rightclick(Point<int16_t> cursorpos) override;
		void send_key(std::int32_t keycode, bool pressed, bool escape) override;

		void reset(std::int32_t npcid);
		void add_item(std::int32_t id, std::int32_t price, std::int32_t pitch, std::int32_t time, std::int16_t buyable);
		void add_rechargable(std::int32_t id, std::int32_t price, std::int32_t pitch, std::int32_t time, std::int16_t chargeprice, std::int16_t buyable);

		void modify(InventoryType::Id type);

	protected:
		Button::State button_pressed(std::uint16_t buttonid) override;

	private:
		void clear_tooltip();
		void show_item(std::int16_t slot, bool sale);
		void changeselltab(InventoryType::Id tab);
		std::int16_t slot_by_position(std::int16_t y);
		std::uint16_t tabbyinventory(InventoryType::Id type);
		void exit_shop();

		enum Buttons : std::int16_t
		{
			BUY_ITEM,
			SELL_ITEM,
			EXIT,
			CHECKBOX,
			OVERALL,
			EQUIP,
			USE,
			ETC,
			SETUP,
			CASH,
			BUY0,
			BUY1,
			BUY2,
			BUY3,
			BUY4,
			BUY5,
			BUY6,
			BUY7,
			BUY8,
			SELL0,
			SELL1,
			SELL2,
			SELL3,
			SELL4,
			SELL5,
			SELL6,
			SELL7,
			SELL8,
			NUM_BUTTONS
		};

		const CharLook& charlook;
		const Inventory& inventory;

		Texture npc;
		Texture buy_selection;
		Texture sell_selection;
		Texture meso;
		Text mesolabel;

		Slider buyslider;
		Slider sellslider;

		std::int16_t buy_x;
		std::int16_t buy_width;
		std::int16_t sell_x;
		std::int16_t sell_width;

		BoolPair<Texture> checkBox;

		bool rightclicksell;

		Point<int16_t> lastcursorpos;

		class BuyItem
		{
		public:
			BuyItem(Texture cur, std::int32_t i, std::int32_t p, std::int32_t pt, std::int32_t t, std::int16_t cp, std::int16_t b);

			void draw(Point<int16_t> position) const;

			std::int32_t get_id() const;
			std::int16_t get_buyable() const;

		private:
			Texture icon;
			Texture currency;
			std::int32_t id;
			std::int32_t price;
			std::int32_t pitch;
			std::int32_t time;
			std::int16_t chargeprice;
			std::int16_t buyable;
			Text namelabel;
			Text pricelabel;
		};

		class SellItem
		{
		public:
			SellItem(std::int32_t item_id, std::int16_t count, std::int16_t slot, bool showcount, Texture cur);

			void draw(Point<int16_t> position) const;

			std::int32_t get_id() const;
			std::int16_t get_slot() const;
			std::int16_t get_sellable() const;

		private:
			Texture icon;
			std::int32_t id;
			std::int16_t slot;
			std::int16_t sellable;
			bool showcount;
			Text namelabel;
			Text pricelabel;
		};

		struct BuyState
		{
			std::vector<BuyItem> items;
			std::int16_t offset;
			std::int16_t lastslot;
			std::int16_t selection;

			void reset();
			void draw(Point<int16_t> position, const Texture& selected) const;
			void show_item(std::int16_t slot);
			void add(BuyItem item);
			void buy() const;
			void select(std::int16_t selected);
		};

		BuyState buystate;

		struct SellState
		{
			std::vector<SellItem> items;
			std::int16_t offset;
			InventoryType::Id tab;
			std::int16_t lastslot;
			std::int16_t selection;

			void reset();
			void change_tab(const Inventory& inventory, InventoryType::Id type, Texture meso);
			void draw(Point<int16_t> position, const Texture& selected) const;
			void show_item(std::int16_t slot);
			void sell(bool skip_confirmation) const;
			void select(std::int16_t selected);
		};

		SellState sellstate;
	};
}