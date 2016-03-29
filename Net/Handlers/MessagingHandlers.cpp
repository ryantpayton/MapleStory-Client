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
#include "Data\DataFactory.h"
#include "Gameplay\Stage.h"
#include "IO\UI.h"
#include "IO\Messages.h"
#include "IO\UITypes\UIStatusMessenger.h"
#include "IO\UITypes\UIStatusbar.h"

namespace Net
{
	using Character::ItemData;
	using Character::Char;
	using Data::DataFactory;
	using Gameplay::Stage;
	using IO::UI;
	using IO::Messages;
	using IO::Chatbar;
	using IO::UIElement;
	using IO::UIStatusMessenger;
	using IO::UIStatusbar;

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
		string message = recv.read<string>();
		int8_t type = recv.readbyte();

		auto speaker = Stage::get().getcharacter(charid);
		if (speaker)
		{
			message = speaker->getname() + ": " + message;
			speaker->speak(message);
		}

		auto linetype = static_cast<Chatbar::LineType>(type);
		UI::get().withelement(UIElement::STATUSBAR, &UIStatusbar::sendchatline, message, linetype);
	}

	void ScrollResultHandler::handle(InPacket& recv) const
	{
		int32_t cid = recv.readint();
		bool success = recv.readbool();
		bool destroyed = recv.readbool();
		recv.readshort(); // legendary spirit if 1

		Char::Effect effect;
		Messages::Type message;
		if (success)
		{
			effect = Char::SCROLL_SUCCESS;
			message = Messages::SCROLL_SUCCESS;
		}
		else
		{
			effect = Char::SCROLL_FAILURE;
			if (destroyed)
			{
				message = Messages::SCROLL_DESTROYED;
			}
			else
			{
				message = Messages::SCROLL_FAILURE;
			}
		}

		Stage::get().getcharacter(cid)
			.ifpresent(&Char::showeffectbyid, effect);

		if (Stage::get().isplayer(cid))
		{
			UI::get().withelement(UIElement::STATUSBAR, &UIStatusbar::displaymessage, message, Chatbar::RED);
			UI::get().enable();
		}
	}

	void ServerMessageHandler::handle(InPacket& recv) const
	{
		int8_t type = recv.readbyte();
		bool servermessage = recv.inspectbool();
		if (servermessage)
			recv.skip(1);
		string message = recv.read<string>();
		
		if (type == 3)
		{
			recv.readbyte(); // channel
			recv.readbool(); // megaphone
		}
		else if (type == 4)
		{
			UI::get().setscrollingnotice(message);
		}
		else if (type == 7)
		{
			recv.readint(); // npcid
		}
	}

	void WeekEventMessageHandler::handle(InPacket& recv) const
	{
		recv.readbyte(); // always 0xFF in solaxia and moople
		string message = recv.read<string>();

		static const string MAPLETIP = "[MapleTip]";
		if (message.substr(0, MAPLETIP.length()).compare("[MapleTip]"))
			message = "[Notice] " + message;

		UI::get().withelement(UIElement::STATUSBAR, &UIStatusbar::sendchatline, message, Chatbar::YELLOW);
	}
}