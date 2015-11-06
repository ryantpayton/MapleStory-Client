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
#include "Npc.h"

namespace Gameplay
{
	Npc::Npc(const Npcdata& nd, int32_t id, int32_t o, bool fl, uint16_t f, vector2d<int32_t> pos) : ndata(nd), animcont(nd.getanimation("stand"))
	{
		name = Textlabel(IO::DWF_14BC, IO::TXC_YELLOW, ndata.getname(), 0);
		func = Textlabel(IO::DWF_14BC, IO::TXC_YELLOW, ndata.getfunc(), 0);
		name.gettext().setback(IO::TXB_NAMETAG);
		func.gettext().setback(IO::TXB_NAMETAG);

		npcid = id;
		oid = o;
		flip = !fl;
		stance = "stand";
		active = true;

		setposition(pos);
		phobj.fhid = f;
	}

	void Npc::draw(vector2d<int32_t> viewpos) const
	{
		if (active)
		{
			vector2d<int32_t> absp = getposition() + viewpos;

			using::Graphics::Texture;
			using::Graphics::FlipArgument;
			const Texture* texture = ndata.gettexture(stance, animcont.getframe());
			if (texture)
			{
				texture->draw(FlipArgument(absp, flip));
			}

			if (ndata.showname())
			{
				name.gettext().draw(absp);
				func.gettext().draw(absp + vector2d<int32_t>(0, 18));
			}
		}
	}

	int8_t Npc::update(const Physics& physics, uint16_t dpf)
	{
		if (active)
		{
			physics.moveobject(phobj, dpf);

			bool aniend = animcont.update(ndata.getanimation(stance), dpf);
			size_t states = ndata.getnumstates();
			if (aniend && states > 0)
			{
				string newstate = ndata.getstate(random.nextint(states - 1));
				setstance(newstate);
			}
		}
		return phobj.fhlayer;
	}

	void Npc::setstance(string st)
	{
		if (stance != st)
		{
			stance = st;
			animcont.reset(ndata.getanimation(st));
		}
	}

	void Npc::setactive(bool a)
	{
		active = a;
	}

	void Npc::setposition(vector2d<int> pos)
	{
		phobj.fx = static_cast<float>(pos.x());
		phobj.fy = static_cast<float>(pos.y());
	}

	int32_t Npc::getoid() const
	{
		return oid;
	}

	int8_t Npc::getlayer() const
	{
		return phobj.fhlayer;
	}

	vector2d<int32_t> Npc::getposition() const
	{
		return vector2d<int32_t>(static_cast<int32_t>(phobj.fx), static_cast<int32_t>(phobj.fy));
	}

	vector2d<int32_t> Npc::getdimensions() const
	{
		return ndata.getdimensions(stance, animcont.getframe());
	}
}
