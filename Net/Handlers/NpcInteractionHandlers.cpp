/////////////////////////////////////////////////////////////////////////////
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
#include "NpcInteractionHandlers.h"

#include "IO\UI.h"
#include "IO\UITypes\UINpcTalk.h"
#include "IO\UITypes\UIShop.h"

namespace Net
{
	using IO::UI;
	using IO::UIElement;
	using IO::UINpcTalk;
	using IO::UIShop;

	void NpcDialogueHandler::handle(InPacket& recv) const
	{
		recv.skip(1);

		int32_t npcid = recv.readint();
		int8_t msgtype = recv.readbyte(); //0 - textonly, 1 - yes/no, 4 - selection, 12 - accept/decline
		int8_t speaker = recv.readbyte();
		string text = recv.readascii();

		int16_t style = 0;
		if (msgtype == 0 && recv.length() > 0)
			style = recv.readshort();

		UI::get().withelement(UIElement::NPCTALK, &UINpcTalk::settext, npcid, msgtype, style, speaker, text);
		UI::get().enable();
	}


	void OpenNpcShopHandler::handle(InPacket& recv) const
	{
		int32_t npcid = recv.readint();
		UI::get().withelement(UIElement::SHOP, &UIShop::reset, npcid);

		int16_t size = recv.readshort();
		for (int16_t i = 0; i < size; i++)
		{
			int32_t itemid = recv.readint();
			int32_t price = recv.readint();
			int32_t pitch = recv.readint();
			int32_t time = recv.readint();

			recv.skip(4);

			bool norecharge = recv.readshort() == 1;
			if (norecharge)
			{
				int16_t buyable = recv.readshort();

				UI::get().withelement(UIElement::SHOP, &UIShop::additem, itemid, price, pitch, time, buyable);
			}
			else
			{
				recv.skip(4);

				int16_t rechargeprice = recv.readshort();
				int16_t slotmax = recv.readshort();

				UI::get().withelement(UIElement::SHOP, &UIShop::addrechargable, itemid, price, pitch, time, rechargeprice, slotmax);
			}
		}
	}
}