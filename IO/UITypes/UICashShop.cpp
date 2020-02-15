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
#include "UICashShop.h"

#include "../UI.h"
#include "../Window.h"

#include "../Components/AreaButton.h"
#include "../Components/MapleButton.h"
#include "../Components/TwoSpriteButton.h"

#include "../../Data/ItemData.h"
#include "../../Gameplay/Stage.h"

#include "../../Net/Packets/GameplayPackets.h"
#include "../../Net/Packets/LoginPackets.h"

#include <nlnx/nx.hpp>

namespace ms
{
	UICashShop::UICashShop() : show_preview(true)
	{
		nl::node CashShopGL = nl::nx::ui["CashShopGL.img"];
		nl::node BaseFrame = CashShopGL["BaseFrame"];
		nl::node CSItemSearch = BaseFrame["CSItemSearch"];
		nl::node Home = CashShopGL["MainMenu"]["Home"];
		nl::node RightMenu = CashShopGL["RightMenu"];
		nl::node CartInven = RightMenu["CartInven"];
		nl::node CashInven = RightMenu["CashInven"];
		nl::node ItemInven = RightMenu["ItemInven"];
		nl::node Preview = RightMenu["Preview"];
		nl::node SpecialSale = RightMenu["SpecialSale"];

		nl::node background = BaseFrame["background01"];
		nl::node preview_background = Preview["GameBackground"];
		nl::node search_background = CSItemSearch["background"];

		Point<int16_t> MiddleMenuPos = Point<int16_t>(179, 24);

		Point<int16_t> RightMenuPos = MiddleMenuPos + Point<int16_t>(586, 6);
		preview_position = RightMenuPos + Point<int16_t>(4, 114);
		Point<int16_t> CartInvenPos = preview_position + Point<int16_t>(0, 240);
		CashInvenPos = CartInvenPos + Point<int16_t>(0, 102);
		Point<int16_t> ItemInvenPos = CashInvenPos + Point<int16_t>(0, 104);

		sprites.emplace_back(background);
		sprites.emplace_back(Home["background"], MiddleMenuPos);
		sprites.emplace_back(preview_background, preview_position);
		sprites.emplace_back(SpecialSale["background3"], RightMenuPos);
		sprites.emplace_back(SpecialSale["lineEffect"], RightMenuPos);

		main_items.push_back({ 5220000, 11, 11000, 0, 539 });
		main_items.push_back({ 5240006, 0, 21000, 14600, 1 });
		main_items.push_back({ 5140000, 0, 25000, 7400, 0 });

		CSItemSearchSprites[true] = CSItemSearch["background2"];
		CSItemSearchSprites[false] = search_background;

		preview_sprites[true] = Preview["On"];
		preview_sprites[false] = Preview["Off"];

		buttons[Buttons::BtCoupon] = std::make_unique<MapleButton>(BaseFrame["BtCoupon"]);
		buttons[Buttons::BtExit] = std::make_unique<MapleButton>(BaseFrame["BtExit"]);
		buttons[Buttons::BtHelp] = std::make_unique<MapleButton>(BaseFrame["BtHelp"]);
		buttons[Buttons::BtHome] = std::make_unique<MapleButton>(BaseFrame["BtHome"]);
		buttons[Buttons::BtGo] = std::make_unique<MapleButton>(CSItemSearch["BtGo"]);
		buttons[Buttons::BtBuyAll] = std::make_unique<MapleButton>(CartInven["BtBuyAll"], CartInvenPos);
		buttons[Buttons::BtGiftAll] = std::make_unique<MapleButton>(CartInven["BtGiftAll"], CartInvenPos);
		buttons[Buttons::BtEnable] = std::make_unique<MapleButton>(CashInven["BtEnable"], CashInvenPos);
		buttons[Buttons::BtAddEquip] = std::make_unique<MapleButton>(ItemInven["BtAddEquip"], ItemInvenPos);
		buttons[Buttons::BtAddEtc] = std::make_unique<MapleButton>(ItemInven["BtAddEtc"], ItemInvenPos);
		buttons[Buttons::BtAddSetup] = std::make_unique<MapleButton>(ItemInven["BtAddSetup"], ItemInvenPos);
		buttons[Buttons::BtAddStor] = std::make_unique<MapleButton>(ItemInven["BtAddStor"], ItemInvenPos);
		buttons[Buttons::BtAddUse] = std::make_unique<MapleButton>(ItemInven["BtAddUse"], ItemInvenPos);

		for (size_t i = Buttons::BtItemInvenTab0; i < Buttons::BtPreview; i++)
		{
			auto wz_index = i - Buttons::BtItemInvenTab0;
			auto x_adj = (i != Buttons::BtItemInvenTab3) ? -1 : 0;
			auto pos = ItemInvenPos + Point<int16_t>(x_adj, -1);

			buttons[i] = std::make_unique<TwoSpriteButton>(ItemInven["Tab"]["disabled"][wz_index], ItemInven["Tab"]["enabled"][wz_index], pos, pos + Point<int16_t>(0, -2));
		}

		// Set equip tab to be the default
		buttons[Buttons::BtItemInvenTab0]->set_state(Button::State::PRESSED);

		buttons[Buttons::BtPreview] = std::make_unique<AreaButton>(preview_position, Texture(preview_background).get_dimensions());
		buttons[Buttons::BtByEquipAll] = std::make_unique<MapleButton>(Preview["BtByEquipAll"], preview_position);
		buttons[Buttons::BtRollBack] = std::make_unique<MapleButton>(Preview["BtRollBack"], preview_position);
		buttons[Buttons::BtUnEquip] = std::make_unique<MapleButton>(Preview["BtUnEquip"], preview_position);

		auto item_search_rect = Rectangle<int16_t>(preview_position, preview_position + Texture(search_background).get_dimensions());
		item_search = Textfield(Text::Font::A11M, Text::Alignment::LEFT, Color::Name::BLACK, item_search_rect, 30);

		CashInvenText = Text(Text::Font::A11M, Text::Alignment::LEFT, Color::Name::WHITE, "Assassin");

		dimension = Texture(background).get_dimensions();
	}

