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
#include "Net\PacketHandler.h"
#include "Net\Packets\LoginPackets.h"
#include "Net\Session.h"
#include "IO\UITypes\UILogin.h"
#include "IO\UITypes\UILoginNotice.h"
#include "IO\UITypes\UIWorldSelect.h"
#include "IO\UITypes\UICharSelect.h"
#include "IO\UITypes\UICharcreation.h"
#include "IO\UI.h"
#include "Program\Configuration.h"

namespace Net
{
	using IO::Element;

	// Handler for a packet that contains the response to an attempt at logging in.
	class LoginResultHandler83 : public PacketHandler
	{
		void handle(InPacket& recv) const override
		{
			// Remove login information.
			IO::UI::remove(Element::LOGINNOTICE);
			IO::UI::remove(Element::LOGINWAIT);

			// The packet should contain a 'reason' integer which can signify various things.
			int32_t reason = recv.readint();
			if (reason != 0)
			{
				// Login unsuccessfull. The LoginNotice displayed will contain the specific information.
				using::IO::ElementLoginNotice;
				switch (reason)
				{
				case 2:
					IO::UI::add(ElementLoginNotice(16));
					return;
				case 7:
					IO::UI::add(ElementLoginNotice(17));
					return;
				case 23:
					// The server sends a request to accept the terms of service. For convenience, just auto-accept.
					Session::dispatch(TOSPacket());
					return;
				default:
					// Other reasons.
					if (reason > 0)
						IO::UI::add(ElementLoginNotice(static_cast<int8_t>(reason - 1)));
				}
				IO::UI::enable();
			}
			else
			{
				// Login successfull. The packet contains information on the account, so we initialise the account with it.
				Session::getlogin().parseaccount(recv);

				// Save the Login ID if the box for it on the login panel is checked.
				if (Program::Configuration::getbool("SaveLogin"))
				{
					Program::Configuration::setstring(
						"Account", Session::getlogin().getaccount().name
						);
				}

				// Request the list of worlds and channels online.
				Session::dispatch(ServerRequestPacket());
			}
		}
	};

	// Handles the packet that contains information on worlds and channels.
	class ServerlistHandler83 : public PacketHandler
	{
		void handle(InPacket& recv) const override
		{
			// Remove the Login UI.
			IO::UI::remove(Element::LOGIN);

			// Parse all worlds.
			Session::getlogin().parseworlds(recv);

			// Add the world selection screen to the ui.
			using::IO::ElementWorldSelect;
			IO::UI::add(ElementWorldSelect());
			IO::UI::enable();
		}
	};

	// Handler for a packet that contains information on all chars on this world.
	class CharlistHandler83 : public PacketHandler
	{
		void handle(InPacket& recv) const override
		{
			recv.skip(1);
			// Parse all characters.
			Session::getlogin().parsecharlist(recv);

			// Remove the world selection screen.
			IO::UI::remove(Element::WORLDSELECT);

			// Add the character selection screen.
			using::IO::ElementCharSelect;
			IO::UI::add(ElementCharSelect());
			IO::UI::enable();
		}
	};

	// Handler for a packet which responds to the request for a character name.
	class CharnameResponseHandler83 : public PacketHandler
	{
		void handle(InPacket& recv) const override
		{
			// Read the name and if it is already in use.
			string name = recv.readascii();
			bool used = recv.readbool();

			if (used)
			{
				using::IO::ElementLoginNotice;
				IO::UI::add(ElementLoginNotice(5));
			}

			// Notify character creation screen.
			using::IO::UIElement;
			UIElement* uicc = IO::UI::getelement(Element::CHARCREATION);
			if (uicc)
			{
				using::IO::UICharcreation;
				reinterpret_cast<UICharcreation*>(uicc)->nameresult(used);
			}

			IO::UI::enable();
		}
	};

	// Handler for the packet that notifies that a char was successfully created.
	class AddNewCharEntryHandler83 : public PacketHandler
	{
		void handle(InPacket& recv) const override
		{
			recv.skip(1);

			// Parse info on the new character.
			Session::getlogin().addcharentry(recv);

			// Remove the character creation ui.
			IO::UIElement* uicc = IO::UI::getelement(Element::CHARCREATION);
			if (uicc)
			{
				uicc->deactivate();
				IO::UI::remove(Element::CHARCREATION);
			}

			// Readd the updated character selection.
			IO::UI::remove(Element::CHARSELECT);
			IO::UI::add(IO::ElementCharSelect());
			IO::UI::enable();
		}
	};

	// Handler for a packet that responds to the request to the delete a character.
	class DeleteCharResponseHandler83 : public PacketHandler
	{
		void handle(InPacket& recv) const override
		{
			// Read the character id and if deletion was successfull (pic was correct).
			recv.readint(); // charid
			bool success = recv.readbool();

			// Show the result to the user.
			if (success)
				IO::UI::add(IO::ElementLoginNotice(55));
			else
				IO::UI::add(IO::ElementLoginNotice(93));
		}
	};

	// Handles the packet which contains the IP of a channel server to connect to.
	class ServerIPHandler83 : public PacketHandler
	{
		void handle(InPacket& recv) const override
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

			// Attempt to reconnect to the server and if successfull, login to the game.
			int32_t cid = recv.readint();
			bool connected = Session::reconnect(addrstr.c_str(), portstr.c_str());
			if (connected)
				Session::dispatch(PlayerLoginPacket(cid));
			else
				Session::disconnect();
		}
	};
}