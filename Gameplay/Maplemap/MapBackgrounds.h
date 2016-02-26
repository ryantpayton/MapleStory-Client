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
#include "Constants.h"
#include "Graphics\Texture.h"
#include "Graphics\Animation.h"
#include <vector>

namespace Gameplay
{
	using Graphics::Animation;

	class Background
	{
	public:
		Background(node src);

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

		static const int16_t WOFFSET = Constants::VIEWWIDTH / 2;
		static const int16_t HOFFSET = Constants::VIEWHEIGHT / 2 - Constants::VIEWYOFFSET;

		Animation animation;
		bool animated;
		int16_t cx;
		int16_t cy;
		float rx;
		float ry;
		int16_t htile;
		int16_t vtile;
		float opacity;
		bool flipped;

		float fx;
		float fy;
		float lastx;
		float lasty;
		float hspeed;
		float vspeed;
	};

	using std::vector;

	class MapBackgrounds
	{
	public:
		MapBackgrounds(node src);
		MapBackgrounds();

		void drawbackgrounds(vector2d<int16_t> viewpos, float inter) const;
		void drawforegrounds(vector2d<int16_t> viewpos, float inter) const;
		void update();

	private:
		vector<Background> backgrounds;
		vector<Background> foregrounds;
	};
}