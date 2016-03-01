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
#include "Constants.h"

namespace Graphics
{
	Animation::Animation(node src)
	{
		bool istexture = src.data_type() == node::type::bitmap;
		if (istexture)
		{
			frames.push_back(src);
		}
		else
		{
			uint8_t framec = 0;
			node nodeit = src[std::to_string(framec)];
			while (nodeit.data_type() == node::type::bitmap)
			{
				frames.push_back(nodeit);

				framec++;
				nodeit = src[std::to_string(framec)];
			}

			if (frames.size() == 0)
				frames.push_back(Frame());
		}

		animated = frames.size() > 1;
		zigzag = src["zigzag"].get_bool();

		reset();
	}

	Animation::Animation() 
	{
		animated = false;
		zigzag = false;

		frames.push_back(Frame());

		reset();
	}

	void Animation::draw(const DrawArgument& args, float inter) const
	{
		uint8_t interframe;
		float interalpha;
		if (animated)
		{
			if (lastelapsed + Constants::TIMESTEP * inter > frames[lastframe].delay)
			{
				interframe = frame;
			}
			else
			{
				interframe = lastframe;
			}
			interalpha = (1.0f - inter) * lastalpha + inter * alpha;
		}
		else
		{
			interframe = 0;
			interalpha = 255.0f;
		}

		bool modifyalpha = args.getalpha() == 1.0f && interalpha != 255.0f;
		if (modifyalpha)
		{
			frames[interframe].texture.draw(args.overwritealpha(interalpha / 255));
		}
		else
		{
			frames[interframe].texture.draw(args);
		}
	}

	void Animation::reset()
	{
		elapsed = 0;
		frame = 0;
		framestep = 1;
		alpha = frames[0].opacities.first;
		alphastep = frames[0].alphastep(alpha, Constants::TIMESTEP);

		lastframe = frame;
		lastalpha = alpha;
	}

	bool Animation::update()
	{
		return update(Constants::TIMESTEP);
	}

	bool Animation::update(uint16_t timestep)
	{
		if (animated)
		{
			lastframe = frame;
			lastelapsed = elapsed;
			lastalpha = alpha;

			alpha += alphastep;
			if (alpha < 0.0f)
			{
				alpha = 0.0f;
			}
			else if (alpha > 255.0f)
			{
				alpha = 255.0f;
			}

			elapsed += timestep;

			uint16_t delay = frames[frame].delay;
			if (elapsed > delay)
			{
				elapsed -= delay;

				size_t framecount = frames.size();
				if (zigzag)
				{
					if (framestep == 1 && frame == framecount - 1)
					{
						framestep = -1;
						frame--;
					}
					else if (framestep == -1 && frame == 0)
					{
						framestep = 1;
						frame++;
					}
					else
					{
						frame += framestep;
					}
				}
				else
				{
					if (frame == framecount - 1)
					{
						frame = 0;
					}
					else
					{
						frame++;
					}
				}

				alphastep = frames[frame].alphastep(alpha, timestep);
				return frame == 0;
			}
		}
		return false;
	}

	Point<int16_t> Animation::getorigin() const
	{
		return frames[frame].texture.getorigin();
	}

	Point<int16_t> Animation::getdimensions() const
	{
		return frames[frame].texture.getdimensions();
	}

	Point<int16_t> Animation::gethead() const
	{ 
		return frames[frame].head;
	}

	rectangle2d<int16_t> Animation::getbounds() const
	{
		return frames[frame].bounds;
	}
}
