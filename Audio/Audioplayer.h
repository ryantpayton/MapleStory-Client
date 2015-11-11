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
#include <cstdint>

namespace Audio
{
	using::std::uint8_t;

	class Audioplayer
	{
	public:
		virtual ~Audioplayer() {}
		virtual void playbgm(void*, size_t) = 0;
		virtual void playsound(size_t) = 0;
		virtual void cachesound(void*, size_t, size_t) = 0;
		virtual void setsfxvolume(uint8_t) = 0;
		virtual void setbgmvolume(uint8_t) = 0;
	};
}