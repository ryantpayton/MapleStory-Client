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
	Animation::Frame::Frame(node src)
	{
		texture = src;
		bounds = src;
		head = src["head"];
		delay = src["delay"];
		if (delay == 0)
			delay = 100;

		uint8_t a0;
		uint8_t a1;
		node::type a0type = src["a0"].data_type();
		if (a0type == node::type::integer)
		{
			node::type a1type = src["a1"].data_type();
			if (a1type == node::type::integer)
			{
				a0 = src["a0"];
				a1 = src["a1"];
			}
			else
			{
				a0 = src["a0"];
				a1 = 255 - a0;
			}
		}
		else
		{
			a0 = 255;
			a1 = 255;
		}
		opacities = { a0, a1 };
	}

	Animation::Frame::Frame()
	{
		delay = 0;
		opacities = { 0, 0 };
	}

	float Animation::Frame::opcstep(float opacity, uint16_t timestep) const
	{
		return timestep * (opacities.second - opacity) / delay;
	}


	Animation::Animation(node src)
	{
		bool istexture = src.data_type() == node::type::bitmap;
		if (istexture)
		{
			frames.push_back(src);
		}
		else
		{
			int16_t fc = 0;
			node nodeit = src[std::to_string(fc)];
			while (nodeit.data_type() == node::type::bitmap)
			{
				frames.push_back(nodeit);

				fc++;
				nodeit = src[std::to_string(fc)];
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

	void Animation::reset()
	{
		frame.set(0);
		opacity.set(frames[0].opacities.first);
		opcstep = frames[0].opcstep(opacity.get(), Constants::TIMESTEP);
		delay = frames[0].delay;
		framestep = 1;
	}

	void Animation::draw(const DrawArgument& args, float alpha) const
	{
		int16_t interframe = frame.get(alpha);
		float interopc = opacity.get(alpha);

		bool modifyopc = interopc != 255.0f;
		if (modifyopc)
		{
			frames[interframe].texture.draw(args + interopc / 255);
		}
		else
		{
			frames[interframe].texture.draw(args);
		}
	}

	bool Animation::update()
	{
		return update(Constants::TIMESTEP);
	}

	bool Animation::update(uint16_t timestep)
	{
		if (!animated)
			return true;

		opacity += opcstep;
		if (opacity < 0.0f)
		{
			opacity = 0.0f;
		}
		else if (opacity > 255.0f)
		{
			opacity = 255.0f;
		}

		if (timestep >= delay)
		{
			int16_t lastframe = static_cast<int16_t>(frames.size() - 1);
			int16_t nextframe;
			bool ended;
			if (zigzag)
			{
				if (framestep == 1 && frame == lastframe)
				{
					framestep = -framestep;
					ended = false;
				}
				else if (framestep == -1 && frame == 0)
				{
					framestep = -framestep;
					ended = true;
				}
				else
				{
					ended = false;
				}

				nextframe = frame + framestep;
			}
			else
			{
				if (frame == lastframe)
				{
					nextframe = 0;
					ended = true;
				}
				else
				{
					nextframe = frame + 1;
					ended = false;
				}
			}

			uint16_t delta = timestep - delay;
			float threshold = static_cast<float>(delta) / timestep;
			frame.next(nextframe, threshold);

			delay = frames[nextframe].delay - delta;
			opcstep = frames[nextframe].opcstep(opacity.get(), timestep);
			return ended;
		}
		else
		{
			frame.normalize();

			delay -= timestep;
			return false;
		}
	}

	uint16_t Animation::getdelay(int16_t frame) const
	{
		return frame < frames.size() ? frames[frame].delay : 0;
	}

	uint16_t Animation::getdelayuntil(int16_t frame) const
	{
		uint16_t total = 0;
		for (int16_t i = 0; i < frame; i++)
		{
			if (i >= frames.size())
				break;

			total += frames[frame].delay;
		}
		return total;
	}

	Point<int16_t> Animation::getorigin() const
	{
		return getframe().texture.getorigin();
	}

	Point<int16_t> Animation::getdimensions() const
	{
		return getframe().texture.getdimensions();
	}

	Point<int16_t> Animation::gethead() const
	{ 
		return getframe().head;
	}

	rectangle2d<int16_t> Animation::getbounds() const
	{
		return getframe().bounds;
	}

	const Animation::Frame& Animation::getframe() const
	{
		return frames[frame.get()];
	}
}
