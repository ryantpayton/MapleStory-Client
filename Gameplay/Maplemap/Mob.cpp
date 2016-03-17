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
#include "Util\Misc.h"

#include "nlnx\nx.hpp"

#include <algorithm>

namespace Gameplay
{
	using Net::Session;

	Mob::Mob(int32_t oi, int32_t mid, int8_t mode, int8_t st, uint16_t fh, 
		bool newspawn, int8_t tm, Point<int16_t> position) : MapObject(oi) {

		string strid = Format::extendid(mid, 7);
		node src = nl::nx::mob[strid + ".img"];

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
		flyspeed = info["flySpeed"];
		touchdamage = info["bodyAttack"].get_bool();
		undead = info["undead"].get_bool();
		noflip = info["noFlip"].get_bool();
		notattack = info["notAttack"].get_bool();
		canjump = src["jump"].size() > 0;
		canfly = src["fly"].size() > 0;
		canmove = src["move"].size() > 0 || canfly;

		if (canfly)
		{
			animations[STAND] = src["fly"];
			animations[MOVE] = src["fly"];
		}
		else
		{
			animations[STAND] = src["stand"];
			animations[MOVE] = src["move"];
		}
		animations[JUMP] = src["jump"];
		animations[HIT] = src["hit1"];
		animations[DIE] = src["die1"];

		name = nl::nx::string["Mob.img"][std::to_string(mid)]["name"];

		node sndsrc = nl::nx::sound["Mob.img"][strid];
		hitsound = sndsrc["Damage"];
		diesound = sndsrc["Die"];

		speed += 100;
		speed *= 0.001f;

		flyspeed += 100;
		flyspeed *= 0.0005f;

		if (canfly)
			phobj.type = PhysicsObject::FLYING;

		id = mid;
		team = tm;
		setposition(position);
		setcontrol(mode);
		phobj.fhid = fh;
		phobj.setflag(PhysicsObject::TURNATEDGES);

		hppercent = 0;
		flip = (st % 2) == 1;
		if (flip)
			st -= 1;
		stance = static_cast<Stance>(st);
		dying = false;
		dead = false;
		fading = false;
		flydirection = STRAIGHT;
		counter = 0;

		namelabel = Text(Text::A13M, Text::CENTER, Text::WHITE);
		namelabel.setback(Text::NAMETAG);
		namelabel.settext(name);

		if (newspawn)
		{
			fadein = true;
			opacity.set(0.0f);
		}
		else
		{
			fadein = false;
			opacity.set(1.0f);
		}

		if (control && stance == Stance::STAND)
		{
			nextmove();
		}
	}

	void Mob::setstance(Stance st)
	{
		if (stance != st)
		{
			stance = st;

			switch (stance)
			{
			case DIE:
				dying = true;
				break;
			}

			animations.at(stance).reset();
		}
	}

	int8_t Mob::update(const Physics& physics)
	{
		if (!active)
			return phobj.fhlayer;

		bool aniend = animations.at(stance).update();
		if (aniend && dying)
		{
			dead = true;
		}

		if (fading)
		{
			opacity -= 0.025f;
			if (opacity.last() < 0.025f)
			{
				opacity.set(0.0f);
				fading = false;
				dead = true;
			}
		}
		else if (fadein)
		{
			opacity += 0.025f;
			if (opacity.last() > 0.975f)
			{
				opacity.set(1.0f);
				fadein = false;
			}
		}

		Point<int16_t> head = getheadpos(getposition());
		bulletlist.remove_if([&](pair<Bullet, AttackEffect>& bulletattack) {
			bool apply = bulletattack.first.update(head);
			if (apply)
			{
				applyattack(bulletattack.second);
			}
			return apply;
		});
		bool nobullets = bulletlist.size() == 0;

		singlelist.remove_if([&](SingleEffect& single) {
			if (single.delay <= Constants::TIMESTEP)
			{
				applysingle(single);
				return true;
			}
			else
			{
				single.delay -= Constants::TIMESTEP;
				return false;
			}
		});
		bool nosingle = singlelist.size() == 0;

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
		bool nonumbers = damagenumbers.size() == 0;

		if (dead && nobullets && nosingle && nonumbers)
		{
			active = false;
			return -1;
		}

		effects.update();
		showhp.update();

		if (control && !dying)
		{
			if (!canfly)
			{
				if (phobj.flagnotset(PhysicsObject::TURNATEDGES))
				{
					flip = !flip;
					phobj.setflag(PhysicsObject::TURNATEDGES);

					if (stance == HIT)
						setstance(STAND);
				}
			}

			switch (stance)
			{
			case MOVE:
				if (canfly)
				{
					phobj.hforce = flip ? flyspeed : -flyspeed;
					switch (flydirection)
					{
					case UPWARDS:
						phobj.vforce = -flyspeed;
						break;
					case DOWNWARDS:
						phobj.vforce = flyspeed;
						break;
					}
				}
				else
				{
					phobj.hforce = flip ? speed : -speed;
					if (canjump && phobj.onground && counter > 50 && counter < 150 && randomizer.below(0.005f))
					{
						phobj.vforce = -5.0f;
					}
				}
				break;
			case HIT:
				if (canmove)
				{
					double KBFORCE = phobj.onground ? 0.2 : 0.1;
					phobj.hforce = flip ? -KBFORCE : KBFORCE;
				}
				break;
			}

			counter++;
			if (aniend && counter > 200)
			{
				nextmove();
				sendmovement();
				counter = 0;
			}

			physics.moveobject(phobj);
		}
		else
		{
			phobj.normalize();
			physics.getfht().updatefh(phobj);
		}

		return phobj.fhlayer;
	}

