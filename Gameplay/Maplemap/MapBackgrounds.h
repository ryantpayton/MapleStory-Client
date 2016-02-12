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
#include "Console.h"
#include "Graphics\Texture.h"
#include "Graphics\Animation.h"
#include <vector>

namespace Gameplay
{
	using Graphics::Animation;

	class Background
	{
	public:
		Background(node src, vector2d<int16_t> walls, vector2d<int16_t> borders);

		void draw(vector2d<int16_t> viewpos, float inter) const;
		void update();

	private:
		enum Type
		{
			NORMAL,
			HTILED,
			VTILED,
			TILED,
			HMOVEA,
			VMOVEA,
			HMOVEB,
			VMOVEB
		};

		static Type typebyid(int32_t id)
		{
			if (id >= NORMAL && id <= VMOVEB)
				return static_cast<Type>(id);

			Console::get().print("Unhandled background type: " + std::to_string(id));

			return NORMAL;
		}

		Animation animation;
		bool animated;
		vector2d<int16_t> rpos;
		vector2d<int16_t> cpos;
		Type type;
		float fx;
		float fy;
		float opacity;
		bool flipped;

		vector2d<int16_t> walls;
		vector2d<int16_t> borders;
	};

	using std::vector;

	class MapBackgrounds
	{
	public:
		MapBackgrounds(node src, vector2d<int16_t> walls, vector2d<int16_t> borders);
		MapBackgrounds();

		void drawbackgrounds(vector2d<int16_t> viewpos, float inter) const;
		void drawforegrounds(vector2d<int16_t> viewpos, float inter) const;
		void update();

	private:
		vector<Background> backgrounds;
		vector<Background> foregrounds;
	};
}