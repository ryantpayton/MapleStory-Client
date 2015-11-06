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
#include "AnimationData.h"

namespace Graphics
{
	class AnimationController
	{
	public:
		AnimationController(const AnimationData&);
		AnimationController();
		~AnimationController();
		void reset(const AnimationData&);
		bool update(const AnimationData&, uint16_t);
		uint8_t getframe() const;
		float getalpha() const;
	private:
		uint8_t frame;
		bool blending;
		uint16_t elapsed;
		float alphastep;
		float alpha;
	};
}

