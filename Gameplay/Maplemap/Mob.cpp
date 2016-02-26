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
#include "Constants.h"
#include "Net\Session.h"
#include "Net\Packets\GameplayPackets.h"
#include "nlnx\nx.hpp"
#include <algorithm>

namespace Gameplay
{
	const int32_t DAMAGECAP = 999999;
	const float MONSTERSPEED = 0.3f;

	Mob::Mob(int32_t oi, int32_t mid, bool cnt, int8_t st, uint16_t fh, 
		bool newspawn, int8_t tm, vector2d<int16_t> position) {

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
		speed = info["speed"];
		speed += 100;
		touchdamage = info["bodyAttack"].get_bool();
		undead = info["undead"].get_bool();
		noflip = info["noFlip"].get_bool();
		notattack = info["notAttack"].get_bool();

		parsestance(STAND, src["stand"]);
		parsestance(MOVE, src["move"]);
		parsestance(HIT, src["hit1"]);
		parsestance(DIE, src["die1"]);

		name = nl::nx::string["Mob.img"][std::to_string(mid)]["name"];

		node sndsrc = nl::nx::sound["Mob.img"][path];
		hitsound = sndsrc["Damage"];
		diesound = sndsrc["Die"];

		oid = oi;
		id = mid;
		team = tm;
		control = cnt;
		phobj.fhid = fh;
		setposition(position);

		active = true;
		hppercent = 0;
		flip = (st % 2) == 1;
		if (flip)
			st -= 1;
		stance = static_cast<Stance>(st);
		fading = false;
		counter = 0;

		namelabel = Text(Text::A13M, Text::CENTER, Text::WHITE);
		namelabel.settext(name);
		namelabel.setback(Text::NAMETAG);

		if (newspawn)
		{
			fadein = true;
			alpha = 0.0f;
		}
		else
		{
			fadein = false;
			alpha = 1.0f;
		}
	}

