//////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015-2016 Daniel Allendorf                                   //
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
#include "MapObjects.h"
#include "Drop.h"

#include "..\Spawn.h"
#include "..\..\Graphics\Animation.h"

#include <array>

namespace jrc
{
	class MesoIcons
	{
	public:
		enum Type
		{
			BRONZE, GOLD, BUNDLE, BAG,
			LENGTH
		};

		void init();
		void update();

		const Animation* get(Type type) const;

	private:
		std::array<Animation, LENGTH> animations;
	};


	class MapDrops
	{
	public:
		MapDrops();

		// Draw all drops on a layer.
		void draw(int8_t layer, double viewx, double viewy, float alpha) const;
		// Update all drops.
		void update(const Physics& physics);

		// Spawn a new drop.
		void send_spawn(const DropSpawn& spawn);
		// Remove a drop.
		void remove_drop(int32_t oid, int8_t mode, const PhysicsObject* looter);
		// Remove all drops.
		void clear();

		Optional<Drop> get_drop(int32_t oid);

		// Find a drop which can be picked up at the specified position.
		const Drop* findinrange(Point<int16_t> playerpos);


		// Initialize animations of meso icons.
		static void init();

	private:
		MapObjects drops;
		bool lootenabled;

		static MesoIcons meso_icons;
	};
}