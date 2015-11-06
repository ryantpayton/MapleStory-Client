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
#include "AnimationData.h"

namespace Graphics
{
	const uint16_t DEF_DELAY = 50;

	AnimationData::AnimationData(node src)
	{
		if (src.data_type() == node::type::bitmap)
		{
			frames.add(new Texture(src));
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
				nodeit = src[std::to_string(framec)];
			}
			lastframe = static_cast<uint8_t>(frames.getend() - 1);
		}
		else
		{
			lastframe = 0;
		}
	}

	AnimationData::AnimationData()
	{
		lastframe = 0;
	}

	AnimationData::~AnimationData()
	{
	}

	uint8_t AnimationData::getlastframe() const
	{
		return lastframe;
	}

	uint8_t AnimationData::nextframe(uint8_t frame) const
	{
		return (frame == lastframe) ? 0 : frame + 1;
	}

	uint16_t AnimationData::getdelay(uint8_t frame) const
	{
		if (frame < delays.size())
		{
			return delays.at(frame);
		}
		return DEF_DELAY;
	}

	uint8_t AnimationData::getalpha(uint8_t frame) const
	{
		if (frame < alphablends.size())
		{
			return alphablends.at(frame).first;
		}
		return 255;
	}

	float AnimationData::nextalpha(uint8_t frame, float alpha) const
	{
		if (frame < alphablends.size())
		{
			return static_cast<float>(alphablends.at(frame).second) - alpha;
		}
		return alpha;
	}

	vector2d<int32_t> AnimationData::getorigin(uint8_t frame) const
	{
		Texture* txt = frames.get(frame);
		if (txt)
		{
			return txt->getorigin();
		}
		else
		{
			return vector2d<int32_t>();
		}
	}

	vector2d<int32_t> AnimationData::getdimensions(uint8_t frame) const
	{
		Texture* txt = frames.get(frame);
		if (txt)
		{
			return txt->getdimensions();
		}
		else
		{
			return vector2d<int32_t>();
		}
	}

	const Texture* AnimationData::gettexture(uint8_t frame) const
	{
		return frames.get(frame);
	}
}
