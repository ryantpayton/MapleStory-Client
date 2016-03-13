//////////////////////////////////////////////////////////////////////////////
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
#include "MapObjects.h"
#include "Drop.h"
#include "Graphics\Animation.h"
#include "Gameplay\Spawn.h"

namespace Gameplay
{
	using Graphics::Animation;

	class MapDrops : public MapObjects
	{
	public:
		MapDrops();

		void update(const Physics& physics) override;

		void sendspawn(const DropSpawn& spawn);
		void removedrop(int32_t oid, int8_t mode, const PhysicsObject* looter);
		const Drop* findinrange(Point<int16_t> playerpos);

		static void init();

	private:
		Optional<Drop> getdrop(int32_t oid);

		bool lootenabled;

		enum MesoType
		{
			BRONZE, GOLD, BUNDLE, BAG
		};
		static unordered_map<MesoType, Animation> mesoicons;
	};
}