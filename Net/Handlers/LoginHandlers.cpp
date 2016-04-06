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
#include "LoginHandlers.h"
#include "Net\Session.h"
#include "Net\Packets\LoginPackets.h"
#include "IO\UI.h"
#include "IO\UITypes\UILogin.h"
#include "IO\UITypes\UILoginNotice.h"
#include "IO\UITypes\UIWorldSelect.h"
#include "IO\UITypes\UICharSelect.h"
#include "IO\UITypes\UICharcreation.h"
#include "Configuration.h"

namespace Net
{
	using IO::UI;
	using IO::UIElement;
	using IO::ElementLoginNotice;
	using IO::ElementWorldSelect;
	using IO::ElementCharSelect;
	using IO::ElementCharcreation;
	using IO::UILoginNotice;
	using IO::UICharcreation;

	void LoginResultHandler::handle(InPacket& recv) const
	{
		// Remove login information.
		UI::get().remove(UIElement::LOGINNOTICE);
		UI::get().remove(UIElement::LOGINWAIT);

		// The packet should contain a 'reason' integer which can signify various things.
		int32_t reason = recv.readint();
		if (reason != 0)
		{
			// Login unsuccessfull. The LoginNotice displayed will contain the specific information.
			switch (reason)
			{
			case 2:
				UI::get().add(ElementLoginNotice(16));
				break;
			case 7:
				UI::get().add(ElementLoginNotice(17));
				break;
			case 23:
				// The server sends a request to accept the terms of service. For convenience, just auto-accept.
				TOSPacket().dispatch();
				break;
			default:
				// Other reasons.
				if (reason > 0)
				{
					auto reasonbyte = static_cast<int8_t>(reason - 1);
					UI::get().add(ElementLoginNotice(reasonbyte));
				}
			}

			UI::get().enable();
		}
		else
		{
			// Login successfull. The packet contains information on the account, so we initialise the account with it.
			Session::get().getlogin().parseaccount(recv);

			// Save the Login ID if the box for it on the login panel is checked.
			bool savelogin = Setting<SaveLogin>::get().load();
			if (savelogin)
			{
				string name = Session::get().getlogin().getaccount().name;
				Setting<DefaultAccount>::get().save(name);
			}

			// Request the list of worlds and channels online.
			ServerRequestPacket().dispatch();
		}
	}

	void ServerlistHandler::handle(InPacket& recv) const
	{
		// Remove the Login UI.
		UI::get().remove(UIElement::LOGIN);

		// Parse all worlds.
		Session::get().getlogin().parseworlds(recv);

		// Add the world selection screen to the ui.
		UI::get().add(ElementWorldSelect());

		UI::get().enable();
	}

	void CharlistHandler::handle(InPacket& recv) const
	{
		recv.skip(1);

		// Parse all characters.
		Session::get().getlogin().parsecharlist(recv);

		// Remove the world selection screen.
		UI::get().remove(UIElement::WORLDSELECT);

		// Add the character selection screen.
		UI::get().add(ElementCharSelect());

		UI::get().enable();
	}

	void CharnameResponseHandler::handle(InPacket& recv) const
	{
		// Read the name and if it is already in use.
		string name = recv.read<string>();
		bool used = recv.readbool();

		if (used)
		{
			UI::get().add(ElementLoginNotice(UILoginNotice::NAME_IN_USE));
		}

		// Notify character creation screen.
		UI::get().withelement(UIElement::CHARCREATION, &UICharcreation::nameresult, used);

		UI::get().enable();
	}

	void AddNewCharEntryHandler::handle(InPacket& recv) const
	{
		recv.skip(1);

		// Parse info on the new character.
		Session::get().getlogin().addcharentry(recv);

		// Remove the character creation ui.
		UI::get().remove(UIElement::CHARCREATION);

		// Readd the updated character selection.
		UI::get().add(ElementCharSelect());

		UI::get().enable();
	}

	void DeleteCharResponseHandler::handle(InPacket& recv) const
	{
		// Read the character id and if deletion was successfull (pic was correct).
		recv.readint(); // charid

		bool success = recv.readbool();

		// Show the result to the user.
		if (success)
		{
			UI::get().add(ElementLoginNotice(UILoginNotice::CASH_ITEMS_CONFIRM_DELETION));
		}
		else
		{
			UI::get().add(ElementLoginNotice(UILoginNotice::BIRTHDAY_INCORRECT));
		}
	}

	void ServerIPHandler::handle(InPacket& recv) const
	{
		recv.skip(2);

		// Read the ipv4 adress in a string.
		string addrstr;
		for (int i = 0; i < 4; i++)
		{
			uint8_t num = static_cast<uint8_t>(recv.readbyte());
			addrstr.append(std::to_string(num));
			if (i < 3)
			{
				addrstr.push_back('.');
			}
		}

		// Read the port adress in a string.
		string portstr = std::to_string(recv.readshort());

		int32_t cid = recv.readint();

		// Attempt to reconnect to the server and if successfull, login to the game.
		Session::get().reconnect(addrstr.c_str(), portstr.c_str());
		PlayerLoginPacket(cid).dispatch();
	}
}