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
#include "Mob.h"
#include "nlnx\nx.hpp"

namespace Gameplay
{
	const int32_t DAMAGECAP = 999999;
	const float MONSTERSPEED = 0.4f;

	Mob::Mob(int32_t oi, int32_t mid, bool c, int8_t st, uint16_t fh, int8_t eff, bool newspawn, int8_t tm, int32_t x, int32_t y)
	{
		string path = std::to_string(mid);
		path.insert(0, 7 - path.size(), '0');

		node src = nl::nx::mob[path + ".img"];

		node info = src["info"];
		level = info["level"];
		watk = info["PADamage"];
		matk = info["MADamage"];
		wdef = info["PDDamage"];
		mdef = info["MDDamage"];
		accuracy = info["acc"];
		avoid = info["eva"];
		knockback = (info["pushed"].get_integer() > 0) ? info["pushed"] : 1;
		speed = info["speed"];
		speed += 100;
		touchdamage = info["bodyAttack"].get_bool();
		undead = info["undead"].get_bool();

		parsestance(STAND, src["stand"]);
		parsestance(MOVE, src["move"]);
		parsestance(HIT, src["hit1"]);
		parsestance(DIE, src["die1"]);

		name = nl::nx::string["Mob.img"][std::to_string(mid)]["name"];

		/*node sndsrc = nx::nodes["Sound"]["Mob.img"][path];
		parsesound(MBS_HIT, sndsrc["Damage"]);
		parsesound(MBS_DIE, sndsrc["Die"]);*/

		oid = oi;
		id = mid;
		control = c;
		stance = static_cast<Stance>(st);
		team = tm;

		phobj.fhid = fh;
		setposition(x, y);

		active = true;
		hppercent = 0;
		moved = 0;
		flip = (stance % 2) == 1;
		walkforce = static_cast<float>(speed) * MONSTERSPEED / 100;
		fading = false;

		namelabel = Textlabel(Textlabel::DWF_14MC, Textlabel::TXC_WHITE, name, 0);
		namelabel.setback(Textlabel::TXB_NAMETAG);

		if (control)
		{
			if (newspawn)
			{
				stance = STAND;
			}
			else
			{
				stance = MOVE;
				nextmove();
			}
		}
	}

	void Mob::parsestance(Stance stance, node src)
	{
		if (src.size() > 0)
		{
			animations[stance] = Animation(src);
			bounds[stance] = rectangle2d<int32_t>(src["0"]["lt"], src["0"]["rb"]);
		}
	}

	/*void Mob::parsesound(mobstate state, node src)
	{
		if (src.data_type() == node::type::audio)
		{
			audio snd = src.get_audio();
			Game::getaudio()->cachesound((void*)snd.data(), snd.length(), snd.id());
			sounds[state] = snd.id();
		}
	}*/

	void Mob::nextmove()
	{
		moved = 0;

		if (stance == MOVE && randomizer.nextbool())
		{
			setstance(STAND);
		}
		else
		{
			flip = randomizer.nextbool();
			setstance(MOVE);
		}
	}

	void Mob::setstance(Stance st)
	{
		if (stance != st)
		{
			stance = st;

			if (animations.count(stance))
				animations.at(stance).reset();
		}
	}

	int8_t Mob::update(const Physics& physics)
	{
		if (active)
		{
			if (animations.count(stance))
				animations.at(stance).update();

			if (control)
			{
				if (moved > 200)
					nextmove();

				switch (stance)
				{
				case MOVE:
					phobj.hforce = flip ? walkforce : -walkforce;
				}

				physics.moveobject(phobj);
				moved++;
			}
		}

		return phobj.fhlayer;
	}

	void Mob::draw(vector2d<int32_t> viewpos, float inter) const
	{
		if (active)
		{
			vector2d<int32_t> absp = phobj.getposition(inter) + viewpos;
			if (animations.count(stance))
			{
				using::Graphics::DrawArgument;
				animations.at(stance).draw(DrawArgument(absp, flip), inter);
			}

			if (hppercent > 0)
			{
				namelabel.draw(absp);
			}
		}
	}

	void Mob::kill(int8_t animation)
	{
		switch (animation)
		{
		case 0:
			active = false;
			break;
		case 1:
			//mdata->playsound(MBS_DIE);
			setstance(DIE);
			break;
		case 2:
			fading = true;
			break;
		}
	}

	void Mob::makeactive()
	{
		active = true;
	}

	void Mob::sendhp(int8_t percent, uint16_t playerlevel)
	{
		if (hppercent == 0)
		{
			int16_t delta = playerlevel - level;
			if (delta > 9)
				namelabel.setcolor(Textlabel::TXC_YELLOW);
			else if (delta < -9)
				namelabel.setcolor(Textlabel::TXC_RED);
		}
		hppercent = percent;
	}

	void Mob::setposition(int32_t x, int32_t y)
	{
		phobj.fx = static_cast<float>(x);
		phobj.fy = static_cast<float>(y);
	}

	int8_t Mob::getlayer() const
	{
		return phobj.fhlayer;
	}

	int32_t Mob::getoid() const
	{
		return oid;
	}

	vector2d<int32_t> Mob::getposition() const
	{
		return vector2d<int32_t>(static_cast<int32_t>(phobj.fx), static_cast<int32_t>(phobj.fy));
	}
}
