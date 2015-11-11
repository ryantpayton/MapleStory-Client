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
#include "Audio\Audioplayer.h"
#include "Gameplay\StageInterface.h"
#include "IO\UI.h"
#include "Net\Session.h"
#include "Util\Configuration.h"
#include "Util\NxFileManager.h"

namespace Journey
{
	using::IO::UI;
	using::Util::NxFileManager;
	using::Util::Configuration;
	using::Gameplay::StageInterface;
	using::Net::Session;
	using::Audio::Audioplayer;

	class ClientInterface
	{
	public:
		virtual ~ClientInterface() {}
		virtual UI& getui() = 0;
		virtual NxFileManager& getnxfiles() = 0;
		virtual Configuration& getconfig() = 0;
		virtual StageInterface& getstage() = 0;
		virtual Session& getsession() = 0;
		virtual Audioplayer& getaudio() = 0;
	};
}