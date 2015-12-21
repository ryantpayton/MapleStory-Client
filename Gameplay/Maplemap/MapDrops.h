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
#include "MapObjects.h"
#include "Drop.h"
#include "Graphics\Animation.h"

namespace Gameplay
{
	using Graphics::Animation;

	class MapDrops : public MapObjects
	{
	public:
		MapDrops();

		void update(const Physics& physics) override;

		void adddrop(int32_t oid, int32_t itemid, bool meso, int32_t owner,
			vector2d<int16_t> pos, vector2d<int16_t> dest, int8_t type, int8_t mod);
		void removedrop(int32_t oid, int8_t mode, const PhysicsObject* looter);
		const Drop* findinrange(rectangle2d<int16_t> range);

	private:
		enum MesoType
		{
			MES_BRONZE = 0,
			MES_GOLD = 1,
			MES_BUNDLE = 2,
			MES_BAG = 3
		};

		map<MesoType, Animation> mesos;
		bool lootenabled;
	};
}