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
#include "ChildHandler.h"
#include "UILogin.h"
#include "UILoginNotice.h"
#include "UIWorldSelect.h"
#include "UICharSelect.h"
#include "TOSPacket.h"
#include "ServerRequestPacket.h"
#include "PlayerLoginPacket.h"

namespace Net
{
	class LoginResultHandler : public ChildHandler
	{
		void LoginResultHandler::handle(ParentHandler* parent, InPacket* recv)
		{
			parent->getui()->remove(UI_LOGINWAIT);
			int reason = recv->readint();
			if (reason != 0)
			{
				switch (reason)
				{
				case 2:
					parent->getui()->add(&ElementLoginNotice(16));
					return;
				case 7:
					parent->getui()->add(&ElementLoginNotice(17));
					return;
				case 23:
					parent->getsession()->dispatch(&TOSPacket());
					return;
				default:
					if (reason > 0)
					{
						parent->getui()->add(&ElementLoginNotice(reason - 1));
					}
				}
				parent->getui()->enable();
				return;
			}
			else
			{
				parent->getlogin()->parseaccount(recv);
				/*if (config->saveid)
				{
					config->defaultacc = accname;
				}*/
				parent->getsession()->dispatch(&ServerRequestPacket());
			}
		}
	};

	class ServerlistHandler : public ChildHandler
	{
		void ServerlistHandler::handle(ParentHandler* parent, InPacket* recv)
		{
			parent->getlogin()->parseworld(recv);
			parent->getui()->remove(UI_LOGIN);
			parent->getui()->add(&ElementWorldSelect(parent->getui(), parent->getlogin(), parent->getsession()));
			parent->getui()->enable();
		}
	};

	class CharlistHandler : public ChildHandler
	{
		void CharlistHandler::handle(ParentHandler* parent, InPacket* recv)
		{
			recv->readbyte();
			char numchars = recv->readbyte();
			for (char i = 0; i < numchars; i++)
			{
				parent->getlogin()->getaccount()->parsecharentry(recv);
			}
			char pic = recv->readbyte();
			char slots = static_cast<char>(recv->readint());
			//Game::getcache()->getsounds()->play(MSN_CHARSEL);
			parent->getlogin()->getaccount()->setpic(pic);
			parent->getlogin()->getaccount()->setslots(slots);
			parent->getui()->remove(UI_WORLDSELECT);
			parent->getui()->add(&ElementCharSelect(parent->getcache()->getequips(), parent->getui(), parent->getlogin(), parent->getsession()));
			parent->getui()->enable();
		}
	};

	class CharnameResultHandler : public ChildHandler
	{
		void CharnameResultHandler::handle(ParentHandler* parent, InPacket* recv)
		{
			string name = recv->readascii();
			bool used = recv->readbool();
			UIElement* uicc = parent->getui()->getelement(UI_CHARCREATION);
			if (uicc)
			{
				//uicc->getimpl()->nameresult(used);
			}
			if (used)
			{
				parent->getui()->add(&ElementLoginNotice(5));
			}
			parent->getui()->enable();
		}
	};

	class AddNewcharHandler : public ChildHandler
	{
		void AddNewcharHandler::handle(ParentHandler* parent, InPacket* recv)
		{
			char stuff = recv->readbyte();
			if (stuff == 0)
			{
				parent->getlogin()->getaccount()->parsecharentry(recv);
				parent->getui()->getelement(UI_CHARCREATION)->deactivate();
				parent->getui()->remove(UI_CHARCREATION);
				parent->getui()->add(&ElementCharSelect(parent->getcache()->getequips(), parent->getui(), parent->getlogin(), parent->getsession()));
				parent->getui()->enable();
			}
		}
	};

	class DeleteCharResultHandler : public ChildHandler
	{
		void DeleteCharResultHandler::handle(ParentHandler* parent, InPacket* recv)
		{
			int cid = recv->readint();
			bool success = recv->readbool();
			if (success)
			{
				parent->getui()->add(&ElementLoginNotice(55));
			}
			else
			{
				parent->getui()->add(&ElementLoginNotice(93));
			}
		}
	};

	class ServerIPHandler : public ChildHandler
	{
		void ServerIPHandler::handle(ParentHandler* parent, InPacket* recv)
		{
			recv->readshort();
			vector<uint8_t> addr;
			for (char i = 0; i < 4; i++)
			{
				addr.push_back(recv->readbyte());
			}
			short port = recv->readshort();
			int cid = recv->readint();
			parent->getsession()->reconnect(addr, port);
			parent->getsession()->dispatch(&PlayerLoginPacket(cid));
		}
	};
}