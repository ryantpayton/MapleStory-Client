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
#include "Footholdtree.h"
#include <algorithm>

namespace Gameplay
{
	Footholdtree::Footholdtree(node src)
	{
		int32_t leftw = 65536;
		int32_t rightw = -65536;
		int32_t botb = -65536;
		int32_t topb = 65536;

		for (node& basef : src)
		{
			int8_t layer = static_cast<int8_t>(stoi(basef.name()));
			for (node& midf : basef)
			{
				for (node& lastf : midf)
				{
					Foothold foothold = Foothold(lastf, layer);

					if (foothold.getl() < leftw)
					{
						leftw = foothold.getl();
					}
					else if (foothold.getr() > rightw)
					{
						rightw = foothold.getr();
					}

					if (foothold.getb() > botb)
					{
						botb = foothold.getb();
					}
					else if (foothold.gett() < topb)
					{
						topb = foothold.gett();
					}

					uint16_t id = foothold.getid();
					footholds[id] = foothold;

					int32_t start = foothold.getl();
					int32_t end = foothold.getr();
					for (int32_t i = start; i <= end; i++)
					{
						footholdsbyx.insert(std::make_pair(i, id));
					}
				}
			}
		}

		walls = vector2d<int32_t>(leftw + 25, rightw - 25);
		borders = vector2d<int32_t>(topb - 400, botb + 400);
	}

	Footholdtree::Footholdtree() {}

	Footholdtree::~Footholdtree() {}

	void Footholdtree::limitmoves(PhysicsObject& phobj) const
	{
		float nextx = phobj.fx + phobj.hspeed;
		float nexty = phobj.fy + phobj.vspeed;

		if (nextx != phobj.fx)
		{
			vector2d<int32_t> vertical = vector2d<int32_t>(
				static_cast<int32_t>(phobj.fy - 40), 
				static_cast<int32_t>(phobj.fy - 10)
				);
			float wall = getwall(phobj.fhid, phobj.hspeed < 0.0f, vertical);
			if ((phobj.hspeed < 0) ? nextx < wall : nextx > wall)
			{
				phobj.hspeed = 0.0f;
			}
		}

		if (nexty > phobj.fy)
		{
			float ground = getfh(phobj.fhid).resolvex(nextx);
			if (nexty >= ground && ground >= phobj.fy)
			{
				phobj.vspeed = 0.0f;
				phobj.fy = ground;
			}
		}

		if (nexty < borders.x())
		{
			phobj.vspeed = 0.0f;
		}
		else if (nexty > borders.y())
		{
			phobj.vspeed = 0.0f;
		}
	}

	void Footholdtree::updatefh(PhysicsObject& phobj) const
	{
		bool checkslope = false;
		if (phobj.onground)
		{
			const Foothold& curfh = getfh(phobj.fhid);

			if (phobj.fx > curfh.getr())
				phobj.fhid = curfh.getnext();
			else if (phobj.fx < curfh.getl())
				phobj.fhid = curfh.getprev();

			if (phobj.fhid == 0)
				phobj.fhid = getbelow(phobj.fx, phobj.fy);
			else
				checkslope = true;
		}
		else
		{
			phobj.fhid = getbelow(phobj.fx, phobj.fy);
		}

		const Foothold& nextfh = getfh(phobj.fhid);
		phobj.fhlayer = nextfh.getlayer();
		phobj.fhslope = nextfh.getslope();

		float ground = nextfh.resolvex(phobj.fx);
		if (phobj.vspeed == 0.0f && checkslope)
		{
			if (phobj.fhslope * abs(ground - phobj.fy) <= abs(phobj.hspeed))
			{
				phobj.fy = ground;
			}
		}
		phobj.onground = phobj.fy == ground;
	}

	const Foothold& Footholdtree::getfh(uint16_t fhid) const
	{
		if (footholds.count(fhid))
			return footholds.at(fhid);
		else
			return nullfh;
	}

	float Footholdtree::getwall(uint16_t curid, bool left, vector2d<int32_t> ver) const
	{
		if (left)
		{
			uint16_t previd = getfh(curid).getprev();
			if (footholds.count(previd))
			{
				if (getfh(previd).iswall() && getfh(previd).getver().overlaps(ver))
				{
					return static_cast<float>(getfh(curid).getl());
				}
			}
			return static_cast<float>(walls.x());
		}
		else
		{
			uint16_t nextid = getfh(curid).getnext();
			if (footholds.count(nextid))
			{
				if (getfh(nextid).iswall() && getfh(nextid).getver().overlaps(ver))
				{
					return static_cast<float>(getfh(curid).getr());
				}
			}
			return static_cast<float>(walls.y());
		}
	}

	uint16_t Footholdtree::getbelow(float fx, float fy) const
	{
		uint16_t ret = 0;
		int32_t x = static_cast<int32_t>(fx);
		if (footholdsbyx.count(x))
		{
			float comp = static_cast<float>(borders.y());
			std::for_each(footholdsbyx.lower_bound(x), footholdsbyx.upper_bound(x), 
				[this, fx, fy, &comp, &ret](const std::pair<int32_t, uint16_t>& val) {
				const Foothold& fh = footholds.at(val.second);
				float ycomp = fh.resolvex(fx);
				if (comp >= ycomp && ycomp >= fy)
				{
					ret = fh.getid();
					comp = ycomp;
				}
			});
		}
		return ret;
	}

	vector2d<int32_t> Footholdtree::getwalls() const
	{
		return walls;
	}

	vector2d<int32_t> Footholdtree::getborders() const
	{
		return borders;
	}
}
