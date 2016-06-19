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
#include "Stage.h"

#include "Combat\RegularAttack.h"

#include "..\Audio\Audio.h"
#include "..\IO\Messages.h"
#include "..\Net\InPacket.h"
#include "..\Net\Packets\GameplayPackets.h"
#include "..\Net\Packets\AttackAndSkillPackets.h"
#include "..\Util\Misc.h"

#include <nlnx\nx.hpp>
#include <nlnx\audio.hpp>

namespace jrc
{
	Stage::Stage()
	{
		state = INACTIVE;
		mapid = 0;
		portalid = 0;

		specialmoves[0] = std::make_unique<RegularAttack>();
	}

	void Stage::loadplayer(const CharEntry& entry)
	{
		player = entry;
		playable = player;
	}

	void Stage::setmap(uint8_t pid, int32_t mid)
	{
		portalid = pid;
		mapid = mid;
	}

	void Stage::clear()
	{
		state = INACTIVE;

		chars.clear();
		npcs.clear();
		mobs.clear();
		drops.clear();
	}

	void Stage::loadmap()
	{
		std::string strid = string_format::extend_id(mapid, 9);
		std::string prefix = std::to_string(mapid / 100000000);
		nl::node src = nl::nx::map["Map"]["Map" + prefix][strid + ".img"];

		layers = MapLayers(src);
		backgrounds = MapBackgrounds(src["back"]);
		physics = Physics(src["foothold"]);
		mapinfo = MapInfo(src, physics.get_fht().get_walls(), physics.get_fht().get_borders());
		portals = MapPortals(src["portal"], mapid);
	}

	void Stage::respawn()
	{
		Music(mapinfo.get_bgm()).play();

		Point<int16_t> spawnpoint = portals.get_portal_by_id(portalid);
		Point<int16_t> startpos = physics.get_y_below(spawnpoint);
		player.respawn(startpos, mapinfo.is_underwater());
		camera.set_position(startpos);
		camera.set_view(mapinfo.get_walls(), mapinfo.get_borders());
	}

	void Stage::reload()
	{
		switch (state)
		{
		case INACTIVE:
			loadmap();
			respawn();
			break;
		case TRANSITION:
			respawn();
			break;
		}

		state = ACTIVE;
	}

	void Stage::draw(float alpha) const
	{
		if (state == ACTIVE)
		{
			Point<int16_t> viewpos = camera.position(alpha);
			Point<double> viewrpos = camera.realposition(alpha);
			double viewx = viewrpos.x();
			double viewy = viewrpos.y();

			backgrounds.drawbackgrounds(viewx, viewy, alpha);
			for (uint8_t i = 0; i < MapLayers::NUM_LAYERS; i++)
			{
				layers.draw(i, viewpos, alpha);
				npcs.draw(i, viewx, viewy, alpha);
				mobs.draw(i, viewx, viewy, alpha);
				chars.draw(i, viewx, viewy, alpha);
				player.draw(i, viewx, viewy, alpha);
				drops.draw(i, viewx, viewy, alpha);
			}
			portals.draw(viewpos, alpha);
			backgrounds.drawforegrounds(viewx, viewy, alpha);
		}
	}

	void Stage::pollspawns()
	{
		for (; !spawnqueue.empty(); spawnqueue.pop())
		{
			Optional<const Spawn> spawn = spawnqueue.front().get();
			if (spawn)
			{
				switch (spawn->type())
				{
				case Spawn::NPC:
					npcs.send_spawn(*spawn.reinterpret<const NpcSpawn>(), physics);
					break;
				case Spawn::MOB:
					mobs.send_spawn(*spawn.reinterpret<const MobSpawn>());
					break;
				case Spawn::DROP:
					drops.send_spawn(*spawn.reinterpret<const DropSpawn>());
					break;
				case Spawn::CHARACTER:
					chars.send_spawn(*spawn.reinterpret<const CharSpawn>());
					break;
				}
			}
		}
	}

	void Stage::pollattacks()
	{
		attackqueue.remove_if([&](DelayedAttack& da) {
			bool apply = da.update();
			if (apply)
			{
				int32_t cid = da.char_id;
				Optional<OtherChar> ouser = chars.get_char(cid);
				if (ouser)
				{
					const AttackResult& attack = da.attack;

					OtherChar& user = *ouser;
					user.updateskill(attack.skill, attack.level);
					user.updateattack(attack.speed);

					const SpecialMove& move = get_move(attack.skill);
					move.apply_useeffects(user, attack.type);

					Stance::Value stance = Stance::byid(attack.stance);
					user.attack(stance);

					mobs.show_result(user, move, attack);
				}
				return true;
			}
			else
			{
				return false;
			}
		});
	}

	void Stage::update()
	{
		if (state == ACTIVE)
		{
			pollspawns();
			pollattacks();

			backgrounds.update();
			layers.update();
			npcs.update(physics);
			mobs.update(physics);
			chars.update(physics);
			drops.update(physics);
			player.update(physics);
			portals.update(player.get_position());
			camera.update(player.get_position());
		}
	}

	void Stage::push_spawn(std::unique_ptr<const Spawn> spawn)
	{
		spawnqueue.push(std::move(spawn));
	}

	const SpecialMove& Stage::get_move(int32_t moveid)
	{
		bool newmove = !specialmoves.count(moveid);
		if (newmove)
		{
			specialmoves[moveid] = std::make_unique<Skill>(moveid);
		}
		return *specialmoves[moveid];
	}

	void Stage::use_move(int32_t moveid)
	{
		if (!player.can_attack())
			return;

		const SpecialMove& move = get_move(moveid);

		SpecialMove::ForbidReason reason = player.can_use(move);
		Weapon::Type weapontype = player.get_stats().get_weapontype();
		switch (reason)
		{
		case SpecialMove::FBR_NONE:
			apply_move(move);
			break;
		default:
			ForbidSkillMessage(reason, weapontype).drop();
			break;
		}
	}

