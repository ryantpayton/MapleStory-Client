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
#include "IO\UI.h"
#include "Data\DataFactory.h"

namespace Net
{
	using Graphics::Textlabel;
	using Character::ItemData;

	// Handler for a packet which contains status messages. Modes:
	// 0 - Item(0) or Meso(1) 
	// 3 - Exp gain
	// 4 - Fame
	// 5 - Mesos
	// 6 - Guild points
	class ShowStatusInfoHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override
		{
			int8_t mode = recv.readbyte();
			if (mode == 0)
			{
				int8_t mode2 = recv.readbyte();
				if (mode2 == 0)
				{
					int32_t itemid = recv.readint();
					int32_t qty = recv.readint();
					const ItemData& idata = Data::getitemdata(itemid);
					if (!idata.isloaded())
						return;

					string name = idata.getname();
					string sign = (qty < 0) ? "-" : "+";
					showstatus(Textlabel::TXC_WHITE, "Gained an item: " + name + " (" + sign + std::to_string(qty) + ")");
				}
				else if (mode2 == 1)
				{
					recv.skip(1);

					int32_t gain = recv.readint();
					string sign = (gain < 0) ? "-" : "+";
					showstatus(Textlabel::TXC_WHITE, "Received mesos (" + sign + std::to_string(gain) + ")");
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
					showstatus(white ? Textlabel::TXC_WHITE : Textlabel::TXC_YELLOW, message);
					if (bonus1 > 0)
						showstatus(Textlabel::TXC_YELLOW, "+ Bonus EXP (+" + std::to_string(bonus1) + ")");
				}
			}
			else if (mode == 4)
			{
				int32_t gain = recv.readint();
				string sign = (gain < 0) ? "-" : "+";
				showstatus(Textlabel::TXC_WHITE, "Received fame (" + sign + std::to_string(gain) + ")");
			}
			else if (mode == 5)
			{
			}
		}

		void showstatus(Textlabel::Textcolor color, string message) const
		{
			IO::UI::showstatus(color, message);
		}
	};
}