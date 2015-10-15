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
#include "OutPacket.h"

namespace Net
{
	class MovementPacket : public OutPacket
	{
	public:
		MovementPacket(SendOpcode opc) : OutPacket(opc) {}
	protected:
		/*void writemoves(vector<movefragment> moves)
		{
			p->writech(moves.size());

			for (vector<movefragment>::iterator mvit = moves.begin(); mvit != moves.end(); ++mvit)
			{
				p->writech(mvit->command);
				p->writesh(mvit->xpos);
				p->writesh(mvit->ypos);
				p->writesh(mvit->xpps);
				p->writesh(mvit->ypps);
				p->writesh(mvit->unk);
				p->writech(mvit->newstate);
				p->writesh(mvit->duration);
			}
		}*/
	};
}