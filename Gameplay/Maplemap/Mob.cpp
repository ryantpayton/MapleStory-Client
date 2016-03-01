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
#include "Gameplay\Movement.h"
#include "Net\Session.h"
#include "Net\Packets\GameplayPackets.h"
#include "nlnx\nx.hpp"
#include <algorithm>

namespace Gameplay
{
	using Net::Session;

	Mob::Mob(int32_t oi, int32_t mid, bool cnt, int8_t st, uint16_t fh, 
		bool newspawn, int8_t tm, Point<int16_t> position) {

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
		touchdamage = info["bodyAttack"].get_bool();
		undead = info["undead"].get_bool();
		noflip = info["noFlip"].get_bool();
		notattack = info["notAttack"].get_bool();
		canjump = src["jump"].size() > 0;

		animations[STAND] = src["stand"];
		animations[MOVE] = src["move"];
		animations[JUMP] = src["jump"];
		animations[HIT] = src["hit1"];
		animations[DIE] = src["die1"];

		name = nl::nx::string["Mob.img"][std::to_string(mid)]["name"];

		node sndsrc = nl::nx::sound["Mob.img"][path];
		hitsound = sndsrc["Damage"];
		diesound = sndsrc["Die"];

		speed += 100;
		speed *= 0.003f;

		oid = oi;
		id = mid;
		team = tm;
		control = cnt;
		phobj.fhid = fh;
		phobj.turnatedges = true;
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

	void Mob::setstance(Stance st)
	{
		if (stance != st)
		{
			stance = st;
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

		Point<int16_t> bodypos = getposition();
		bodypos.shifty(-animations.at(stance).getdimensions().y() / 2);
		bulletlist.remove_if([&](pair<Bullet, DamageEffect>& bullet) {
			bool apply = bullet.first.update(bodypos);
			if (apply)
			{
				applydamage(bullet.second);
				return true;
			}
			else
			{
				return false;
			}
		});

		bool aniend = animations.at(stance).update();
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
				phobj.hforce = flip ? speed : -speed;
				if (canjump && phobj.onground && counter > 50 && counter < 150 && randomizer.below(0.005f))
				{
					phobj.vforce = -5.0f;
				}
				break;
			case HIT:
				phobj.hforce = flip ? -0.5f : 0.5f;
				break;
			}

			counter++;
			if (counter > 200)
			{
				nextmove();
				sendmovement();
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

	void Mob::sendmovement()
	{
		using Net::MoveMobPacket;
		Session::get().dispatch(MoveMobPacket(
			oid, 
			1, 0, 0, 0, 0, 0, 0, 
			getposition(),
			Movement(phobj, valueof(stance, flip)
			)));
	}

	void Mob::draw(const Camera& camera, float inter) const
	{
		if (!active)
			return;

		Point<int16_t> viewpos = camera.getposition(inter);
		Point<int16_t> absp = phobj.getposition(inter) + viewpos;

		using Graphics::DrawArgument;
		animations.at(stance).draw(DrawArgument(absp, flip && !noflip, alpha), inter);

		effects.draw(absp, inter);

		if (hppercent > 0)
		{
			namelabel.draw(absp);

			if (stance != DIE)
			{
				hpbar.draw(getheadpos(absp), hppercent);
			}
		}

		for (auto& bullet : bulletlist)
		{
			bullet.first.draw(viewpos, inter);
		}

		for (auto& dmg : damagenumbers)
		{
			dmg.draw(viewpos);
		}
	}
	Point<int16_t> Mob::getheadpos(Point<int16_t> position) const
	{
		Point<int16_t> head = animations.at(stance).gethead();
		position.shiftx((flip && !noflip) ? -head.x() : head.x());
		position.shifty(head.y());
		return position;
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
		if (percent > 100)
			percent = 100;
		else if (percent < 0)
			percent = 0;
		hppercent = percent;
	}

	bool Mob::isactive() const
	{
		return active && stance != DIE;
	}

	bool Mob::isinrange(const rectangle2d<int16_t>& range) const
	{
		rectangle2d<int16_t> bounds = animations.at(stance).getbounds();
		bounds.shift(getposition());
		return range.overlaps(bounds);
	}

	vector<int32_t> Mob::damage(const Attack& attack)
	{
		int16_t leveldelta = level - attack.playerlevel;
		if (leveldelta < 0)
			leveldelta = 0;
		float hitchance = static_cast<float>(attack.accuracy) / (((1.84f + 0.07f * leveldelta) * avoid) + 1.0f);
		if (hitchance < 0.01f)
			hitchance = 0.01f;
		double mindamage = attack.mindamage * (1 - 0.01f * leveldelta) - wdef * 0.5f;
		if (mindamage < 1.0)
			mindamage = 1.0;
		double maxdamage = attack.maxdamage * (1 - 0.01f * leveldelta) - wdef * 0.6f;
		if (maxdamage < 1.0)
			maxdamage = 1.0;
		bool fromright = attack.origin.x() > phobj.fx;

		DamageEffect damageeffect;
		damageeffect.hiteffect = attack.hiteffect;
		damageeffect.hitsound = attack.hitsound;
		damageeffect.fromright = fromright;

		vector<int32_t> damagelines;
		for (int32_t i = 0; i < attack.hitcount; i++)
		{
			auto singledamage = randomdamage(mindamage, maxdamage, hitchance, attack.critical);
			damageeffect.numbers.push_back(singledamage);
			damagelines.push_back(singledamage.first);
		}

		bool ranged = attack.type == Attack::RANGED;
		if (ranged)
		{
			Bullet bullet = Bullet(attack.bullet, attack.origin, fromright);
			bulletlist.push_back({ bullet, damageeffect });
		}
		else
		{
			applydamage(damageeffect);
		}

		sendmovement();

		return damagelines;
	}

	pair<int32_t, bool> Mob::randomdamage(double mindamage, 
		double maxdamage, float hitchance, float critical) const {

		bool hit = randomizer.below(hitchance);
		if (hit)
		{
			static const double DAMAGECAP = 999999.0;

			double damage = randomizer.nextreal<double>(mindamage, maxdamage);
			bool iscritical = randomizer.below(critical);
			if (iscritical)
				damage *= 1.5;
			if (damage < 1)
				damage = 1;
			else if (damage > DAMAGECAP)
				damage = DAMAGECAP;

			int32_t intdamage = static_cast<int32_t>(damage);
			return std::make_pair(intdamage, iscritical);
		}
		else
		{
			return std::make_pair(0, false);
		}
	}

	void Mob::applydamage(const DamageEffect& damageeffect)
	{
		Point<int16_t> head = getheadpos(getposition());
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
			counter = 160;
			setstance(HIT);
		}
	}
}
