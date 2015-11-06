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
#include "AnimationController.h"

namespace Graphics
{
	AnimationController::AnimationController(const AnimationData& animdata)
	{
		reset(animdata);
	}

	AnimationController::AnimationController() {}

	AnimationController::~AnimationController() {}

	void AnimationController::reset(const AnimationData& animdata)
	{
		elapsed = 0;
		frame = 0;
		alpha = static_cast<float>(animdata.getalpha(0));
		alphastep = animdata.nextalpha(0, alpha) / (animdata.getdelay(0) / 16);
	}

	bool AnimationController::update(const AnimationData& animdata, uint16_t dpf)
	{
		if (animdata.getlastframe() > 0)
		{
			elapsed += dpf;

			alpha += alphastep;
			if (alpha < 0.f)
				alpha = 0.f;
			else if (alpha > 255.f)
				alpha = 255.f;

			uint16_t delay = animdata.getdelay(frame);
			if (elapsed > delay)
			{
				elapsed -= delay;
				frame = animdata.nextframe(frame);
				alphastep = (animdata.nextalpha(frame, alpha) * dpf) / delay;
				return frame == 0;
			}
		}
		return false;
	}

	uint8_t AnimationController::getframe() const
	{
		return frame;
	}

	float AnimationController::getalpha() const
	{
		return alpha / 255;
	}
}
