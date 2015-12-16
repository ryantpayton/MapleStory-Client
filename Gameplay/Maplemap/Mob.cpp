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
#include "Program\Constants.h"
#include "Net\Session.h"
#include "Net\Packets\GameplayPackets83.h"
#include "nlnx\nx.hpp"

namespace Gameplay
{
	const int32_t DAMAGECAP = 999999;
	const float MONSTERSPEED = 0.4f;

	Mob::Mob(int32_t oi, int32_t mid, int32_t, int8_t st, uint16_t fh, 
		bool newspawn, int8_t tm, int16_t x, int16_t y) {

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
		knockback = info["pushed"];
		if (knockback < 1)
			knockback = 1;
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
		team = tm;

		phobj.fhid = fh;
		setposition(x, y);

		active = true;
		hppercent = 0;
		flip = (st % 2) == 1;
		if (flip)
			st -= 1;
		stance = static_cast<Stance>(st);
		fading = false;

		behaviour = SeededState(randomizer.nextint(200000000), 3, STOP);
		counter = 0;

		namelabel = Textlabel(Textlabel::DWF_14MC, Textlabel::TXC_WHITE, name, 0);
		namelabel.setback(Textlabel::TXB_NAMETAG);

		if (newspawn)
		{
			fadein = true;
			alpha = 0.0f;
		}
	}

	void Mob::parsestance(Stance stance, node src)
	{
		if (src.size() > 0)
		{
			animations[stance] = Animation(src);
			bounds[stance] = rectangle2d<int16_t>(src["0"]["lt"], src["0"]["rb"]);
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
		if (!active)
			return phobj.fhlayer;

		if (fading)
		{
			alpha -= 0.05f;
			if (alpha < 0.05f)
			{
				alpha = 0.0f;
				fading = false;
			}
		}
		else if (fadein)
		{
			alpha += 0.05f;
			if (alpha > 0.95f)
			{
				alpha = 1.0f;
				fadein = false;
			}
		}

		bool aniend;
		if (animations.count(stance))
			aniend = animations.at(stance).update();
		else 
			aniend = true;

		if (aniend && stance == DIE)
		{
			active = false;
			return -1;
		}

		size_t remove = 0;
		for (auto& dmg : damagenumbers)
		{
			if (dmg.update())
				remove++;
		}

		for (size_t i = remove; i--;)
		{
			damagenumbers.erase(damagenumbers.begin());
		}

		switch (behaviour.getstate())
		{
		case MOVELEFT:
			phobj.hforce = -static_cast<float>(speed) * MONSTERSPEED / 100;
			flip = false;
			setstance(MOVE);
			break;
		case MOVERIGHT:
			phobj.hforce = static_cast<float>(speed) * MONSTERSPEED / 100;
			flip = true;
			setstance(MOVE);
			break;
		case STOP:
			setstance(STAND);
			break;
		}

		counter++;
		if (counter > 200)
		{
			behaviour.nextstate();
			counter = 0;
		}

		physics.moveobject(phobj);

		return phobj.fhlayer;
	}

	void Mob::draw(vector2d<int16_t> viewpos, float inter) const
	{
		if (!active)
			return;

		vector2d<int16_t> absp = phobj.getposition(inter) + viewpos;
		if (animations.count(stance))
		{
			using Graphics::DrawArgument;
			animations.at(stance).draw(DrawArgument(absp, flip), inter);
		}

		if (hppercent > 0)
		{
			namelabel.draw(absp);
		}

		for (auto& dmg : damagenumbers)
		{
			dmg.draw(viewpos);
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

	void Mob::setposition(int16_t x, int16_t y)
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

	vector2d<int16_t> Mob::getposition() const
	{
		return vector2d<int16_t>(
			static_cast<int16_t>(phobj.fx),
			static_cast<int16_t>(phobj.fy)
			);
	}

	bool Mob::isactive() const
	{
		return active && stance != DIE;
	}

	bool Mob::isinrange(const rectangle2d<int16_t>& range) const
	{
		if (bounds.count(stance))
		{
			rectangle2d<int16_t> curbounds = bounds.at(stance);
			curbounds.shift(getposition());
			return range.overlaps(curbounds);
		}
		else
			return false;
	}

	vector<int32_t> Mob::damage(const Attack& attack)
	{
		int16_t leveldelta = level - attack.playerlevel;
		if (leveldelta < 0)
			leveldelta = 0;
		float hitchance = static_cast<float>(attack.accuracy) / 
			((1.84f + 0.07f * leveldelta) * avoid) - 1;

		int32_t mindamage = static_cast<int32_t>(
			attack.mindamage * (1 - 0.01f * leveldelta) - wdef * 0.5f);
		int32_t maxdamage = static_cast<int32_t>(
			attack.maxdamage * (1 - 0.01f * leveldelta) - wdef * 0.6f);

		int64_t totaldamage = 0;
		vector<int32_t> damagelines;
		int16_t yshift = 0;
		float alphashift = 0.0f;
		vector2d<int16_t> startpos = getposition() - 
			vector2d<int16_t>(0, animations.at(stance).getdimensions().y());

		for (int32_t i = 0; i < attack.hitcount; i++)
		{
			auto singledamage = randomdamage(
				mindamage, maxdamage, hitchance, attack.critical);

			damagelines.push_back(singledamage.first);
			totaldamage += singledamage.first;

			damagenumbers.push_back(DamageNumber(
				singledamage.second ? DamageNumber::CRITICAL : DamageNumber::NORMAL,
				singledamage.first,
				1.0f + alphashift,
				vector2d<int16_t>(startpos.x(), startpos.y() - yshift)));

			yshift += 24;
			alphashift += 0.1f;
		}

		return damagelines;
	}

	pair<int32_t, bool> Mob::randomdamage(int32_t mindamage, 
		int32_t maxdamage, float hitchance, float critical) const {

		if (randomizer.below(hitchance))
			return std::make_pair(0, false);

		int32_t damage = randomizer.nextint<int32_t>(mindamage, maxdamage);

		bool iscritical = randomizer.below(critical);
		if (iscritical)
			damage = static_cast<int32_t>(damage * 1.5f);

		if (damage < 1)
			damage = 1;
		else if (damage > DAMAGECAP)
			damage = DAMAGECAP;

		return std::make_pair(damage, iscritical);
	}
}
