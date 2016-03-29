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
#include "Net\Packets\GameplayPackets.h"
#include "Util\Misc.h"

#include "nlnx\nx.hpp"

#include <algorithm>

namespace Gameplay
{
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
		dying = false;
		dead = false;
		fading = false;
		setstance(st);
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

	void Mob::setstance(uint8_t stancebyte)
	{
		flip = (stancebyte % 2) == 0;
		if (!flip)
		{
			stancebyte -= 1;
		}
		if (stancebyte < MOVE)
			stancebyte = MOVE;
		setstance(static_cast<Stance>(stancebyte));
	}

	void Mob::setstance(Stance newstance)
	{
		if (stance != newstance)
		{
			stance = newstance;

			animations.at(stance).reset();
		}
	}

	int8_t Mob::update(const Physics& physics)
	{
		if (!active)
			return phobj.fhlayer;

		bool aniend = animations.at(stance).update();
		if (aniend && stance == DIE)
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

		if (dead)
		{
			active = false;
			return -1;
		}

		effects.update();
		showhp.update();

		if (!dying)
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
				}
				break;
			case HIT:
				if (canmove)
				{
					double KBFORCE = phobj.onground ? 0.2 : 0.1;
					phobj.hforce = flip ? -KBFORCE : KBFORCE;
				}
				break;
			case JUMP:
				phobj.vforce = -5.0;
				break;
			}

			physics.moveobject(phobj);

			if (control)
			{
				counter++;

				bool next;
				switch (stance)
				{
				case HIT:
					next = counter > 200;
					break;
				case JUMP:
					next = phobj.onground;
					break;
				default:
					next = aniend && counter > 200;
					break;
				}

				if (next)
				{
					nextmove();
					updatemovement();
					counter = 0;
				}
			}
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
			case HIT:
			case STAND:
				setstance(MOVE);
				flip = randomizer.nextbool();
				break;
			case MOVE:
			case JUMP:
				if (canjump && phobj.onground && randomizer.below(0.25f))
				{
					setstance(JUMP);
				}
				else
				{
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
				break;
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

	void Mob::updatemovement()
	{
		using Net::MoveMobPacket;
		MoveMobPacket(
			oid, 
			1, 0, 0, 0, 0, 0, 0, 
			getposition(),
			Movement(phobj, valueof(stance, flip))
			).dispatch();
	}

	void Mob::draw(Point<int16_t> viewpos, float alpha) const
	{
		Point<int16_t> absp = phobj.getposition(alpha) + viewpos;
		Point<int16_t> headpos = getheadpos(absp);

		effects.drawbelow(headpos, alpha);

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
					hpbar.draw(headpos, hppercent);
				}
			}
		}

		effects.drawabove(headpos, alpha);
	}

	void Mob::setcontrol(int8_t mode)
	{
		control = mode > 0;
		aggro = mode == 2;
	}

	void Mob::sendmovement(Point<int16_t> start, const Movement& movement)
	{
		if (control)
			return;

		setposition(start);
		lastmove = movement;

		uint8_t laststance = lastmove.newstate;
		setstance(laststance);

		phobj.fhid = lastmove.fh;
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
			dying = true;
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

	void Mob::showeffect(Animation animation, int8_t)
	{
		if (active)
		{
			effects.add(animation);
		}
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

	vector<pair<int32_t, bool>> Mob::calculatedamage(const Attack& attack)
	{
		int16_t leveldelta = level - attack.playerlevel;
		if (leveldelta < 0)
			leveldelta = 0;

		float hitchance = calchitchance(leveldelta, attack.accuracy);
		double mindamage = calcmindamage(leveldelta, attack.mindamage);
		double maxdamage = calcmaxdamage(leveldelta, attack.maxdamage);

		vector<pair<int32_t, bool>> result;
		for (int32_t i = 0; i < attack.hitcount; i++)
		{
			auto singledamage = randomdamage(mindamage, maxdamage, hitchance, attack.critical);
			result.push_back(singledamage);
		}

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

	vector<DamageNumber> Mob::placenumbers(vector<pair<int32_t, bool>> damagelines) const
	{
		vector<DamageNumber> numbers;
		Point<int16_t> head = getheadpos(getposition());
		for (size_t i = 0; i < damagelines.size(); i++)
		{
			int32_t amount = damagelines[i].first;
			bool critical = damagelines[i].second;
			DamageNumber::Type type = critical ? DamageNumber::CRITICAL : DamageNumber::NORMAL;
			numbers.push_back(DamageNumber(type, amount, head));

			int16_t vspace = critical ? 36 : 30;
			head.shifty(-vspace);
		}
		return numbers;
	}

	void Mob::applydamage(int32_t damage, bool toleft)
	{
		hitsound.play();

		if (dying && stance != DIE)
		{
			setstance(DIE);
			diesound.play();
		}
		else if (control && isalive() && damage >= knockback)
		{
			flip = toleft;
			counter = 170;
			setstance(HIT);

			updatemovement();
		}
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

	Point<int16_t> Mob::getheadpos() const
	{
		Point<int16_t> position = getposition();
		return getheadpos(position);
	}
}
