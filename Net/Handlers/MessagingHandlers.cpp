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
#include "MessagingHandlers.h"
#include "Character\Char.h"
#include "Gameplay\Stage.h"
#include "IO\UI.h"
#include "IO\UITypes\UIStatusMessenger.h"
#include "IO\UITypes\UIStatusbar.h"
#include "IO\UITypes\UINpcTalk.h"
#include "Data\DataFactory.h"

namespace Net
{
	using IO::UI;
	using IO::UIElement;
	using IO::UIStatusMessenger;
	using IO::UIStatusbar;
	using IO::UINpcTalk;
	using Character::ItemData;
	using Character::Char;
	using Data::DataFactory;
	using Gameplay::Stage;

	// Modes:
	// 0 - Item(0) or Meso(1) 
	// 3 - Exp gain
	// 4 - Fame
	// 5 - Mesos
	// 6 - Guild points
	void ShowStatusInfoHandler::handle(InPacket& recv) const
	{
		int8_t mode = recv.readbyte();
		if (mode == 0)
		{
			int8_t mode2 = recv.readbyte();
			if (mode2 == 0)
			{
				int32_t itemid = recv.readint();
				int32_t qty = recv.readint();

				const ItemData& idata = DataFactory::get().getitemdata(itemid);
				if (!idata.isloaded())
					return;

				string name = idata.getname();
				string sign = (qty < 0) ? "-" : "+";

				showstatus(Text::WHITE, "Gained an item: " + name + " (" + sign + std::to_string(qty) + ")");
			}
			else if (mode2 == 1)
			{
				recv.skip(1);

				int32_t gain = recv.readint();
				string sign = (gain < 0) ? "-" : "+";

				showstatus(Text::WHITE, "Received mesos (" + sign + std::to_string(gain) + ")");
			}
		}
		else if (mode == 3)
		{
			bool white = recv.readbool();
			int32_t gain = recv.readint();
			bool inchat = recv.readbool();
			int32_t bonus1 = recv.readint();

			recv.readshort();
			recv.readint(); // bonus 2
			recv.readbool(); // 'event or party'
			recv.readint(); // bonus 3
			recv.readint(); // bonus 4
			recv.readint(); // bonus 5

			string message = "You have gained experience (+" + std::to_string(gain) + ")";
			if (inchat)
			{

			}
			else
			{
				showstatus(white ? Text::WHITE : Text::YELLOW, message);
				if (bonus1 > 0)
					showstatus(Text::YELLOW, "+ Bonus EXP (+" + std::to_string(bonus1) + ")");
			}
		}
		else if (mode == 4)
		{
			int32_t gain = recv.readint();
			string sign = (gain < 0) ? "-" : "+";

			showstatus(Text::WHITE, "Received fame (" + sign + std::to_string(gain) + ")");
		}
		else if (mode == 5)
		{
		}
	}

	void ShowStatusInfoHandler::showstatus(Text::Color color, string message) const
	{
		UI::get().withelement(UIElement::STATUSMESSENGER, &UIStatusMessenger::showstatus, color, message);
	}

	void ChatReceivedHandler::handle(InPacket& recv) const
	{
		int32_t charid = recv.readint();
		recv.readbool(); // 'gm'
		string message = recv.readascii();
		int8_t type = recv.readbyte();

		Optional<Char> speaker = Stage::get().getcharacter(charid);
		if (speaker)
		{
			string fulltext = speaker->getname() + ": " + message;
			speaker->speak(fulltext);

			UI::get().withelement(UIElement::STATUSBAR, &UIStatusbar::sendchatline, fulltext, type);
		}
	}

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
}