/////////////////////////////////////////////////////////////////////////////
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
#include "Footholdtree.h"

#include "..\..\Console.h"
#include "..\..\Graphics\GraphicsGL.h"

namespace jrc
{
	Footholdtree::Footholdtree(nl::node src)
	{
		int16_t leftw = 30000;
		int16_t rightw = -30000;
		int16_t botb = -30000;
		int16_t topb = 30000;

		for (auto basef : src)
		{
			uint8_t layer;
			try
			{
				layer = static_cast<uint8_t>(stoi(basef.name()));
			}
			catch (const std::exception& ex)
			{
				Console::get().print(__func__, ex);
				continue;
			}

			for (auto midf : basef)
			{
				for (auto lastf : midf)
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

					uint16_t id = foothold.get_id();
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

	Footholdtree::Footholdtree() {}

	void Footholdtree::limit_movement(PhysicsObject& phobj) const
	{
		if (phobj.hmobile())
		{
			double crnt_x = phobj.crnt_x();
			double next_x = phobj.next_x();

			bool left = phobj.hspeed < 0.0f;
			double wall = getwall(phobj.fhid, left, phobj.next_y());
			bool collision = left ?
				crnt_x >= wall && next_x <= wall :
				crnt_x <= wall && next_x >= wall;

			if (!collision && phobj.is_flag_set(PhysicsObject::TURNATEDGES))
			{
				wall = get_edge(phobj.fhid, left);
				collision = left ?
					crnt_x >= wall && next_x <= wall : 
					crnt_x <= wall && next_x >= wall;
			}

			if (collision)
			{
				phobj.limitx(wall);
				phobj.clear_flag(PhysicsObject::TURNATEDGES);
			}
		}

		if (phobj.vmobile())
		{
			double crnt_y = phobj.crnt_y();
			double next_y = phobj.next_y();

			auto ground = Range<double>(
				get_fh(phobj.fhid).resolvex(phobj.crnt_x()),
				get_fh(phobj.fhid).resolvex(phobj.next_x())
				);
			bool collision = crnt_y <= ground.first() && next_y >= ground.second();
			if (collision)
			{
				phobj.limity(ground.second());

				limit_movement(phobj);
			}
			else
			{
				if (next_y < borders.first())
				{
					phobj.limity(borders.first());
				}
				else if (next_y > borders.second())
				{
					phobj.limity(borders.second());
				}
			}
		}
	}

	void Footholdtree::update_fh(PhysicsObject& phobj) const
	{
		if (phobj.type == PhysicsObject::FIXATED && phobj.fhid > 0)
			return;

		const Foothold& curfh = get_fh(phobj.fhid);
		bool checkslope = false;

		double x = phobj.crnt_x();
		double y = phobj.crnt_y();
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
				phobj.fhid = get_fhid_below(x, y);
			}
			else
			{
				checkslope = true;
			}
		}
		else
		{
			phobj.fhid = get_fhid_below(x, y);
		}

		const Foothold& nextfh = get_fh(phobj.fhid);
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

		if (phobj.enablejd || phobj.is_flag_set(PhysicsObject::CHECKBELOW))
		{
			uint16_t belowid = get_fhid_below(x, nextfh.resolvex(x) + 1.0);
			if (belowid > 0)
			{
				double nextground = get_fh(belowid).resolvex(x);
				phobj.enablejd = (nextground - ground) < 600.0;
				phobj.groundbelow = ground + 1.0;
			}
			else
			{
				phobj.enablejd = false;
			}

			phobj.clear_flag(PhysicsObject::CHECKBELOW);
		}

		if (phobj.fhlayer == 0 || phobj.onground)
		{
			phobj.fhlayer = nextfh.get_layer();
		}
	}

	const Foothold& Footholdtree::get_fh(uint16_t fhid) const
	{
		return footholds.count(fhid) ? footholds.at(fhid) : nullfh;
	}

	double Footholdtree::getwall(uint16_t curid, bool left, double fy) const
	{
		auto shorty = static_cast<int16_t>(fy);
		auto vertical = Range<int16_t>(shorty - 50, shorty - 1);
		if (left)
		{
			uint16_t previd = get_fh(curid).getprev();
			if (get_fh(previd).iswall() && get_fh(previd).getver().overlaps(vertical))
			{
				return get_fh(curid).getl();
			}
			previd = get_fh(previd).getprev();
			if (get_fh(previd).iswall() && get_fh(previd).getver().overlaps(vertical))
			{
				return get_fh(previd).getl();
			}
			return walls.first();
		}
		else
		{
			uint16_t nextid = get_fh(curid).getnext();
			if (get_fh(nextid).iswall() && get_fh(nextid).getver().overlaps(vertical))
			{
				return get_fh(curid).getr();
			}
			nextid = get_fh(nextid).getnext();
			if (get_fh(nextid).iswall() && get_fh(nextid).getver().overlaps(vertical))
			{
				return get_fh(nextid).getr();
			}
			return walls.second();
		}
	}

	double Footholdtree::get_edge(uint16_t curid, bool left) const
	{
		const Foothold& fh = get_fh(curid);
		if (left)
		{
			uint16_t previd = fh.getprev();
			if (!previd)
				return fh.getl();

			const Foothold& prev = get_fh(previd);
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

			const Foothold& next = get_fh(nextid);
			nextid = next.getnext();
			if (!nextid)
				return next.getr();

			return walls.second();
		}
	}

	uint16_t Footholdtree::get_fhid_below(double fx, double fy) const
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
					ret = fh.get_id();
				}
			}
		}
		return ret;
	}

	int16_t Footholdtree::get_y_below(Point<int16_t> position) const
	{
		uint16_t fhid = get_fhid_below(position.x(), position.y());
		if (fhid > 0)
		{
			const Foothold& fh = get_fh(fhid);
			return static_cast<int16_t>(fh.resolvex(position.x()));
		}
		else
		{
			return borders.second();
		}
	}

	Range<int16_t> Footholdtree::get_walls() const
	{
		return walls;
	}

	Range<int16_t> Footholdtree::get_borders() const
	{
		return borders;
	}
}
