/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015 SYJourney                                               //
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
#include "Packethandler.h"
#include "UILogin.h"
#include "UILoginNotice.h"
#include "UIWorldSelect.h"
#include "UICharSelect.h"
#include "LoginPackets.h"

namespace Net
{
	class LoginResultHandler : public Handler
	{
		void LoginResultHandler::handle(InPacket* recv)
		{
			ui->remove(UI_LOGINWAIT);
			int reason = recv->readint();
			if (reason != 0)
			{
				switch (reason)
				{
				case 2:
					ui->add(&ElementLoginNotice(16));
					return;
				case 7:
					ui->add(&ElementLoginNotice(17));
					return;
				case 23:
					session->dispatch(&TOSPacket());
					return;
				default:
					if (reason > 0)
					{
						ui->add(&ElementLoginNotice(reason - 1));
					}
				}
				ui->enable();
				return;
			}
			else
			{
				login->parseaccount(recv);
				/*if (config->saveid)
				{
					config->defaultacc = accname;
				}*/
				session->dispatch(&ServerRequestPacket());
			}
		}
	};

	class ServerlistHandler : public Handler
	{
		void ServerlistHandler::handle(InPacket* recv)
		{
			login->parseworld(recv);
			ui->remove(UI_LOGIN);
			ui->add(&ElementWorldSelect(ui, login, session));
			ui->enable();
		}
	};

	class CharlistHandler : public Handler
	{
		void CharlistHandler::handle(InPacket* recv)
		{
			recv->readbyte();
			char numchars = recv->readbyte();
			for (char i = 0; i < numchars; i++)
			{
				login->getaccount()->parsecharentry(recv);
			}
			char pic = recv->readbyte();
			char slots = static_cast<char>(recv->readint());
			//Game::getcache()->getsounds()->play(MSN_CHARSEL);
			login->getaccount()->setpic(pic);
			login->getaccount()->setslots(slots);
			ui->remove(UI_WORLDSELECT);
			ui->add(&ElementCharSelect(cache->getequips(), ui, login, session));
			ui->enable();
		}
	};

	class CharnameResultHandler : public Handler
	{
		void CharnameResultHandler::handle(InPacket* recv)
		{
			string name = recv->readascii();
			bool used = recv->readbool();
			UIElement* uicc = ui->getelement(UI_CHARCREATION);
			if (uicc)
			{
				//uicc->getimpl()->nameresult(used);
			}
			if (used)
			{
				ui->add(&ElementLoginNotice(5));
			}
			ui->enable();
		}
	};

	class AddNewcharHandler : public Handler
	{
		void AddNewcharHandler::handle(InPacket* recv)
		{
			char stuff = recv->readbyte();
			if (stuff == 0)
			{
				login->getaccount()->parsecharentry(recv);
				ui->getelement(UI_CHARCREATION)->deactivate();
				ui->remove(UI_CHARCREATION);
				//ui->add(UI_CHARSELECT);
				ui->enable();
			}
		}
	};

	class DeleteCharResultHandler : public Handler
	{
		void DeleteCharResultHandler::handle(InPacket* recv)
		{
			int cid = recv->readint();
			bool success = recv->readbool();
			if (success)
			{
				ui->add(&ElementLoginNotice(55));
			}
			else
			{
				ui->add(&ElementLoginNotice(93));
			}
		}
	};

	class ServerIPHandler : public Handler
	{
		void ServerIPHandler::handle(InPacket* recv)
		{
			recv->readshort();
			vector<uint8_t> addr;
			for (char i = 0; i < 4; i++)
			{
				addr.push_back(recv->readbyte());
			}
			short port = recv->readshort();
			int clientid = recv->readint();
			session->reconnect(addr, port);
			creator->playerlogin(clientid);
		}
	};
}