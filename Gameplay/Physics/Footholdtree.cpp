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

namespace Gameplay
{
	Footholdtree::Footholdtree(node src)
	{
		for (node basef = src.begin(); basef != src.end(); ++basef)
		{
			int8_t layer = static_cast<int8_t>(stoi(basef.name()));
			for (node midf = basef.begin(); midf != basef.end(); ++midf)
			{
				for (node lastf = midf.begin(); lastf != midf.end(); ++lastf)
				{
					uint16_t id = stoi(lastf.name());
					footholds[id] = Foothold(id, layer, lastf);

					if (footholds[id].getprev() == 0)
					{
						edgesl.push_back(id);
					}
					else if (footholds[id].getnext() == 0)
					{
						edgesr.push_back(id);
					}
				}
			}
		}

		int32_t leftw = 65536;
		for (vector<uint16_t>::iterator fhit = edgesl.begin(); fhit != edgesl.end(); ++fhit)
		{
			int32_t lit = footholds[*fhit].getl();
			if (lit < leftw)
			{
				leftw = lit;
			}
		}
		int32_t rightw = -65536;
		for (vector<uint16_t>::iterator fhit = edgesr.begin(); fhit != edgesr.end(); ++fhit)
		{
			int32_t rit = footholds[*fhit].getr();
			if (rit > rightw)
			{
				rightw = rit;
			}
		}
		walls = vector2d<int32_t>(leftw + 25, rightw - 25);

		int32_t botb = -65536;
		for (map<uint16_t, Foothold>::iterator fhit = footholds.begin(); fhit != footholds.end(); ++fhit)
		{
			int32_t btit = fhit->second.getb();
			if (btit > botb)
			{
				botb = btit;
			}
		}
		int32_t topb = 65536;
		for (map<uint16_t, Foothold>::iterator fhit = footholds.begin(); fhit != footholds.end(); ++fhit)
		{
			int32_t tpit = fhit->second.gett();
			if (tpit < topb)
			{
				topb = tpit;
			}
		}
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
			vector2d<int32_t> vertical = vector2d<int32_t>(static_cast<int32_t>(phobj.fy - 40), static_cast<int32_t>(phobj.fy - 10));
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
		if (phobj.fhid == 0 || !phobj.onground)
		{
			phobj.fhid = getbelow(phobj.fx, phobj.fy);
		}
		else
		{
			const Foothold& curfh = getfh(phobj.fhid);
			if (phobj.fx > curfh.getr())
			{
				phobj.fhid = getnext(phobj.fhid, false, phobj.fx, phobj.fy);
			}
			else if (phobj.fx < curfh.getl())
			{
				phobj.fhid = getnext(phobj.fhid, true, phobj.fx, phobj.fy);
			}

			if (phobj.fhid == 0)
			{
				phobj.fhid = getbelow(phobj.fx, phobj.fy);
			}
			else
			{
				checkslope = true;
			}
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

	const Foothold& Footholdtree::getfh(uint16_t fid) const
	{
		return footholds.count(fid) ? footholds.at(fid) : nullfh;
	}

	int32_t Footholdtree::getlwall(vector2d<int32_t> pos) const
	{
		int32_t ret = walls.x();
		vector2d<int32_t> ver = vector2d<int32_t>(pos.y() - 80, pos.y() - 20);
		for (map<uint16_t, Foothold>::const_iterator ftit = footholds.begin(); ftit != footholds.end(); ++ftit)
		{
			Foothold fh = ftit->second;
			if (fh.getver().overlaps(ver) && fh.iswall())
			{
				int32_t x = fh.getl();
				if (x > ret && x <= pos.x())
				{
					ret = x;
				}
			}
		}
		return ret;
	}

	int32_t Footholdtree::getrwall(vector2d<int32_t> pos) const
	{
		int32_t ret = walls.y();
		vector2d<int32_t> ver = vector2d<int32_t>(pos.y() - 80, pos.y() - 20);
		for (map<uint16_t, Foothold>::const_iterator ftit = footholds.begin(); ftit != footholds.end(); ++ftit)
		{
			Foothold fh = ftit->second;

			if (fh.getver().overlaps(ver) && fh.iswall())
			{
				int32_t x = fh.getl();

				if (x < ret && x >= pos.x())
				{
					ret = x;
				}
			}
		}
		return ret;
	}

	float Footholdtree::getwall(uint16_t curid, bool left, vector2d<int32_t> ver) const
	{
		if (left)
		{
			if (footholds.count(curid))
			{
				uint16_t previd = footholds.at(curid).getprev();
				if (footholds.count(previd))
				{
					if (footholds.at(previd).iswall() && footholds.at(previd).getver().overlaps(ver))
					{
						return static_cast<float>(footholds.at(curid).getl());
					}
				}
			}
			return static_cast<float>(walls.x());
		}
		else
		{
			if (footholds.count(curid))
			{
				uint16_t nextid = footholds.at(curid).getnext();
				if (footholds.count(nextid))
				{
					if (footholds.at(nextid).iswall() && footholds.at(nextid).getver().overlaps(ver))
					{
						return static_cast<float>(footholds.at(curid).getr());
					}
				}
			}
			return static_cast<float>(walls.y());
		}
	}

	uint16_t Footholdtree::getnext(uint16_t curid, bool left, float fx, float fy) const
	{
		if (footholds.count(curid))
		{
			uint16_t nextid = left ? footholds.at(curid).getprev() : footholds.at(curid).getnext();
			if (footholds.count(nextid))
			{
				return nextid;
			}
		}
		return 0;
	}

	uint16_t Footholdtree::getbelow(float fx, float fy) const
	{
		uint16_t ret = 0;
		float comp = static_cast<float>(borders.y());
		for (map<uint16_t, Foothold>::const_iterator fhit = footholds.begin(); fhit != footholds.end(); ++fhit)
		{
			if (fhit->second.hcontains(static_cast<int32_t>(fx)))
			{
				float ycomp = fhit->second.resolvex(fx);
				if (comp > ycomp && ycomp >= fy)
				{
					ret = fhit->second.getid();
					comp = ycomp;
				}
			}
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
