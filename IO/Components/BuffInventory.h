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
#include "Graphics\Texture.h"
#include <unordered_map>

namespace IO
{
	using std::unordered_map;
	using Util::vector2d;
	using Graphics::Texture;

	struct BuffIcon
	{
		const Texture* texture;
		int32_t duration;
	};

	class BuffInventory
	{
	public:
		BuffInventory();
		~BuffInventory();

		void draw(vector2d<int16_t> position, float inter) const;
		void update();

		void addbuff(int32_t buffid, int32_t duration);
		void cancelbuff(int32_t buffid);

	private:
		unordered_map<int32_t, BuffIcon> icons;
	};
}