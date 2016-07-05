//////////////////////////////////////////////////////////////////////////////
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
#include "Mob.h"

#include "../Movement.h"

#include "../../Constants.h"
#include "../../Net/Packets/GameplayPackets.h"
#include "../../Util/Misc.h"

#include "nlnx/nx.hpp"

#include <algorithm>
#include <functional>

namespace jrc
{
	Mob::Mob(int32_t oi, int32_t mid, int8_t mode, int8_t st, uint16_t fh, 
		bool newspawn, int8_t tm, Point<int16_t> position) : MapObject(oi) {

		std::string strid = string_format::extend_id(mid, 7);
		nl::node src = nl::nx::mob[strid + ".img"];

		nl::node info = src["info"];

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

		nl::node sndsrc = nl::nx::sound["Mob.img"][strid];

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
		set_position(position);
		set_control(mode);
		phobj.fhid = fh;
		phobj.set_flag(PhysicsObject::TURNATEDGES);

		hppercent = 0;
		dying = false;
		dead = false;
		fading = false;
		awaitdeath = false;
		set_stance(st);
		flydirection = STRAIGHT;
		counter = 0;

		namelabel = { Text::A13M, Text::CENTER, Text::WHITE, Text::NAMETAG, name };

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
			next_move();
		}
	}

	void Mob::set_stance(uint8_t stancebyte)
	{
		flip = (stancebyte % 2) == 0;
		if (!flip)
		{
			stancebyte -= 1;
		}
		if (stancebyte < MOVE)
			stancebyte = MOVE;
		set_stance(static_cast<Stance>(stancebyte));
	}

	void Mob::set_stance(Stance newstance)
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
				if (phobj.is_flag_not_set(PhysicsObject::TURNATEDGES))
				{
					flip = !flip;
					phobj.set_flag(PhysicsObject::TURNATEDGES);

					if (stance == HIT)
						set_stance(STAND);
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

			physics.move_object(phobj);

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
					next_move();
					update_movement();
					counter = 0;
				}
			}
		}
		else
		{
			phobj.normalize();
			physics.get_fht().update_fh(phobj);
		}

		return phobj.fhlayer;
	}

	void Mob::next_move()
	{
		if (canmove)
		{
			switch (stance)
			{
			case HIT:
			case STAND:
				set_stance(MOVE);
				flip = randomizer.next_bool();
				break;
			case MOVE:
			case JUMP:
				if (canjump && phobj.onground && randomizer.below(0.25f))
				{
					set_stance(JUMP);
				}
				else
				{
					switch (randomizer.next_int(3))
					{
					case 0:
						set_stance(STAND);
						break;
					case 1:
						set_stance(MOVE);
						flip = false;
						break;
					case 2:
						set_stance(MOVE);
						flip = true;
						break;
					}
				}
				break;
			}

			if (stance == MOVE && canfly)
			{
				flydirection = randomizer.next_enum(NUM_DIRECTIONS);
			}
		}
		else
		{
			set_stance(STAND);
		}
	}

	void Mob::update_movement()
	{
		MoveMobPacket(
			oid, 
			1, 0, 0, 0, 0, 0, 0, 
			get_position(),
			Movement(phobj, value_of(stance, flip))
			).dispatch();
	}

	void Mob::draw(double viewx, double viewy, float alpha) const
	{
		Point<int16_t> absp = phobj.get_absolute(viewx, viewy, alpha);
		Point<int16_t> headpos = get_head_position(absp);

		effects.drawbelow(absp, alpha);

		if (!dead)
		{
			float interopc = opacity.get(alpha);

			animations.at(stance).draw(DrawArgument(absp, flip && !noflip, interopc), alpha);

			if (showhp)
			{
				namelabel.draw(absp);

				if (!dying && hppercent > 0)
				{
					hpbar.draw(headpos, hppercent);
				}
			}
		}

		effects.drawabove(absp, alpha);
	}

	void Mob::set_control(int8_t mode)
	{
		control = mode > 0;
		aggro = mode == 2;
	}

	void Mob::send_movement(Point<int16_t> start, std::vector<Movement>&& in_movements)
	{
		if (control)
			return;

		set_position(start);

		movements = std::forward<decltype(in_movements)>(in_movements);

		if (movements.empty())
			return;

		const Movement& lastmove = movements.front();

		uint8_t laststance = lastmove.newstate;
		set_stance(laststance);

		phobj.fhid = lastmove.fh;
	}

	Point<int16_t> Mob::get_head_position(Point<int16_t> position) const
	{
		Point<int16_t> head = animations.at(stance).get_head();
		position.shift_x((flip && !noflip) ? -head.x() : head.x());
		position.shift_y(head.y());
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
			if (awaitdeath)
			{
				apply_death();
			}
			break;
		case 2:
			fading = true;
			dying = true;
			break;
		}
	}

	void Mob::show_hp(int8_t percent, uint16_t playerlevel)
	{
		if (hppercent == 0)
		{
			int16_t delta = playerlevel - level;
			if (delta > 9)
				namelabel.change_color(Text::YELLOW);
			else if (delta < -9)
				namelabel.change_color(Text::RED);
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
		showhp.set_for(2000);
	}

	void Mob::show_effect(const Animation& animation, int8_t pos, int8_t z, bool f)
	{
		if (!active)
			return;

		Point<int16_t> shift;
		switch (pos)
		{
		case 0:
			shift = get_head_position({});
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		}
		effects.add(animation, { shift, f }, z);
	}

	float Mob::calculate_hitchance(int16_t leveldelta, int32_t player_accuracy) const
	{
		float faccuracy = static_cast<float>(player_accuracy);
		float hitchance = faccuracy / (((1.84f + 0.07f * leveldelta) * avoid) + 1.0f);
		if (hitchance < 0.01f)
		{
			hitchance = 0.01f;
		}
		return hitchance;
	}

	double Mob::calculate_mindamage(int16_t leveldelta, double damage, bool magic) const
	{
		double mindamage = magic ?
			damage - (1 + 0.01 * leveldelta) * mdef * 0.6 :
			damage * (1 - 0.01 * leveldelta) - wdef * 0.6;
		return mindamage < 1.0 ? 1.0 : mindamage;
	}

	double Mob::calculate_maxdamage(int16_t leveldelta, double damage, bool magic) const
	{
		double maxdamage = magic ?
			damage - (1 + 0.01 * leveldelta) * mdef * 0.5 :
			damage * (1 - 0.01 * leveldelta) - wdef * 0.5;
		return maxdamage < 1.0 ? 1.0 : maxdamage;
	}

	std::vector<std::pair<int32_t, bool>> Mob::calculate_damage(const Attack& attack)
	{
		double mindamage;
		double maxdamage;
		float hitchance;
		float critical;
		int16_t leveldelta = level - attack.playerlevel;
		if (leveldelta < 0)
			leveldelta = 0;

		Attack::DamageType damagetype = attack.damagetype;
		switch (damagetype)
		{
		case Attack::DMG_WEAPON:
		case Attack::DMG_MAGIC:
			mindamage = calculate_mindamage(leveldelta, attack.mindamage, damagetype == Attack::DMG_MAGIC);
			maxdamage = calculate_maxdamage(leveldelta, attack.maxdamage, damagetype == Attack::DMG_MAGIC);
			hitchance = calculate_hitchance(leveldelta, attack.accuracy);
			critical = attack.critical;
			break;
		case Attack::DMG_FIXED:
			mindamage = attack.fixdamage;
			maxdamage = attack.fixdamage;
			hitchance = 1.0f;
			critical = 0.0f;
			break;
		}

		std::vector<std::pair<int32_t, bool>> result(attack.hitcount);
		std::generate(result.begin(), result.end(), [&](){
			return next_damage(mindamage, maxdamage, hitchance, critical);
		});

		update_movement();
		awaitdeath = false;

		return result;
	}

	std::pair<int32_t, bool> Mob::next_damage(double mindamage, double maxdamage, float hitchance, float critical) const
	{
		bool hit = randomizer.below(hitchance);
		if (!hit)
			return{ 0, false };
		
		constexpr double DAMAGECAP = 999999.0;

		double damage = randomizer.next_real(mindamage, maxdamage);
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
		return{ intdamage, iscritical };
	}

	void Mob::apply_damage(int32_t damage, bool toleft)
	{
		hitsound.play();

		if (dying && stance != DIE)
		{
			apply_death();
		}
		else if (control && is_alive() && damage >= knockback)
		{
			flip = toleft;
			counter = 170;
			set_stance(HIT);

			update_movement();
			awaitdeath = true;
		}
	}

	MobAttack Mob::create_touch_attack() const
	{
		if (!touchdamage)
			return{};

		int32_t minattack = static_cast<int32_t>(watk * 0.8f);
		int32_t maxattack = watk;
		int32_t attack = randomizer.next_int(minattack, maxattack);
		return{ attack, get_position(), id, oid };
	}

	void Mob::apply_death()
	{
		set_stance(DIE);
		diesound.play();
		dying = true;
	}

	bool Mob::is_alive() const
	{
		return active && !dying;
	}

	bool Mob::is_in_range(const Rectangle<int16_t>& range) const
	{
		if (!active)
			return false;

		Rectangle<int16_t> bounds = animations.at(stance).get_bounds();
		bounds.shift(get_position());
		return range.overlaps(bounds);
	}

	Point<int16_t> Mob::get_head_position() const
	{
		Point<int16_t> position = get_position();
		return get_head_position(position);
	}
}
