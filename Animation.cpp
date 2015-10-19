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
#include "Animation.h"

namespace Graphics
{
	Animation::Animation(node src)
	{
		if (src.data_type() == node::type::bitmap)
		{
			frames.add(new Texture(src));
		}
		else if (src["0"].data_type() == node::type::bitmap)
		{
			uint8_t framec = 0;
			node nodeit = src[to_string(framec)];
			while (nodeit.data_type() == node::type::bitmap)
			{
				node delay = nodeit["delay"];
				if (delay.data_type() == node::type::integer)
				{
					delays.push_back(static_cast<short>(delay.get_integer()));
				}
				else if (delay.data_type() == node::type::string)
				{
					delays.push_back(static_cast<short>(stoi(delay.get_string())));
				}
				else
				{
					delays.push_back(DEF_DELAY);
				}

				frames.add(new Texture(nodeit));

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

				framec++;
				nodeit = src[to_string(framec)];
			}

			frame = 0;
			last_f = static_cast<uint8_t>(frames.getend() - 1);
			elapsed = 0;

			if (alphablends.size() > 0)
			{
				alpha = static_cast<float>(alphablends[0].first);
				alphastep = (alphablends[0].second - alpha) / (delays[0] / 16);
			}
		}
		else
		{
			frame = 0;
			last_f = 0;
		}
	}

	void Animation::draw(DrawArgument& args)
	{
		if (frame < frames.getend())
		{
			Texture* txt = frames.get(frame);
			txt->draw(args);
		}
	}

	void Animation::update(short dpf)
	{
		if (last_f > 0)
		{
			elapsed += dpf;

			if (blending)
			{
				alpha += alphastep;
				if (alpha < 0.f)
					alpha = 0.f;
				else if (alpha > 255.f)
					alpha = 255.f;
			}

			short delay = delays[frame];
			if (elapsed > delay)
			{
				elapsed -= delay;

				frame = (frame == last_f) ? 0 : frame + 1;

				if (blending)
				{
					float nexta = static_cast<float>(alphablends[frame].second - alpha);
					alphastep = (nexta * dpf) / delay;
				}

				//return frame == 0;
			}
		}

		//return false;
	}

	vector2d<int> Animation::getorigin()
	{
		Texture* txt = frames.get(frame);
		if (txt)
		{
			return txt->getorigin();
		}
		else
		{
			return vector2d<int>();
		}
	}

	vector2d<int> Animation::getdimension()
	{
		Texture* txt = frames.get(frame);
		if (txt)
		{
			return txt->getdimension();
		}
		else
		{
			return vector2d<int>();
		}
	}
}