	void Mob::nextmove()
	{
		if (canmove)
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

			if (stance == MOVE && canfly)
			{
				flydirection = nextdirection(randomizer);
			}
		}
		else
		{
			setstance(STAND);
		}
	}

	void Mob::sendmovement()
	{
		using Net::MoveMobPacket;
		Session::get().dispatch(
			MoveMobPacket(
			oid, 
			1, 0, 0, 0, 0, 0, 0, 
			getposition(),
			Movement(phobj, valueof(stance, flip)
			)));
	}

	void Mob::draw(Point<int16_t> viewpos, float alpha) const
	{
		Point<int16_t> absp = phobj.getposition(alpha) + viewpos;

		if (!dead)
		{
			float interopc = opacity.get(alpha);

			using Graphics::DrawArgument;
			animations.at(stance).draw(DrawArgument(absp, flip && !noflip, interopc), alpha);

			if (showhp)
			{
				namelabel.draw(absp);

				if (stance != DIE)
				{
					hpbar.draw(getheadpos(absp), hppercent);
				}
			}
		}

		effects.draw(absp, alpha);
	}

	void Mob::draweffects(Point<int16_t> viewpos, float alpha) const
	{
		for (auto& bullet : bulletlist)
		{
			bullet.first.draw(viewpos, alpha);
		}

		for (auto& dmg : damagenumbers)
		{
			dmg.draw(viewpos, alpha);
		}
	}

	void Mob::setcontrol(int8_t mode)
	{
		control = mode > 0;
		aggro = mode == 2;
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
			dying = true;
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
		{
			percent = 100;
		}
		else if (percent < 0)
		{
			percent = 0;
		}
		hppercent = percent;
		showhp.setfor(2000);
	}

	bool Mob::isalive() const
	{
		return active && !dying;
	}

	bool Mob::isinrange(const rectangle2d<int16_t>& range) const
	{
		if (!active)
			return false;

		rectangle2d<int16_t> bounds = animations.at(stance).getbounds();
		bounds.shift(getposition());
		return range.overlaps(bounds);
	}

	float Mob::calchitchance(int16_t leveldelta, int32_t accuracy) const
	{
		float faccuracy = static_cast<float>(accuracy);
		float hitchance = faccuracy / (((1.84f + 0.07f * leveldelta) * avoid) + 1.0f);
		if (hitchance < 0.01f)
		{
			hitchance = 0.01f;
		}
		return hitchance;
	}

	double Mob::calcmindamage(int16_t leveldelta, double damage) const
	{
		double mindamage = damage * (1 - 0.01f * leveldelta) - wdef * 0.5f;
		if (mindamage < 1.0)
		{
			mindamage = 1.0;
		}
		return mindamage;
	}

	double Mob::calcmaxdamage(int16_t leveldelta, double damage) const
	{
		double maxdamage = damage * (1 - 0.01f * leveldelta) - wdef * 0.6f;
		if (maxdamage < 1.0)
		{
			maxdamage = 1.0;
		}
		return maxdamage;
	}

	vector<int32_t> Mob::damage(const Attack& attack)
	{
		int16_t leveldelta = level - attack.playerlevel;
		if (leveldelta < 0)
			leveldelta = 0;

		float hitchance = calchitchance(leveldelta, attack.accuracy);
		double mindamage = calcmindamage(leveldelta, attack.mindamage);
		double maxdamage = calcmaxdamage(leveldelta, attack.maxdamage);

		AttackEffect attackeffect = attack;

		vector<int32_t> result;
		for (int32_t i = 0; i < attack.hitcount; i++)
		{
			auto singledamage = randomdamage(mindamage, maxdamage, hitchance, attack.critical);
			attackeffect.push_back(singledamage);

			auto singlenumber = singledamage.first;
			result.push_back(singlenumber);
		}

		bool ranged = attack.type == Attack::RANGED;
		if (ranged)
		{
			Bullet bullet = Bullet(attack.bullet, attack.origin, attack.direction == Attack::TOLEFT);
			bulletlist.push_back({ bullet, attackeffect });
		}
		else
		{
			applyattack(attackeffect);
		}

		sendmovement();

		return result;
	}

	pair<int32_t, bool> Mob::randomdamage(double mindamage, double maxdamage, float hitchance, float critical) const 
	{
		bool hit = randomizer.below(hitchance);
		if (hit)
		{
			static const double DAMAGECAP = 999999.0;

			auto damage = randomizer.nextreal<double>(mindamage, maxdamage);
			bool iscritical = randomizer.below(critical);
			if (iscritical)
			{
				damage *= 1.5;
			}
			if (damage < 1)
			{
				damage = 1;
			}
			else if (damage > DAMAGECAP)
			{
				damage = DAMAGECAP;
			}

			auto intdamage = static_cast<int32_t>(damage);
			return std::make_pair(intdamage, iscritical);
		}
		else
		{
			return std::make_pair(0, false);
		}
	}

	void Mob::applyattack(const AttackEffect& attackeffect)
	{
		Point<int16_t> head = getheadpos(getposition());
		queue<SingleEffect> singleffects = attackeffect.seperate(head);

		applysingle(singleffects.front());
		singleffects.pop();
		for (; singleffects.size(); singleffects.pop())
		{
			singlelist.push_back(singleffects.front());
		}

		int32_t total = attackeffect.gettotal();
		bool toleft = attackeffect.toleft();
		if (total >= knockback)
		{
			applyknockback(toleft);
		}
	}

	void Mob::applysingle(const SingleEffect& singleeffect)
	{
		damagenumbers.push_back(singleeffect.number);
		effects.add(singleeffect.hiteffect);
		singleeffect.hitsound.play();
		hitsound.play();
	}

	void Mob::applyknockback(bool fromright)
	{
		if (isalive())
		{
			flip = fromright;
			counter = 170;
			setstance(HIT);
		}
	}


	Mob::AttackEffect::AttackEffect(const Attack& attack)
	{
		hiteffect = attack.hiteffect;
		hitsound = attack.hitsound;
		direction = attack.direction;
		hitdelays = attack.hitdelays;
		firstdelay = attack.delay;
	}

	void Mob::AttackEffect::push_back(pair<int32_t, bool> number)
	{
		numbers.push_back(number);

		total += number.first;
	}

	bool Mob::AttackEffect::toleft() const
	{
		return direction == Attack::TOLEFT;
	}

	int32_t Mob::AttackEffect::gettotal() const
	{
		return total;
	}

	queue<Mob::SingleEffect> Mob::AttackEffect::seperate(Point<int16_t> head) const
	{
		queue<SingleEffect> singleeffects;
		for (size_t i = 0; i < numbers.size(); i++)
		{
			int32_t amount = numbers[i].first;
			bool critical = numbers[i].second;

			DamageNumber::Type type = critical ? DamageNumber::CRITICAL : DamageNumber::NORMAL;
			auto damagenumber = DamageNumber(type, amount, head);

			uint16_t delay;
			if (i > 0 && i - 1 < hitdelays.size())
			{
				delay = hitdelays[i - 1];
			}
			else
			{
				delay = 0;
			}

			auto singleeffect = SingleEffect(damagenumber, hiteffect, hitsound, delay - firstdelay);
			singleeffects.push(singleeffect);

			int16_t vspace = critical ? 36 : 30;
			head.shifty(-vspace);
		}
		return singleeffects;
	}
}
