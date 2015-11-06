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
#include "Texture.h"
#include "Ptrvector.h"

namespace Graphics
{
	using::std::pair;
	using::std::vector;
	using::Util::Ptrvector;

	// Maplestory-animation-data loaded from a node of game data.
	// Objects that use the same sprite should share this data.
	class AnimationData
	{
	public:
		AnimationData(node);
		AnimationData();
		~AnimationData();
		uint8_t getlastframe() const;
		uint8_t nextframe(uint8_t) const;
		uint16_t getdelay(uint8_t) const;
		uint8_t getalpha(uint8_t) const;
		float nextalpha(uint8_t, float) const;
		vector2d<int32_t> getorigin(uint8_t) const;
		vector2d<int32_t> getdimensions(uint8_t) const;
		const Texture* gettexture(uint8_t) const;
	private:
		Ptrvector<Texture> frames;
		vector<uint16_t> delays;
		vector<pair<uint8_t, uint8_t>> alphablends;
		uint8_t lastframe;
	};
}

