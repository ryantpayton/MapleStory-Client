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
#include "Graphics\GraphicsGL.h"

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
					Foothold foothold = Foothold(lastf);

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
					layersbyfh[id] = layer;

					if (!foothold.iswall())
					{
						int16_t start = foothold.getl();
						int16_t end = foothold.getr();
						for (int16_t i = start; i <= end; i++)
						{
							footholdsbyx.insert(std::make_pair(i, id));
						}
					}
				}
			}
		}

		walls = Range<int16_t>(leftw + 25, rightw - 25);
		borders = Range<int16_t>(topb - 300, botb + 80);
	}

	void Footholdtree::draw(Point<int16_t>) const
	{
		/*for (auto& fhit : footholds)
		{
			const Foothold& fh = fhit.second;
			Range<int16_t> hor = fh.gethor();
			Range<int16_t> ver = fh.getver();
			int16_t l = pos.x() + hor.first();
			int16_t r = pos.x() + hor.second() + 1;
			int16_t t = pos.y() + ver.first();
			int16_t b = pos.y() + ver.second() + 1;
			Graphics::GraphicsGL::get().drawrectangle(l, t, r - l, b - t, 1.0, 0.0, 0.0, 1.0);
		}*/
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
					Foothold foothold = Foothold(recv);

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
					layersbyfh[id] = layer;

					if (abs(foothold.getslope()) < 0.5)
					{
						int16_t start = foothold.getl();
						int16_t end = foothold.getr();
						for (int16_t i = start; i <= end; i++)
						{
							footholdsbyx.insert(std::make_pair(i, id));
						}
					}
				}
			}
		}

		walls = Range<int16_t>(leftw + 25, rightw - 25);
		borders = Range<int16_t>(topb - 400, botb + 400);
	}

	Footholdtree::Footholdtree() {}

	Footholdtree::~Footholdtree() {}

	void Footholdtree::limitmoves(PhysicsObject& phobj) const
	{
		double nextx = phobj.fx + phobj.hspeed;
		double nexty = phobj.fy + phobj.vspeed;
		if (nexty > phobj.fy)
		{
			Range<double> ground = Range<double>(
				getfh(phobj.fhid).resolvex(phobj.fx),
				getfh(phobj.fhid).resolvex(nextx)
				);
			if (phobj.fy <= ground.first() && nexty >= ground.second())
			{
				phobj.vspeed = 0.0f;
				phobj.fy = ground.second();
				nexty = phobj.fy;
			}
		}

		if (nextx != phobj.fx)
		{
			bool left = phobj.hspeed < 0.0f;
			double wall = getwall(phobj.fhid, left, nexty);
			if (left)
			{
				if (phobj.fx >= wall && nextx <= wall)
				{
					phobj.hspeed = 0.0f;
					phobj.fx = wall;
				}
			}
			else
			{
				if (phobj.fx <= wall && nextx >= wall)
				{
					phobj.hspeed = 0.0f;
					phobj.fx = wall;
				}
			}

			if (phobj.turnatedges)
			{
				double edge = getedge(phobj.fhid, left);
				if (left ? nextx < edge : nextx > edge)
				{
					phobj.hspeed = 0.0f;
				}
			}
		}

		if (nexty < borders.first())
		{
			phobj.vspeed = 0.0f;
		}
		else if (nexty > borders.second())
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
			if (std::floor(phobj.fx) > curfh.getr())
				phobj.fhid = curfh.getnext();
			else if (std::ceil(phobj.fx) < curfh.getl())
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
		phobj.fh = &nextfh;

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

		uint16_t belowid = getbelow(phobj.fx, nextfh.resolvex(phobj.fx) + 1.0);
		if (belowid > 0)
		{
			double jddelta = getfh(belowid).resolvex(phobj.fx) - ground;
			phobj.enablejd = jddelta < 250.0;
		}
		else
		{
			phobj.enablejd = false;
		}

		if (phobj.fhlayer == 0 || phobj.onground)
		{
			if (layersbyfh.count(phobj.fhid))
				phobj.fhlayer = layersbyfh.at(phobj.fhid);
		}
	}

	const Foothold& Footholdtree::getfh(uint16_t fhid) const
	{
		return footholds.count(fhid) ? footholds.at(fhid) : nullfh;
	}

	double Footholdtree::getwall(uint16_t curid, bool left, double fy) const
	{
		Range<int16_t> vertical = Range<int16_t>(
			static_cast<int16_t>(fy - 50),
			static_cast<int16_t>(fy - 1)
			);
		if (left)
		{
			uint16_t previd = getfh(curid).getprev();
			if (getfh(previd).iswall() && getfh(previd).getver().overlaps(vertical))
			{
				return getfh(curid).getl();
			}
			previd = getfh(previd).getprev();
			if (getfh(previd).iswall() && getfh(previd).getver().overlaps(vertical))
			{
				return getfh(previd).getl();
			}
			return walls.first();
		}
		else
		{
			uint16_t nextid = getfh(curid).getnext();
			if (getfh(nextid).iswall() && getfh(nextid).getver().overlaps(vertical))
			{
				return getfh(curid).getr();
			}
			nextid = getfh(nextid).getnext();
			if (getfh(nextid).iswall() && getfh(nextid).getver().overlaps(vertical))
			{
				return getfh(nextid).getr();
			}
			return walls.second();
		}
	}

	double Footholdtree::getedge(uint16_t curid, bool left) const
	{
		if (left)
		{
			uint16_t previd = getfh(curid).getprev();
			if (previd)
				return walls.first();

			return getfh(curid).getl();
		}
		else
		{
			uint16_t nextid = getfh(curid).getnext();
			if (nextid)
				return walls.second();

			return getfh(curid).getr();
		}
	}

	uint16_t Footholdtree::getbelow(double fx, double fy) const
	{
		uint16_t ret = 0;
		int16_t x = static_cast<int16_t>(fx);
		if (footholdsbyx.count(x))
		{
			double comp = borders.second();
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

	int16_t Footholdtree::getgroundbelow(Point<int16_t> position) const
	{
		uint16_t fhid = getbelow(position.x(), position.y());
		if (fhid > 0)
		{
			const Foothold& fh = getfh(fhid);
			return static_cast<int16_t>(fh.resolvex(position.x()));
		}
		else
		{
			return borders.second();
		}
	}

	Range<int16_t> Footholdtree::getwalls() const
	{
		return walls;
	}

	Range<int16_t> Footholdtree::getborders() const
	{
		return borders;
	}
}
