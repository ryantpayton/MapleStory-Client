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

#include "Util\Misc.h"

#include <set>

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

		bool hasa0 = src["a0"].data_type() == node::type::integer;
		bool hasa1 = src["a1"].data_type() == node::type::integer;
		if (hasa0 && hasa1)
		{
			opacities = { src["a0"], src["a1"] };
		}
		else if (hasa0)
		{
			uint8_t a0 = src["a0"];
			opacities = { a0, 255 - a0 };
		}
		else if (hasa1)
		{
			uint8_t a1 = src["a1"];
			opacities = { 255 - a1, a1 };
		}
		else
		{
			opacities = { 255, 255 };
		}

		bool hasz0 = src["z0"].data_type() == node::type::integer;
		bool hasz1 = src["z1"].data_type() == node::type::integer;
		if (hasz0 && hasz1)
		{
			scales = { src["z0"], src["z1"] };
		}
		else if (hasz0)
		{
			scales = { src["z0"], 0 };
		}
		else if (hasz1)
		{
			scales = { 100, src["z1"] };
		}
		else
		{
			scales = { 100, 100 };
		}
	}

	Animation::Frame::Frame()
	{
		delay = 0;
		opacities = { 0, 0 };
		scales = { 0, 0 };
	}

	float Animation::Frame::opcstep(uint16_t timestep) const
	{
		return timestep * static_cast<float>(opacities.second - opacities.first) / delay;
	}

	float Animation::Frame::scalestep(uint16_t timestep) const
	{
		return timestep * static_cast<float>(scales.second - scales.first) / delay;
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
			using std::set;
			set<int16_t> frameids;
			for (node sub : src)
			{
				if (sub.data_type() == node::type::bitmap)
				{
					int16_t fid = StringConversion<int16_t>(sub.name()).ordefault(-1);
					if (fid >= 0)
					{
						frameids.insert(fid);
					}
				}
			}

			for (auto& fid : frameids)
			{
				node sub = src[std::to_string(fid)];
				frames.push_back(sub);
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
		xyscale.set(frames[0].scales.first);
		delay = frames[0].delay;
		framestep = 1;
	}

	void Animation::draw(const DrawArgument& args, float alpha) const
	{
		int16_t interframe = frame.get(alpha);
		float interopc = opacity.get(alpha) / 255;
		float interscale = xyscale.get(alpha) / 100;

		bool modifyopc = interopc != 1.0f;
		bool modifyscale = interscale != 1.0f;
		if (modifyopc || modifyscale)
		{
			auto absargs = args + DrawArgument(interscale, interscale, interopc);
			frames[interframe].texture.draw(absargs);
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
		auto& framedata = frames[frame.get()];

		opacity += framedata.opcstep(timestep);
		if (opacity.last() < 0.0f)
		{
			opacity.set(0.0f);
		}
		else if (opacity.last() > 255.0f)
		{
			opacity.set(255.0f);
		}

		xyscale += framedata.scalestep(timestep);
		if (xyscale.last() < 0.0f)
		{
			opacity.set(0.0f);
		}

		if (timestep >= delay)
		{
			int16_t lastframe = static_cast<int16_t>(frames.size() - 1);
			int16_t nextframe;
			bool ended;
			if (zigzag && lastframe > 0)
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

			delay = frames[nextframe].delay;
			if (delay >= delta)
				delay -= delta;

			opacity.set(frames[nextframe].opacities.first);
			xyscale.set(frames[nextframe].scales.first);
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
