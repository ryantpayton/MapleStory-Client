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

#include "../UIElement.h"

#include "../Components/Textfield.h"

#include "../../Template/BoolPair.h"

namespace ms
{
	class UICashShop : public UIElement
	{
	public:
		static constexpr Type TYPE = UIElement::Type::CASHSHOP;
		static constexpr bool FOCUSED = true;
		static constexpr bool TOGGLED = false;

		UICashShop();

		void draw(float inter) const;

		Button::State button_pressed(uint16_t buttonid);

		UIElement::Type get_type() const override;

		void exit_cashshop();

	private:
		struct MainItem
		{
			int32_t itemid;
			int32_t count;
			int32_t price;
			int32_t discount;
			int32_t likes;
		};

		enum Buttons : uint16_t
		{
			BtCoupon,
			BtExit,
			BtHelp,
			BtHome,
			BtGo,
			BtBuyAll,
			BtGiftAll,
			BtEnable,
			BtAddEquip,
			BtAddEtc,
			BtAddSetup,
			BtAddStor,
			BtAddUse,
			BtItemInvenTab0,
			BtItemInvenTab1,
			BtItemInvenTab2,
			BtItemInvenTab3,
			BtItemInvenTab4,
			BtPreview,
			BtByEquipAll,
			BtRollBack,
			BtUnEquip
		};

		std::vector<MainItem> main_items;

		Textfield item_search;
		BoolPair<Sprite> CSItemSearchSprites;

		Text CashInvenText;
		Point<int16_t> CashInvenPos;

		bool show_preview;
		Point<int16_t> preview_position;
		BoolPair<Sprite> preview_sprites;
	};
}