	void UICashShop::draw(float inter) const
	{
		UIElement::draw_sprites(inter);

		for (size_t i = 0; i < 3; i++)
		{
			auto main_item = ItemData::get(main_items[i].itemid);
			main_item.get_icon(true).draw(position);
		}

		preview_sprites[show_preview].draw(position + preview_position, inter);

		CashInvenText.draw(position + CashInvenPos + Point<int16_t>(83, 1));

		item_search.draw(position);

		auto CSItemSearchEnabled = item_search.get_state() == Textfield::State::NORMAL && item_search.empty();
		CSItemSearchSprites[CSItemSearchEnabled].draw(position, inter);

		UIElement::draw_buttons(inter);
	}

	Button::State UICashShop::button_pressed(uint16_t buttonid)
	{
		switch (buttonid)
		{
		case Buttons::BtPreview:
			show_preview = !show_preview;

			return Button::State::NORMAL;
		case Buttons::BtExit:
		{
			uint16_t width = Setting<Width>::get().load();
			uint16_t height = Setting<Height>::get().load();

			Constants::Constants::get().set_viewwidth(width);
			Constants::Constants::get().set_viewheight(height);

			float fadestep = 0.025f;

			Window::get().fadeout(
				fadestep,
				[]()
				{
					GraphicsGL::get().clear();
					ChangeMapPacket().dispatch();
				}
			);

			GraphicsGL::get().lock();
			Stage::get().clear();
			Timer::get().start();

			return Button::State::NORMAL;
		}
		default:
			break;
		}

		return Button::State::DISABLED;
	}

	UIElement::Type UICashShop::get_type() const
	{
		return TYPE;
	}

	void UICashShop::exit_cashshop()
	{
		UI& ui = UI::get();
		ui.change_state(UI::State::GAME);

		Stage& stage = Stage::get();
		Player& player = stage.get_player();

		PlayerLoginPacket(player.get_oid()).dispatch();

		int32_t mapid = player.get_stats().get_mapid();
		uint8_t portalid = player.get_stats().get_portal();

		stage.load(mapid, portalid);
		stage.transfer_player();

		ui.enable();
		Timer::get().start();
		GraphicsGL::get().unlock();
	}
}