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
#include "CashShopHandlers.h"

#include "Helpers/CashShopParser.h"
#include "Helpers/ItemParser.h"

#include "../../Gameplay/Stage.h"
#include "../../IO/UI.h"
#include "../../IO/Window.h"

namespace ms
{
	void SetCashShopHandler::handle(InPacket& recv) const
	{
		CashShopParser::parseCharacterInfo(recv);

		recv.skip_byte();	// Not MTS
		recv.skip_string();	// account_name
		recv.skip_int();

		int16_t specialcashitem_size = recv.read_short();

		for (size_t i = 0; i < specialcashitem_size; i++)
		{
			recv.skip_int();	// sn
			recv.skip_int();	// mod
			recv.skip_byte();	// info
		}

		recv.skip(121);

		for (size_t cat = 1; cat <= 8; cat++)
		{
			for (size_t gender = 0; gender < 2; gender++)
			{
				for (size_t in = 0; in < 5; in++)
				{
					recv.skip_int(); // category
					recv.skip_int(); // gender
					recv.skip_int(); // commoditysn
				}
			}
		}

		recv.skip_int();
		recv.skip_short();
		recv.skip_byte();
		recv.skip_int();

		transition();

		UI::get().change_state(UI::State::CASHSHOP);
	}

	void SetCashShopHandler::transition() const
	{
		Constants::Constants::get().set_viewwidth(1024);
		Constants::Constants::get().set_viewheight(768);

		float fadestep = 0.025f;

		Window::get().fadeout(
			fadestep,
			[]()
			{
				GraphicsGL::get().clear();

				Stage::get().load(-1, 0);

				UI::get().enable();
				Timer::get().start();
				GraphicsGL::get().unlock();
			}
		);

		GraphicsGL::get().lock();
		Stage::get().clear();
		Timer::get().start();
	}

	void QueryCashResultHandler::handle(InPacket& recv) const
	{
		recv.skip_int(); // Cash 1
		recv.skip_int(); // Cash 2
		recv.skip_int(); // Cash 4
	}

	void CashShopOperationHandler::handle(InPacket& recv) const
	{
		int8_t type = recv.read_byte();

		switch (type)
		{
		case 0x49:
			// showCouponRedeemedItem
			recv.skip_int();
			recv.skip_int();
			recv.skip_short();
			recv.skip_short();
			recv.skip_int(); // itemid
			recv.skip_int();
			break;
		case 0x89:
		{
			// showBoughtCashPackage
			int8_t length = recv.read_byte();

			for (int8_t i = 0; i < length; i++)
				parseCashItemInformation(recv);

			recv.skip_short();
			break;
		}
		case 0x8D:
			// showBoughtQuestItem
			recv.skip_int();
			recv.skip_short();
			recv.skip_byte();
			recv.skip_byte();
			recv.skip_int(); // itemId
			break;
		case 0x55:
		case 0x4F:
		{
			int32_t length = 1;

			while (recv.read_int() != 0)
				length++;

			for (int32_t i = length; i < 10; i++)
				recv.skip_int();

			break;
		}
		case 0x57:
			// showBoughtCashItem
			parseCashItemInformation(recv);
			break;
		case 0x5C:
			// showCashShopMessage
			recv.skip_byte(); // message
			break;
		case 0x4B:
		{
			// showCashInventory
			int16_t length = recv.read_short();

			for (int16_t i = 0; i < length; i++)
				parseCashItemInformation(recv);

			recv.skip_short(); // storage slots
			recv.skip_short(); // character slots
			break;
		}
		case 0x4D:
		{
			// showGifts
			int16_t length = recv.read_short();

			for (int16_t i = 0; i < length; i++)
				parseCashItemInformation(recv);

			break;
		}
		case 0x5E:
			// showGiftSucceed
			recv.skip_string();	// to
			recv.skip_int();	// itemid
			recv.skip_short();	// count
			recv.skip_int();	// price
			break;
		case 0x60:
			// showBoughtInventorySlots
			recv.skip_byte();	// type
			recv.skip_short();	// slots
			break;
		case 0x62:
			// showBoughtStorageSlots
			recv.skip_short(); // slots
			break;
		case 0x64:
			// showBoughtCharacterSlot
			recv.skip_short(); // slots
			break;
		case 0x68:
		{
			// takeFromCashInventory
			InventoryType::Id inv = InventoryType::CASH;
			int16_t slot = recv.read_short(); // item position

			Player& player = Stage::get().get_player();

			ItemParser::parse_item(recv, inv, slot, player.get_inventory());
			break;
		}
		case 0x6A:
			// putIntoCashInventory
			parseCashItemInformation(recv);
			break;
		default:
			break;
		}
	}

	void CashShopOperationHandler::parseCashItemInformation(InPacket& recv) const
	{
		bool isGift = false;

		recv.skip_long(); // pet/ring id

		if (!isGift)
		{
			recv.skip_int(); // accountId
			recv.skip_int();
		}

		recv.skip_int(); // itemid

		if (!isGift)
		{
			recv.skip_int();	// sn
			recv.skip_short();	// qty
		}

		recv.skip_padded_string(13); // gift from

		if (isGift)
		{
			recv.skip_padded_string(13); // giftMessage
			return;
		}

		parseExpirationTime(recv);

		recv.skip_long();
	}

	void CashShopOperationHandler::parseExpirationTime(InPacket& recv) const
	{
		recv.skip_long(); // offset expiration time
	}
}