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
#include "Handler.h"
#include "Datacache.h"
#include "UI.h"
#include "Login.h"
#include "Session.h"

using namespace Program;
using namespace IO;

namespace Net
{
	class ParentHandler : public Handler
	{
	public:
		virtual void handle(InPacket*) = 0;
		virtual Datacache* getcache() = 0;
		virtual UI* getui() = 0;
		virtual Login* getlogin() = 0;
		virtual Session* getsession() = 0;
	};
}