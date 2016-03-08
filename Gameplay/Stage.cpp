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
#include "Net\InPacket.h"
#include "Net\Packets\GameplayPackets.h"
#include "Net\Packets\AttackAndSkillPackets.h"
#include "Net\Session.h"
#include "Audio\AudioPlayer.h"
#include "nlnx\nx.hpp"
#include "nlnx\audio.hpp"

namespace Gameplay
{
	using Audio::AudioPlayer;
	using Net::Session;

	Stage::Stage()
	{
		state = INACTIVE;
		mapid = 0;
		portalid = 0;
	}

	bool Stage::loadplayer(int32_t charid)
	{
		player = Session::get().getlogin().getcharbyid(charid);
		playable = player;

		return isplayer(charid);
	}

	void Stage::setmap(uint8_t pid, int32_t mid)
	{
		portalid = pid;
		mapid = mid;
	}

	void Stage::clear()
	{
		state = INACTIVE;

		layers.clear();
		portals.clear();
		chars.clear();
		npcs.clear();
		mobs.clear();
		drops.clear();
	}

	void Stage::loadmap()
	{
		string strid = std::to_string(mapid);
		strid.insert(0, 9 - strid.length(), '0');
		node src = nl::nx::map["Map"]["Map" + std::to_string(mapid / 100000000)][strid + ".img"];

		portals.load(src["portal"], mapid);
		physics.load(src);
		backgrounds = src["back"];
		mapinfo = MapInfo(src, physics.getfht().getwalls(), physics.getfht().getborders());
		for (uint8_t i = 0; i < MapLayer::NUM_LAYERS; i++)
		{
			layers[i] = MapLayer(src[std::to_string(i)]);
		}
	}

	void Stage::parsemap(InPacket& recv)
	{
		if (state != INACTIVE)
			return;

		mapid = recv.readint();
		portalid = recv.readbyte();
		mapinfo = MapInfo(recv);
		physics.parsefht(recv);
		portals.parseportals(recv, mapid);

		for (uint8_t i = 0; i < MapLayer::NUM_LAYERS; i++)
		{
			layers[i] = MapLayer(recv);
		}

		state = TRANSITION;
	}

	void Stage::respawn()
	{
		AudioPlayer::get().playbgm(mapinfo.getbgm());

		Point<int16_t> startpos = physics.getgroundbelow(portals.getspawnpoint(portalid));
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

	void Stage::draw(float inter) const
	{
		if (state != ACTIVE)
			return;

		Point<int16_t> viewpos = camera.getposition(inter);
		backgrounds.drawbackgrounds(viewpos, inter);
		for (uint8_t i = 0; i < MapLayer::NUM_LAYERS; i++)
		{
			layers.at(i).draw(viewpos, inter);
			npcs.draw(i, camera, inter);
			mobs.draw(i, camera, inter);
			chars.draw(i, camera, inter);
			if (i == player.getlayer())
			{
				player.draw(camera, inter);
			}
			drops.draw(i, camera, inter);
		}
		portals.draw(viewpos, inter);
		backgrounds.drawforegrounds(viewpos, inter);

		physics.getfht().draw(viewpos);
	}

	void Stage::pollspawns()
	{
		while (spawnqueue.size() > 0)
		{
			const Spawn* spawn = spawnqueue.back().get();
			switch (spawn->type())
			{
			case Spawn::NPC:
				npcs.add(spawn->instantiate(physics));
				break;
			case Spawn::MOB:
				mobs.add(spawn->instantiate(physics));
				break;
			case Spawn::CHARACTER:
				chars.add(spawn->instantiate(physics));
				break;
			}
			spawnqueue.pop_back();
		}
	}

	void Stage::update()
	{
		if (state == ACTIVE)
		{
			pollspawns();

			backgrounds.update();
			for (uint8_t i = 0; i < MapLayer::NUM_LAYERS; i++)
			{
				layers[i].update();
			}

			npcs.update(physics);
			mobs.update(physics);
			chars.update(physics);
			drops.update(physics);
			player.update(physics);
			portals.update(player.getbounds());
			camera.update(player.getposition());
		}
	}

	void Stage::queuespawn(const Spawn* spawn)
	{
		spawnqueue.push_back(unique_ptr<const Spawn>(spawn));
	}

	void Stage::useskill(int32_t skillid)
	{
		using Character::Skill;
		const Skill& skill = player.useskill(skillid);

		if (skill.isoffensive())
		{
			Attack attack = player.prepareskillattack(skillid);
			mobs.sendattack(attack);
		}
		else
		{
			uint8_t levelbyte = static_cast<uint8_t>(player.getskills().getlevel(skillid));

			using Net::UseSkillPacket;
			Session::get().dispatch(UseSkillPacket(skillid, levelbyte));
		}
	}

	void Stage::useattack()
	{
		if (!player.canattack())
			return;

		Attack attack = player.prepareregularattack();
		mobs.sendattack(attack);
	}

	void Stage::checkportals()
	{
		if (player.isattacking())
			return;

		const WarpInfo* warpinfo = portals.findportal(player.getbounds());
		if (warpinfo)
		{
			if (warpinfo->mapid == mapid)
			{
				Point<int16_t> spawnpoint = portals.getspawnpoint(warpinfo->portal);
				player.respawn(spawnpoint, mapinfo.isswimmap());
			}
			else if (warpinfo->valid)
			{
				using Net::ChangeMapPacket;
				Session::get().dispatch(ChangeMapPacket(false, warpinfo->mapid, warpinfo->portal, false));
			}
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
		const Drop* drop = drops.findinrange(player.getbounds());
		if (drop)
		{
			using Net::PickupItemPacket;
			Session::get().dispatch(PickupItemPacket(drop->getoid(), drop->getposition()));
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
					useattack();
					break;
				case Keyboard::PICKUP:
					checkdrops();
					break;
				}
			}

			playable->sendaction(Keyboard::actionbyid(action), down);
			break;
		case Keyboard::SKILL:
			if (player.canuseskill(action))
				useskill(action);
			break;
		case Keyboard::ITEM:
			player.useitem(action);
			break;
		case Keyboard::FACE:
			player.sendface(action);
			break;
		}
	}

	Cursor::State Stage::sendmouse(bool pressed, Point<int16_t> position)
	{
		return npcs.sendmouse(pressed, position, camera.getposition(1.0f));
	}

	bool Stage::isplayer(int32_t cid) const
	{
		return player.getoid() == cid;
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
		return isplayer(cid) ?
		player : chars.getchar(cid).cast<Char>();
	}
}
