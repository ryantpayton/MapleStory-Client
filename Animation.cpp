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
#include "Animation.h"

namespace Graphics
{
	Animation::Animation(node src) : animdata(src), animcont(animdata) {}

	Animation::Animation() {}

	void Animation::draw(const DrawArgument& args) const
	{
		const Texture* txt = animdata.gettexture(animcont.getframe());
		if (txt)
		{
			float alpha = animcont.getalpha();
			if (alpha == 1.0f)
			{
				txt->draw(args);
			}
			else
			{
				txt->draw(FullArgument(args.getpos(), args.getstretch(), args.getcenter(), args.getxscale(), args.getyscale(), alpha));
			}
		}
	}

	void Animation::update(uint16_t dpf)
	{
		animcont.update(animdata, dpf);
	}

	vector2d<int32_t> Animation::getorigin() const
	{
		return animdata.getorigin(animcont.getframe());
	}

	vector2d<int32_t> Animation::getdimensions() const
	{
		return animdata.getdimensions(animcont.getframe());
	}
}
