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
#include "Animation.h"
#include "Program\Constants.h"

namespace Graphics
{
	const uint16_t DEF_DELAY = 50;

	Animation::Animation(node src)
	{
		if (src.data_type() == node::type::bitmap)
		{
			textures.push_back(Texture(src));
		}
		else if (src["0"].data_type() == node::type::bitmap)
		{
			uint8_t framec = 0;
			node nodeit = src[std::to_string(framec)];
			while (nodeit.data_type() == node::type::bitmap)
			{
				node delay = nodeit["delay"];
				if (delay.data_type() == node::type::integer)
				{
					delays.push_back(static_cast<uint16_t>(delay.get_integer()));
				}
				else if (delay.data_type() == node::type::string)
				{
					delays.push_back(static_cast<uint16_t>(stoi(delay.get_string())));
				}
				else
				{
					delays.push_back(DEF_DELAY);
				}

				textures.push_back(Texture(nodeit));

				node a0_node = nodeit["a0"];
				if (a0_node.data_type() == node::type::integer)
				{
					uint8_t a0 = static_cast<uint8_t>(a0_node.get_integer());
					node a1_node = nodeit["a1"];
					if (a1_node.data_type() == node::type::integer)
					{
						alphablends.push_back(pair<uint8_t, uint8_t>(a0, static_cast<uint8_t>(a1_node.get_integer())));
					}
					else
					{
						alphablends.push_back(pair<uint8_t, uint8_t>(a0, 255 - a0));
					}
				}
				else
				{
					alphablends.push_back(pair<uint8_t, uint8_t>(255, 255));
				}

				framec++;
				nodeit = src[std::to_string(framec)];
			}

			alpha = static_cast<float>(alphablends[0].first);
			alphastep = nextalphastep(0, Constants::TIMESTEP);
			lastalpha = alpha;
		}

		elapsed = 0;
		frame = 0;
		lastframe = frame;
		animated = textures.size() > 1;
	}

	Animation::Animation() 
	{
		animated = false;
	}

	void Animation::draw(const DrawArgument& args, float inter) const
	{
		uint8_t interframe;
		float interalpha;
		if (animated)
		{
			if (lastelapsed + Constants::TIMESTEP * inter > delays[lastframe])
				interframe = frame;
			else
				interframe = lastframe;

			interalpha = (1.0f - inter) * lastalpha + inter * alpha;
		}
		else
		{
			interframe = 0;
			interalpha = 255.0f;
		}

		if (args.getalpha() == 1.0f && interalpha != 255.0f)
			textures[interframe].draw(args.overwritealpha(interalpha / 255));
		else
			textures[interframe].draw(args);
	}

	void Animation::reset()
	{
		if (animated)
		{
			lastframe = frame;
			lastalpha = alpha;

			elapsed = 0;
			frame = 0;
			alpha = static_cast<float>(alphablends[0].first);
			alphastep = nextalphastep(0, Constants::TIMESTEP);
		}
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

			elapsed += timestep;

			alpha += alphastep;
			if (alpha < 0.0f)
				alpha = 0.0f;
			else if (alpha > 255.0f)
				alpha = 255.0f;

			uint16_t delay = delays[frame];
			if (elapsed > delay)
			{
				elapsed -= delay;
				frame = (frame == textures.size() - 1) ? 0 : frame + 1;

				alphastep = nextalphastep(frame, timestep);
				return frame == 0;
			}
		}
		return false;
	}

	vector2d<int32_t> Animation::getorigin() const
	{
		return textures[frame].getorigin();
	}

	vector2d<int32_t> Animation::getdimensions() const
	{
		return textures[frame].getdimensions();
	}

	float Animation::nextalphastep(uint8_t frm, uint16_t timestep) const
	{
		return (static_cast<float>(alphablends[frm].second) - alpha) * timestep / delays[frm];
	}
}