	void Mob::parsestance(Stance stance, node src)
	{
		if (src.size() > 0)
		{
			animations[stance] = src;
			bounds[stance] = rectangle2d<int16_t>(src["0"]["lt"], src["0"]["rb"]);
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
		if (!active)
			return phobj.fhlayer;

		if (fading)
		{
			alpha -= 0.025f;
			if (alpha < 0.025f)
			{
				alpha = 0.0f;
				fading = false;
			}
		}
		else if (fadein)
		{
			alpha += 0.025f;
			if (alpha > 0.975f)
			{
				alpha = 1.0f;
				fadein = false;
			}
		}

		auto indices = bulletlist.collect(&Bullet::update, getposition());
		for (auto& index : indices)
		{
			Optional<Bullet> bullet = bulletlist.get(index);
			if (bullet)
			{
				applydamage(bullet->damageeffect);
				bulletlist.remove(index);
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

		effects.update();

		if (control && stance != DIE)
		{
			switch (stance)
			{
			case MOVE:
				phobj.hforce = flip ? static_cast<float>(speed) * MONSTERSPEED / 100
					: -static_cast<float>(speed) * MONSTERSPEED / 100;
				break;
			case HIT:
				phobj.hforce = flip ? -0.5f : 0.5f;
				break;
			}

			counter++;
			if (counter > 200)
			{
				nextmove();
				writemovement();
				counter = 0;
			}
		}

		physics.moveobject(phobj);

		return phobj.fhlayer;
	}

	void Mob::nextmove()
	{
		switch (stance)
		{
		case STAND:
			setstance(MOVE);
			flip = randomizer.nextbool();
			break;
		case MOVE:
		case HIT:
			switch (randomizer.nextint(2))
			{
			case 0:
				setstance(STAND);
				break;
			case 1:
				setstance(MOVE);
				flip = false;
				break;
			case 2:
				setstance(MOVE);
				flip = true;
				break;
			}
		}
	}

	void Mob::writemovement()
	{
		MovementFragment lastmove;
		lastmove.type = MovementFragment::MVT_ABSOLUTE;
		lastmove.command = 0;
		if (flip)
			lastmove.newstate = static_cast<uint8_t>(stance);
		else
			lastmove.newstate = static_cast<uint8_t>(stance + 1);
		lastmove.xpos = static_cast<int16_t>(phobj.fx);
		lastmove.ypos = static_cast<int16_t>(phobj.fy);
		lastmove.lastx = static_cast<int16_t>(phobj.lastx);
		lastmove.lasty = static_cast<int16_t>(phobj.lasty);
		lastmove.duration = Constants::TIMESTEP;
		movements.push_back(lastmove);

		if (movements.size() > 0)
		{
			using Net::Session;
			using Net::MoveMobPacket;
			Session::get().dispatch(MoveMobPacket(oid, 1, 0, 0, 0, 0, 0, 0, getposition(), movements));
			movements.clear();
		}
	}

	void Mob::draw(const Camera& camera, float inter) const
	{
		if (!active)
			return;

		vector2d<int16_t> viewpos = camera.getposition(inter);

		vector2d<int16_t> absp = phobj.getposition(inter) + viewpos;
		if (animations.count(stance))
		{
			using Graphics::DrawArgument;
			if (noflip)
			{
				animations.at(stance).draw(DrawArgument(absp, alpha), inter);
			}
			else
			{
				animations.at(stance).draw(DrawArgument(absp, flip, alpha), inter);
			}
		}

		effects.draw(absp, inter);

		if (hppercent > 0)
		{
			namelabel.draw(absp);
		}

		bulletlist.foreach(&Bullet::draw, viewpos, inter);

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
			diesound.play();
			setstance(DIE);
			break;
		case 2:
			fading = true;
			break;
		}
	}

	void Mob::sendhp(int8_t percent, uint16_t playerlevel)
	{
		if (hppercent == 0)
		{
			int16_t delta = playerlevel - level;
			if (delta > 9)
				namelabel.setcolor(Text::YELLOW);
			else if (delta < -9)
				namelabel.setcolor(Text::RED);
		}
		hppercent = percent;
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
		int16_t leveldelta = (level > attack.playerlevel) ? level - attack.playerlevel : 0;
		float hitchance = static_cast<float>(attack.accuracy) / (((1.84f + 0.07f * leveldelta) * avoid) - 1);
		int32_t mindamage = static_cast<int32_t>(attack.mindamage * (1 - 0.01f * leveldelta) - wdef * 0.5f);
		int32_t maxdamage = static_cast<int32_t>(attack.maxdamage * (1 - 0.01f * leveldelta) - wdef * 0.6f);

		vector<int32_t> damagelines;

		DamageEffect damageeffect;
		damageeffect.hiteffect = attack.hiteffect;
		damageeffect.hitsound = attack.hitsound;
		damageeffect.fromright = attack.origin.x() > phobj.fx;

		for (int32_t i = 0; i < attack.hitcount; i++)
		{
			auto singledamage = randomdamage(mindamage, maxdamage, hitchance, attack.critical);
			damagelines.push_back(singledamage.first);

			damageeffect.numbers.push_back(singledamage);
		}

		bool ranged = attack.type == Attack::RANGED;
		if (ranged)
		{
			Bullet bullet = { attack.bullet, damageeffect, attack.origin };
			bulletlist.add(bullet);
		}
		else
		{
			applydamage(damageeffect);
		}

		writemovement();

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

	void Mob::applydamage(const DamageEffect& damageeffect)
	{
		vector2d<int16_t> head = getposition();
		head.shifty(-animations.at(stance).getdimensions().y());

		float alphashift = 0.0f;

		int32_t total = 0;
		for (auto& number : damageeffect.numbers)
		{
			int32_t amount = number.first;
			total += amount;

			DamageNumber damagenumber = DamageNumber(
				number.second ? DamageNumber::CRITICAL : DamageNumber::NORMAL,
				number.first,
				1.0f + alphashift,
				head);
			damagenumbers.push_back(damagenumber);

			head.shifty(-24);
			alphashift += 0.1f;

			damageeffect.hitsound.play();
			hitsound.play();
		}

		if (total >= knockback)
			applyknockback(damageeffect.fromright);

		effects.add(damageeffect.hiteffect);
	}

	void Mob::applyknockback(bool fromright)
	{
		if (active && stance != DIE)
		{
			flip = fromright;
			counter = 175;
			setstance(HIT);
		}
	}
}
