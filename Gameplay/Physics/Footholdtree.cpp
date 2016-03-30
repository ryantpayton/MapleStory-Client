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
			uint8_t layer;
			try
			{
				layer = static_cast<uint8_t>(stoi(basef.name()));
			}
			catch (const std::exception&)
			{
				continue;
			}

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
			uint8_t layer = recv.readbyte();
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
		if (phobj.hmobile())
		{
			double crntx = phobj.crntx();
			double nextx = phobj.nextx();

			bool left = phobj.hspeed < 0.0f;
			double wall = getwall(phobj.fhid, left, phobj.nexty());
			bool collision = left ?
				crntx >= wall && nextx <= wall :
				crntx <= wall && nextx >= wall;

			if (!collision && phobj.flagset(PhysicsObject::TURNATEDGES))
			{
				wall = getedge(phobj.fhid, left);
				collision = left ?
					crntx >= wall && nextx <= wall : 
					crntx <= wall && nextx >= wall;
			}

			if (collision)
			{
				phobj.limitx(wall);
				phobj.clearflag(PhysicsObject::TURNATEDGES);
			}
		}

		if (phobj.vmobile())
		{
			double crnty = phobj.crnty();
			double nexty = phobj.nexty();

			auto ground = Range<double>(
				getfh(phobj.fhid).resolvex(phobj.crntx()),
				getfh(phobj.fhid).resolvex(phobj.nextx())
				);
			bool collision = crnty <= ground.first() && nexty >= ground.second();
			if (collision)
			{
				phobj.limity(ground.second());

				limitmoves(phobj);
			}
			else
			{
				if (nexty < borders.first())
				{
					phobj.limity(borders.first());
				}
				else if (nexty > borders.second())
				{
					phobj.limity(borders.second());
				}
			}
		}
	}

	void Footholdtree::updatefh(PhysicsObject& phobj) const
	{
		if (phobj.type == PhysicsObject::FIXATED && phobj.fhid > 0)
			return;

		const Foothold& curfh = getfh(phobj.fhid);
		bool checkslope = false;

		double x = phobj.crntx();
		double y = phobj.crnty();
		if (phobj.onground)
		{
			if (std::floor(x) > curfh.getr())
			{
				phobj.fhid = curfh.getnext();
			}
			else if (std::ceil(x) < curfh.getl())
			{
				phobj.fhid = curfh.getprev();
			}

			if (phobj.fhid == 0)
			{
				phobj.fhid = getbelow(x, y);
			}
			else
			{
				checkslope = true;
			}
		}
		else
		{
			phobj.fhid = getbelow(x, y);
		}

		const Foothold& nextfh = getfh(phobj.fhid);
		phobj.fhslope = nextfh.getslope();

		double ground = nextfh.resolvex(x);
		if (phobj.vspeed == 0.0 && checkslope)
		{
			double vdelta = abs(phobj.fhslope);
			if (phobj.fhslope < 0.0)
			{
				vdelta *= (ground - y);
			}
			else if (phobj.fhslope > 0.0)
			{
				vdelta *= (y - ground);
			}

			if (curfh.getslope() != 0.0 || nextfh.getslope() != 0.0)
			{
				if (phobj.hspeed > 0.0 && vdelta <= phobj.hspeed)
				{
					phobj.y = ground;
				}
				else if (phobj.hspeed < 0.0 && vdelta >= phobj.hspeed)
				{
					phobj.y = ground;
				}
			}
		}

		phobj.onground = phobj.y == ground;

		uint16_t belowid = getbelow(x, nextfh.resolvex(x) + 1.0);
		if (belowid > 0)
		{
			double nextground = getfh(belowid).resolvex(x);
			phobj.enablejd = (nextground - ground) < 600.0;
			phobj.groundbelow = ground + 1.0;
		}
		else
		{
			phobj.enablejd = false;
		}

		if (phobj.fhlayer == 0 || phobj.onground)
		{
			phobj.fhlayer = nextfh.getlayer();
		}
	}

	const Foothold& Footholdtree::getfh(uint16_t fhid) const
	{
		return footholds.count(fhid) ? footholds.at(fhid) : nullfh;
	}

	double Footholdtree::getwall(uint16_t curid, bool left, double fy) const
	{
		auto shorty = static_cast<int16_t>(fy);
		auto vertical = Range<int16_t>(shorty - 50, shorty - 1);
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
		const Foothold& fh = getfh(curid);
		if (left)
		{
			uint16_t previd = fh.getprev();
			if (!previd)
				return fh.getl();

			const Foothold& prev = getfh(previd);
			previd = prev.getprev();
			if (!previd)
				return prev.getl();

			return walls.first();
		}
		else
		{
			uint16_t nextid = fh.getnext();
			if (!nextid)
				return fh.getr();

			const Foothold& next = getfh(nextid);
			nextid = next.getnext();
			if (!nextid)
				return next.getr();

			return walls.second();
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
