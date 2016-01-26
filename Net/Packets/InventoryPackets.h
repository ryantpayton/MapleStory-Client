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
#include "Net\OutPacket.h"
#include "Net\SendOpcodes.h"

#include "Character\Inventory\Inventory.h"

namespace Net
{
	using Character::Inventory;
	using Character::Clothing;

	// Packet which requests that the inventory is sorted.
	// Opcode: GATHER_ITEMS(69)
	class GatherItemsPacket : public OutPacket
	{
	public:
		GatherItemsPacket(Inventory::InvType type) : OutPacket(GATHER_ITEMS)
		{
			writetime();
			writech(type);
		}
	};


	// Packet which requests that the inventory is sorted.
	// Opcode: SORT_ITEMS(70)
	class SortItemsPacket : public OutPacket
	{
	public:
		SortItemsPacket(Inventory::InvType type) : OutPacket(SORT_ITEMS)
		{
			writetime();
			writech(type);
		}
	};


	// Packet which requests that an item is moved.
	// Opcode: MOVE_ITEM(71)
	class MoveItemPacket : public OutPacket
	{
	public:
		MoveItemPacket(Inventory::InvType type, int16_t slot, int16_t action, int16_t qty) : OutPacket(MOVE_ITEM)
		{
			writetime();
			writech(type);
			writesh(slot);
			writesh(action);
			writesh(qty);
		}
	};


	// Packet which requests that an item is equipped.
	// Opcode: MOVE_ITEM(71)
	class EquipItemPacket : public MoveItemPacket
	{
	public:
		EquipItemPacket(int16_t src, Clothing::Slot dest) : MoveItemPacket(Inventory::EQUIP, src, -dest, 1) {}
	};


	// Packet which requests that an item is unequipped.
	// Opcode: MOVE_ITEM(71)
	class UnequipItemPacket : public MoveItemPacket
	{
	public:
		UnequipItemPacket(int16_t src, int16_t dest) : MoveItemPacket(Inventory::EQUIPPED, -src, dest, 1) {}
	};


	// A packet which requests that an 'USE' item is used.
	// Opcode: USE_ITEM(72)
	class UseItemPacket : public OutPacket
	{
	public:
		UseItemPacket(int16_t slot, int32_t itemid) : OutPacket(USE_ITEM)
		{
			writetime();
			writesh(slot);
			writeint(itemid);
		}
	};
}