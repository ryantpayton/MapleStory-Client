/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015-2016 Daniel Allendorf                                   //
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
#include "TestingHandlers.h"

#include "../../Configuration.h"

#include "../Packets/LoginPackets.h"

#include "../../IO/UI.h"
#include "../../IO/UITypes/UILoginNotice.h"

namespace jrc
{
	void CheckSpwResultHandler::handle(InPacket& recv) const
	{
		int reason = recv.read_byte();

		if (reason == 0)
			UI::get().emplace<UILoginNotice>(UILoginNotice::Message::INCORRECT_PIC);
		else
			std::cout << "[CheckSpwResultHandler]: Reason (" << reason << ") not handled!" << std::endl;

		UI::get().enable();
	}

	void FieldEffectHandler::handle(InPacket& recv) const
	{
		int rand = recv.read_byte();
		std::string path = recv.read_string();

		std::cout << "[FieldEffectHandler]: Random Value: " << rand << " Path: " << path << std::endl;
	}

	void FieldObstacleOnOffListHandler::handle(InPacket& recv) const
	{
		//EnumMap<std::string, int> envList;
		int envListSize = recv.read_int();

		for (int i = 0; i < envListSize; i++) {
			std::string envMoveKey = recv.read_string();
			int envMoveVal = recv.read_int();

			//envList.add?
		}

		std::cout << "[FieldObstacleOnOffListHandler]: envListSize: " << envListSize << std::endl;//<< " envList: " << envList << std::endl;
	}

	void AdminResultHandler::handle(InPacket& recv) const
	{
		int type = recv.read_byte();
		int mode = recv.read_byte();

		// GM Hide
		if (type == 16)
		{
			// Hide GM otherwise do nothing since they are already unhidden
			if (mode == 1)
			{
				// TODO: Hide GM?
			}
		}
		else {
			std::cout << "[AdminResultHandler]: Unhandled Admin Result! Type: " << type << " Mode: " << mode << std::endl;
		}
	}

	void FacialExpressionHandler::handle(InPacket& recv) const
	{
		int from = recv.read_int();
		int expression = recv.read_int();

		// TODO: Show expression?

		std::cout << "[FacialExpressionHandler]: Expression (" << expression << ") expressed from (" << from << ")" << std::endl;
	}

	void GiveForeignBuffHandler::handle(InPacket& recv) const
	{
		std::string message;

		while (recv.available()) {
			message += recv.read_byte();
		}

		std::cout << "[GiveForeignBuffHandler]: " << message << std::endl;
	}

	void CancelForeignBuffHandler::handle(InPacket& recv) const
	{
		std::string message;

		while (recv.available()) {
			message += recv.read_byte();
		}

		std::cout << "[CancelForeignBuffHandler]: " << message << std::endl;
	}

	void UpdateQuestInfoHandler::handle(InPacket& recv) const
	{
		std::string message;

		while (recv.available()) {
			message += recv.read_byte();
		}

		std::cout << "[UpdateQuestInfoHandler]: " << message << std::endl;
	}

	void LockUiHandler::handle(InPacket& recv) const
	{
		int lockUi = recv.read_byte();

		if (lockUi) {
			// TODO: Lock UI?
		}

		std::cout << "[LockUiHandler]: Lock UI? " << lockUi << std::endl;
	}

	void ToggleUiHandler::handle(InPacket& recv) const
	{
		int disableUi = recv.read_byte();

		if (disableUi) {
			// TODO: Disable UI?
		}

		std::cout << "[ToggleUiHandler]: Disable UI? " << disableUi << std::endl;
	}

	void ConfirmShopTransactionHandler::handle(InPacket& recv) const
	{
		int code = recv.read_byte();

		std::cout << "[ConfirmShopTransactionHandler]: Code: " << code << std::endl;
	}

	void PlayerInteractionHandler::handle(InPacket& recv) const
	{
		int item = recv.read_byte();

		std::cout << "[PlayerInteractionHandler]: Item: " << item << std::endl;
	}

	void AutoHpPotHandler::handle(InPacket& recv) const
	{
		int item = recv.read_byte();

		std::cout << "[AutoHpPotHandler]: Item: " << item << std::endl;
	}

	void AutoMpPotHandler::handle(InPacket& recv) const
	{
		int item = recv.read_byte();

		std::cout << "[AutoMpPotHandler]: Item: " << item << std::endl;
	}
}