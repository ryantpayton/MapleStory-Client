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
		int16_t leftw = 30000;
		int16_t rightw = -30000;
		int16_t botb = -30000;
		int16_t topb = 30000;

		for (node basef : src)
		{
			int8_t layer = static_cast<int8_t>(stoi(basef.name()));
			for (node midf : basef)
			{
				for (node lastf : midf)
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

					int16_t start = foothold.getl();
					int16_t end = foothold.getr();
					for (int16_t i = start; i <= end; i++)
					{
						footholdsbyx.insert(std::make_pair(i, id));
					}
				}
			}
		}

		walls = vector2d<int16_t>(leftw + 25, rightw - 25);
		borders = vector2d<int16_t>(topb - 400, botb + 400);
	}

	Footholdtree::Footholdtree(InPacket& recv)
	{
		int16_t leftw = 30000;
		int16_t rightw = -30000;
		int16_t botb = -30000;
		int16_t topb = 30000;

		uint16_t numbase = recv.readshort();
		for (uint16_t i = 0; i < numbase; i++)
		{
			int8_t layer = recv.readbyte();
			uint16_t nummid = recv.readshort();
			for (uint16_t j = 0; j < nummid; j++)
			{
				uint16_t numlast = recv.readshort();
				for (uint16_t k = 0; k < numlast; k++)
				{
					Foothold foothold = Foothold(recv, layer);

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

					int16_t start = foothold.getl();
					int16_t end = foothold.getr();
					for (int16_t i = start; i <= end; i++)
					{
						footholdsbyx.insert(std::make_pair(i, id));
					}
				}
			}
		}

		walls = vector2d<int16_t>(leftw + 25, rightw - 25);
		borders = vector2d<int16_t>(topb - 400, botb + 400);
	}

	Footholdtree::Footholdtree() {}

	Footholdtree::~Footholdtree() {}

	void Footholdtree::limitmoves(PhysicsObject& phobj) const
	{
		double nextx = phobj.fx + phobj.hspeed;
		double nexty = phobj.fy + phobj.vspeed;

		if (nextx != phobj.fx)
		{
			vector2d<int16_t> vertical = vector2d<int16_t>(
				static_cast<int16_t>(phobj.fy - 40),
				static_cast<int16_t>(phobj.fy - 10)
				);
			double wall = getwall(phobj.fhid, phobj.hspeed < 0.0f, vertical);
			if ((phobj.hspeed < 0) ? nextx < wall : nextx > wall)
			{
				phobj.hspeed = 0.0f;
			}
		}

		if (nexty > phobj.fy)
		{
			double ground = getfh(phobj.fhid).resolvex(nextx);
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
		const Foothold& curfh = getfh(phobj.fhid);
		bool checkslope = false;

		if (phobj.onground)
		{
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
		phobj.fhslope = nextfh.getslope();

		double ground = nextfh.resolvex(phobj.fx);
		if (phobj.vspeed == 0.0 && checkslope)
		{
			double vdelta = abs(phobj.fhslope);
			if (phobj.fhslope < 0.0f)
				vdelta *= (ground - phobj.fy);
			else if (phobj.fhslope > 0.0f)
				vdelta *= (phobj.fy - ground);

			if (curfh.getslope() != 0.0 || nextfh.getslope() != 0.0)
			{
				if (phobj.hspeed > 0.0f && vdelta <= phobj.hspeed)
					phobj.fy = ground;
				else if (phobj.hspeed < 0.0f && vdelta >= phobj.hspeed)
					phobj.fy = ground;
			}
		}

		phobj.onground = phobj.fy == ground;

		if (phobj.fhlayer == 0 || phobj.onground)
			phobj.fhlayer = nextfh.getlayer();
	}

	const Foothold& Footholdtree::getfh(uint16_t fhid) const
	{
		if (footholds.count(fhid))
			return footholds.at(fhid);
		else
			return nullfh;
	}

	double Footholdtree::getwall(uint16_t curid, bool left, vector2d<int16_t> ver) const
	{
		if (left)
		{
			uint16_t previd = getfh(curid).getprev();
			if (footholds.count(previd))
			{
				if (getfh(previd).iswall() && getfh(previd).getver().overlaps(ver))
				{
					return static_cast<double>(getfh(curid).getl());
				}
			}
			return static_cast<double>(walls.x());
		}
		else
		{
			uint16_t nextid = getfh(curid).getnext();
			if (footholds.count(nextid))
			{
				if (getfh(nextid).iswall() && getfh(nextid).getver().overlaps(ver))
				{
					return static_cast<double>(getfh(curid).getr());
				}
			}
			return static_cast<double>(walls.y());
		}
	}

	uint16_t Footholdtree::getbelow(double fx, double fy) const
	{
		uint16_t ret = 0;
		int16_t x = static_cast<int16_t>(fx);
		if (footholdsbyx.count(x))
		{
			double comp = static_cast<double>(borders.y());
			for (auto fhit = footholdsbyx.lower_bound(x); fhit != footholdsbyx.upper_bound(x); ++fhit)
			{
				const Foothold& fh = footholds.at(fhit->second);
				double ycomp = fh.resolvex(fx);
				if (comp >= ycomp && ycomp >= fy)
				{
					comp = ycomp;
					ret = fh.getid();
				}
			}
		}
		return ret;
	}

	int16_t Footholdtree::getgroundbelow(vector2d<int16_t> position) const
	{
		uint16_t fhid = getbelow(
			static_cast<double>(position.x()), 
			static_cast<double>(position.y())
			);

		const Foothold& fh = getfh(fhid);
		if (fh.getid() > 0)
			return static_cast<int16_t>(fh.resolvex(static_cast<double>(position.x())));
		else
			return borders.y();
	}

	vector2d<int16_t> Footholdtree::getwalls() const
	{
		return walls;
	}

	vector2d<int16_t> Footholdtree::getborders() const
	{
		return borders;
	}
}
