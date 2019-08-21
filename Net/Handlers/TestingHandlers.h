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
#pragma once

#include "../PacketHandler.h"

namespace ms
{
	// TODO: Comment
	class CheckSpwResultHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	// TODO: Comment
	class FieldEffectHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	// TODO: Comment
	class FieldObstacleOnOffListHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	// TODO: Comment
	class AdminResultHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	// TODO: Comment
	class FacialExpressionHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	// TODO: Comment
	class GiveForeignBuffHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	// TODO: Comment
	class CancelForeignBuffHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	// TODO: Comment
	class UpdateQuestInfoHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	// TODO: Comment
	class LockUiHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	// TODO: Comment
	class ToggleUiHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	// TODO: Comment
	class ConfirmShopTransactionHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	// TODO: Comment
	class PlayerInteractionHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	// TODO: Comment
	class AutoHpPotHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	// TODO: Comment
	class AutoMpPotHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};
}