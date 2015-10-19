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
#include "Datacache.h"
#include "Stage.h"
#include "UI.h"
#include "Login.h"
#include "Session.h"

using namespace Gameplay;
using namespace Program;
using namespace IO;

namespace Net
{
	class ParentHandler
	{
	public:
		virtual ~ParentHandler(){}
		virtual void handle(InPacket&) = 0;
		virtual Datacache& getcache() const = 0;
		virtual Stage& getstage() const = 0;
		virtual UI& getui() const = 0;
		virtual Login& getlogin() const = 0;
		virtual Session& getsession() const = 0;
	};
}