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
#include "Camera.h"
#include "Skill.h"
#include "Maplemap\MapInfo.h"
#include "Maplemap\MapLayer.h"
#include "Maplemap\MapPortals.h"
#include "Physics\Physics.h"

#include "Net\Packets\GameplayPackets83.h"
#include "Net\Session.h"
#include "Audio\Audioplayer.h"
#include "nlnx\nx.hpp"
#include "nlnx\audio.hpp"

namespace Gameplay
{
	namespace Stage
	{
		Camera camera;
		Physics physics;

		map<uint8_t, MapLayer> layers;
		MapInfo mapinfo;
		MapPortals portals;
		MapNpcs npcs;
		MapChars chars;
		MapMobs mobs;
		MapDrops drops;

		Player player;

		Playable* playable = nullptr;
		int32_t currentmapid = 0;
		bool active = false;

		bool loadplayer(int32_t charid)
		{
			const Net::CharEntry& entry = Net::Session::getlogin().getcharbyid(charid);
			if (entry.cid == charid)
			{
				player = Player(entry);
				playable = &player;
				return true;
			}
			else
			{
				return false;
			}
		}

		void warptomap(uint8_t portalid, int32_t mapid)
		{
			player.getstats().setportal(portalid);
			loadmap(mapid);
		}

		void loadmap(int32_t mapid)
		{
			// Load the map with the given id.
			active = false;

			// Clear all objects on the last map.
			layers.clear();
			portals.clear();
			chars.clear();
			npcs.clear();
			mobs.clear();
			drops.clear();

			// Get the node that has the new map data.
			string strid = std::to_string(mapid);
			strid.insert(0, 9 - strid.length(), '0');
			node src = nl::nx::map["Map"]["Map" + std::to_string(mapid / 100000000)][strid + ".img"];

			// Load new map data.
			physics.loadfht(src["foothold"]);
			mapinfo.loadinfo(src, physics.getfht().getwalls(), physics.getfht().getborders());
			portals.load(src["portal"], mapid);
			//backgrounds = mapbackgrounds(src["back"]);
			for (uint8_t i = 0; i < MapLayer::NUM_LAYERS; i++)
			{
				layers[i] = MapLayer(src[std::to_string(i)]);
			}

			currentmapid = mapid;
		}

		void respawn()
		{
			// Change the background music if required.
			if (mapinfo.hasnewbgm())
				Audioplayer::playbgm(mapinfo.getbgm());

			// Respawn the player at the spawnpoint associated with the portal id.
			vector2d<int16_t> startpos = 
				portals.getspawnpoint(player.getstats().getportal());
			player.respawn(startpos);
			camera.setposition(startpos);
			camera.updateview(mapinfo.getwalls(), mapinfo.getborders());

			active = true;
		}

		void draw(float inter)
		{
			if (!active)
				return;

			vector2d<int16_t> viewpos = camera.getposition(inter);
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
		}

		void update()
		{
			if (!active)
				return;

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
			camera.update(player.getrealposition());
		}

		void sendattack(const Attack& attack)
		{
			AttackResult result = mobs.sendattack(attack);
			Net::Session::dispatch(Net::CloseRangeAttackPacket83(result));
		}

		void useskill(int32_t skillid)
		{
			static map<int32_t, Skill> skillcache;
			if (!skillcache.count(skillid))
				skillcache[skillid] = Skill(skillid);
			const Skill& skill = skillcache[skillid];

			const SkillLevel* skilllevel = skill.getlevel(player.getskills().getlevelof(skillid));
			if (skilllevel == nullptr)
				return;

			const Character::Weapon* weapon = player.getlook().getequips().getweapon();
			if (weapon == nullptr)
				return;

			player.getlook().setaction(skill.getaction(weapon->istwohanded()));

			if (skill.isoffensive())
			{
				if (!player.canattack())
					return;

				Attack attack = player.prepareattack();
				attack.skill = skillid;
				attack.mobcount = skilllevel->mobcount;
				attack.hitcount = skilllevel->attackcount;
				attack.range = skilllevel->range;
				sendattack(attack);
			}
			else
			{

			}
		}

		void useattack()
		{
			if (!player.canattack())
				return;

			Attack attack = player.regularattack();
			sendattack(attack);
		}

		void useitem(int32_t itemid)
		{
			// Use the first item with the given id found in the inventory.
			using Character::Inventory;
			Inventory::InvType type = player.getinvent().gettypebyid(itemid);
			int16_t slot = player.getinvent().finditem(type, itemid);

			if (slot < 0)
				return;

			switch (type)
			{
			case Inventory::USE:
				using Net::UseItemPacket83;
				Net::Session::dispatch(UseItemPacket83(slot, itemid));
				break;
			}
		}

		void checkportals()
		{
			if (player.isattacking())
				return;

			// Check for portals within the player's range.
			const WarpInfo* warpinfo = portals.findportal(player.getbounds());
			if (warpinfo)
			{
				if (warpinfo->mapid == currentmapid)
				{
					// Teleport inside a map.
					vector2d<int16_t> spawnpoint = portals.getspawnpoint(warpinfo->portal);
					player.respawn(spawnpoint);
				}
				else if (warpinfo->valid)
				{
					// Warp to a different map.
					using Net::ChangeMapPacket83;
					Net::Session::dispatch(ChangeMapPacket83(false, warpinfo->mapid, warpinfo->portal, false));
				}
			}
		}

		void checkseats()
		{
			if (player.issitting() || player.isattacking())
				return;

			const Seat* seat = mapinfo.findseat(player.getposition());
			player.setseat(seat);
		}

		void checkladders(bool up)
		{
			if (player.isclimbing() || player.isattacking())
				return;

			const Ladder* ladder = mapinfo.findladder(player.getposition(), up);
			player.setladder(ladder);
		}

		void checkdrops()
		{
			const Drop* drop = drops.findinrange(player.getbounds());
			if (drop)
			{
				Net::Session::dispatch(Net::PickupItemPacket(drop->getoid(), drop->getposition()));
			}
		}

		void sendkey(IO::Keyboard::Keytype type, int32_t action, bool down)
		{
			if (!playable)
				return;

			using IO::Keyboard;

			switch (type)
			{
			case Keyboard::KT_ACTION:
				if (down)
				{
					// Handle key actions which require parts of map data.
					switch (action)
					{
					case Keyboard::KA_UP:
						checkladders(true);
						checkportals();
						break;
					case Keyboard::KA_DOWN:
						checkladders(false);
						break;
					case Keyboard::KA_SIT:
						checkseats();
						break;
					case Keyboard::KA_ATTACK:
						useattack();
						break;
					case Keyboard::KA_PICKUP:
						checkdrops();
						break;
					}
				}

				// Pass the action to the playable mapobject.
				playable->sendaction(static_cast<Keyboard::Keyaction>(action), down);
				break;
			case Keyboard::KT_SKILL:
				useskill(action);
				break;
			case Keyboard::KT_ITEM:
				useitem(action);
				break;
			case Keyboard::KT_FACE:
				player.sendface(action);
				break;
			}
		}

		MapNpcs& getnpcs()
		{
			return npcs;
		}

		MapChars& getchars()
		{
			return chars;
		}

		MapMobs& getmobs()
		{
			return mobs;
		}

		MapDrops& getdrops()
		{
			return drops;
		}

		Player& getplayer()
		{
			return player;
		}

		Char* getcharacter(int32_t cid)
		{
			if (cid == player.getoid())
				return &player;
			else
				return chars.getchar(cid);
		}
	}
}
