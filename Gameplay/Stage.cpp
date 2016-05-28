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
#include "Stage.h"

#include "Combat\RegularAttack.h"

#include "..\Audio\Audio.h"
#include "..\IO\Messages.h"
#include "..\Net\InPacket.h"
#include "..\Net\Packets\GameplayPackets.h"
#include "..\Net\Packets\AttackAndSkillPackets.h"
#include "..\Util\Misc.h"

#include "nlnx\nx.hpp"
#include "nlnx\audio.hpp"

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
		mapinfo = MapInfo(src, physics.getfht().getwalls(), physics.getfht().getborders());
		portals = MapPortals(src["portal"], mapid);
	}

	void Stage::respawn()
	{
		Music(mapinfo.getbgm()).play();

		Point<int16_t> spawnpoint = portals.get_portal_by_id(portalid);
		Point<int16_t> startpos = physics.getgroundbelow(spawnpoint);
		player.respawn(startpos, mapinfo.isswimmap());
		camera.setposition(startpos);
		camera.updateview(mapinfo.getwalls(), mapinfo.getborders());
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
		for (; spawnqueue.size() > 0; spawnqueue.pop())
		{
			Optional<const Spawn> spawn = spawnqueue.front().get();
			if (spawn)
			{
				switch (spawn->type())
				{
				case Spawn::NPC:
					npcs.sendspawn(*spawn.reinterpret<const NpcSpawn>(), physics);
					break;
				case Spawn::MOB:
					mobs.sendspawn(*spawn.reinterpret<const MobSpawn>());
					break;
				case Spawn::DROP:
					drops.sendspawn(*spawn.reinterpret<const DropSpawn>());
					break;
				case Spawn::CHARACTER:
					chars.sendspawn(*spawn.reinterpret<const CharSpawn>());
					break;
				}
			}
		}
	}

	void Stage::update()
	{
		if (state == ACTIVE)
		{
			pollspawns();

			backgrounds.update();
			layers.update();
			npcs.update(physics);
			mobs.update(physics);
			chars.update(physics);
			drops.update(physics);
			player.update(physics);
			portals.update(player.getposition());
			camera.update(player.getposition());
		}
	}

	void Stage::queuespawn(const Spawn* spawn)
	{
		spawnqueue.emplace(spawn);
	}

	const SpecialMove& Stage::getmove(int32_t moveid)
	{
		bool newmove = !specialmoves.count(moveid);
		if (newmove)
		{
			specialmoves[moveid] = std::make_unique<Skill>(moveid);
		}
		return *specialmoves[moveid];
	}

	void Stage::usemove(int32_t moveid)
	{
		if (!player.canattack())
			return;

		const SpecialMove& move = getmove(moveid);
		if (player.canuse(move))
			applymove(move);
	}

	void Stage::applymove(const SpecialMove& move)
	{
		bool offensive = move.isoffensive();
		if (offensive)
		{
			Attack attack = player.prepareattack(move.isskill());

			move.applyuseeffects(player, attack.type);
			move.applystats(player, attack);

			AttackResult result = mobs.sendattack(attack);
			mobs.showresult(player, move, result);

			applyusemovement(move);
			applyresultmovement(move, result);

			AttackPacket(result).dispatch();
		}
		else
		{
			move.applyuseeffects(player, Attack::MAGIC);

			int32_t moveid = move.getid();
			int32_t level = player.getskills().get_level(moveid);
			UseSkillPacket(moveid, level).dispatch();
		}
	}

	void Stage::applyusemovement(const SpecialMove& move)
	{
		switch (move.getid())
		{
		case Skill::FP_TELEPORT:
		case Skill::IL_TELEPORT:
		case Skill::PRIEST_TELEPORT:
			break;
		case Skill::FLASH_JUMP:
			break;
		}
	}

	void Stage::applyresultmovement(const SpecialMove& move, const AttackResult& result)
	{
		switch (move.getid())
		{
		case Skill::HERO_RUSH:
		case Skill::PALADIN_RUSH:
		case Skill::DK_RUSH:
			applyrush(result);
			break;
		}
	}

	void Stage::applyrush(const AttackResult& result)
	{
		if (result.mobcount == 0)
			return;

		int32_t oid = result.get_last_oid();
		auto mob = mobs.getmob(oid);
		if (mob)
		{
			int16_t targetx = mob->getposition().x();
			player.rush(targetx);
		}
	}

	void Stage::showattack(int32_t cid, const AttackResult& attack)
	{
		Optional<OtherChar> ouser = chars.getchar(cid);
		if (ouser)
		{
			OtherChar& user = *ouser;
			user.updateskill(attack.skill, attack.level);
			user.updateattack(attack.speed);

			const SpecialMove& move = getmove(attack.skill);
			move.applyuseeffects(user, attack.type);

			Stance::Value stance = Stance::byid(attack.stance);
			user.attack(stance);

			mobs.showresult(user, move, attack);
		}
	}

	void Stage::showbuff(int32_t cid, int32_t skillid, int8_t level)
	{
		Optional<OtherChar> ouser = chars.getchar(cid);
		if (ouser)
		{
			OtherChar& user = *ouser;
			user.updateskill(skillid, level);

			const SpecialMove& move = getmove(skillid);
			move.applyuseeffects(user, Attack::MAGIC);
		}
	}

	void Stage::showplayerbuff(int32_t skillid)
	{
		const SpecialMove& move = getmove(skillid);
		if (player.canuse(move))
		{
			move.applyuseeffects(player, Attack::MAGIC);
		}
	}

	void Stage::checkportals()
	{
		if (player.isattacking())
			return;

		Point<int16_t> playerpos = player.getposition();
		Portal::WarpInfo warpinfo = portals.findportal(playerpos);
		if (warpinfo.mapid == mapid)
		{
			Point<int16_t> spawnpoint = portals.get_portal_by_name(warpinfo.portal);
			Point<int16_t> startpos = physics.getgroundbelow(spawnpoint);
			player.respawn(startpos, mapinfo.isswimmap());
		}
		else if (warpinfo.valid)
		{
			ChangeMapPacket(false, warpinfo.mapid, warpinfo.portal, false).dispatch();
		}
	}

	void Stage::checkseats()
	{
		if (player.issitting() || player.isattacking())
			return;

		Optional<const Seat> seat = mapinfo.findseat(player.getposition());
		player.setseat(seat);
	}

	void Stage::checkladders(bool up)
	{
		if (player.isclimbing() || player.isattacking())
			return;

		Optional<const Ladder> ladder = mapinfo.findladder(player.getposition(), up);
		player.setladder(ladder);
	}

	void Stage::checkdrops()
	{
		Point<int16_t> playerpos = player.getposition();
		const Drop* drop = drops.findinrange(playerpos);
		if (drop)
		{
			PickupItemPacket(drop->getoid(), drop->getposition()).dispatch();
		}
	}

	void Stage::sendkey(Keyboard::Keytype type, int32_t action, bool down)
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
					checkladders(true);
					checkportals();
					break;
				case Keyboard::DOWN:
					checkladders(false);
					break;
				case Keyboard::SIT:
					checkseats();
					break;
				case Keyboard::ATTACK:
					usemove(0);
					break;
				case Keyboard::PICKUP:
					checkdrops();
					break;
				}
			}

			playable->sendaction(Keyboard::actionbyid(action), down);
			break;
		case Keyboard::SKILL:
			usemove(action);
			break;
		case Keyboard::ITEM:
			player.useitem(action);
			break;
		case Keyboard::FACE:
			//player.sendface(action);
			// for testing
			switch (action)
			{
			case 100:
				usemove(Skill::DRAGON_BUSTER);
				break;
			case 101:
				usemove(Skill::ENERGY_BOLT);
				break;
			case 102:
				usemove(Skill::MAGIC_CLAW);
				break;
			case 103:
				usemove(Skill::SLASH_BLAST);
				break;
			case 104:
				usemove(Skill::TRIPLE_THROW);
				break;
			case 105:
				usemove(Skill::AVENGER);
				break;
			case 106:
				usemove(Skill::DK_RUSH);
				break;
			}
			break;
		}
	}

	Cursor::State Stage::sendmouse(bool pressed, Point<int16_t> position)
	{
		return npcs.sendmouse(pressed, position, camera.position());
	}

	bool Stage::isplayer(int32_t cid) const
	{
		return cid == player.getoid();
	}

	MapNpcs& Stage::getnpcs()
	{
		return npcs;
	}

	MapChars& Stage::getchars()
	{
		return chars;
	}

	MapMobs& Stage::getmobs()
	{
		return mobs;
	}

	MapDrops& Stage::getdrops()
	{
		return drops;
	}

	Player& Stage::getplayer()
	{
		return player;
	}

	Optional<Char> Stage::getcharacter(int32_t cid)
	{
		if (isplayer(cid))
		{
			return player;
		}
		else
		{
			return chars.getchar(cid).cast<Char>();
		}
	}
}