	void Stage::apply_move(const SpecialMove& move)
	{
		bool offensive = move.isoffensive();
		if (offensive)
		{
			Attack attack = player.prepare_attack(move.is_skill());

			move.apply_useeffects(player, attack.type);
			move.applystats(player, attack);

			AttackResult result = mobs.send_attack(attack);
			mobs.show_result(player, move, result);

			apply_use_movement(move);
			apply_result_movement(move, result);

			AttackPacket(result).dispatch();
		}
		else
		{
			move.apply_useeffects(player, Attack::MAGIC);

			int32_t moveid = move.get_id();
			int32_t level = player.get_skills().get_level(moveid);
			UseSkillPacket(moveid, level).dispatch();
		}
	}

	void Stage::apply_use_movement(const SpecialMove& move)
	{
		switch (move.get_id())
		{
		case Skill::FP_TELEPORT:
		case Skill::IL_TELEPORT:
		case Skill::PRIEST_TELEPORT:
			break;
		case Skill::FLASH_JUMP:
			break;
		}
	}

	void Stage::apply_result_movement(const SpecialMove& move, const AttackResult& result)
	{
		switch (move.get_id())
		{
		case Skill::RUSH_HERO:
		case Skill::RUSH_PALADIN:
		case Skill::RUSH_DK:
			apply_rush(result);
			break;
		}
	}

	void Stage::apply_rush(const AttackResult& result)
	{
		if (result.mobcount == 0)
			return;

		int32_t oid = result.get_last_oid();
		auto mob = mobs.get_mob(oid);
		if (mob)
		{
			int16_t targetx = mob->get_position().x();
			player.rush(targetx);
		}
	}

	void Stage::show_attack(int32_t cid, const AttackResult& attack)
	{
		attackqueue.emplace_back(attack, cid);
	}

	void Stage::show_buff(int32_t cid, int32_t skillid, int8_t level)
	{
		Optional<OtherChar> ouser = chars.get_char(cid);
		if (ouser)
		{
			OtherChar& user = *ouser;
			user.updateskill(skillid, level);

			const SpecialMove& move = get_move(skillid);
			move.apply_useeffects(user, Attack::MAGIC);
		}
	}

	void Stage::show_player_buff(int32_t skillid)
	{
		const SpecialMove& move = get_move(skillid);
		if (player.can_use(move))
		{
			move.apply_useeffects(player, Attack::MAGIC);
		}
	}

	void Stage::check_portals()
	{
		if (player.is_attacking())
			return;

		Point<int16_t> playerpos = player.get_position();
		Portal::WarpInfo warpinfo = portals.find_at(playerpos);
		if (warpinfo.mapid == mapid)
		{
			Point<int16_t> spawnpoint = portals.get_portal_by_name(warpinfo.portal);
			Point<int16_t> startpos = physics.get_y_below(spawnpoint);
			player.respawn(startpos, mapinfo.is_underwater());
		}
		else if (warpinfo.valid)
		{
			ChangeMapPacket(false, warpinfo.mapid, warpinfo.portal, false).dispatch();
		}
	}

	void Stage::check_seats()
	{
		if (player.is_sitting() || player.is_attacking())
			return;

		Optional<const Seat> seat = mapinfo.findseat(player.get_position());
		player.set_seat(seat);
	}

	void Stage::check_ladders(bool up)
	{
		if (player.is_climbing() || player.is_attacking())
			return;

		Optional<const Ladder> ladder = mapinfo.findladder(player.get_position(), up);
		player.set_ladder(ladder);
	}

	void Stage::check_drops()
	{
		Point<int16_t> playerpos = player.get_position();
		const Drop* drop = drops.findinrange(playerpos);
		if (drop)
		{
			PickupItemPacket(drop->get_oid(), drop->get_position()).dispatch();
		}
	}

	void Stage::send_key(Keyboard::Keytype type, int32_t action, bool down)
	{
		if (state != ACTIVE || !playable)
			return;

		switch (type)
		{
		case Keyboard::ACTION:
			if (down)
			{
				switch (action)
				{
				case Keyboard::UP:
					check_ladders(true);
					check_portals();
					break;
				case Keyboard::DOWN:
					check_ladders(false);
					break;
				case Keyboard::SIT:
					check_seats();
					break;
				case Keyboard::ATTACK:
					use_move(0);
					break;
				case Keyboard::PICKUP:
					check_drops();
					break;
				}
			}

			playable->send_action(Keyboard::actionbyid(action), down);
			break;
		case Keyboard::SKILL:
			use_move(action);
			break;
		case Keyboard::ITEM:
			player.use_item(action);
			break;
		case Keyboard::FACE:
			player.set_expression(action);
			break;
		}
	}

	Cursor::State Stage::send_cursor(bool pressed, Point<int16_t> position)
	{
		return npcs.send_cursor(pressed, position, camera.position());
	}

	bool Stage::is_player(int32_t cid) const
	{
		return cid == player.get_oid();
	}

	MapNpcs& Stage::get_npcs()
	{
		return npcs;
	}

	MapChars& Stage::get_chars()
	{
		return chars;
	}

	MapMobs& Stage::get_mobs()
	{
		return mobs;
	}

	MapDrops& Stage::get_drops()
	{
		return drops;
	}

	Player& Stage::get_player()
	{
		return player;
	}

	Optional<Char> Stage::get_character(int32_t cid)
	{
		if (is_player(cid))
		{
			return player;
		}
		else
		{
			return chars.get_char(cid).cast<Char>();
		}
	}
}
