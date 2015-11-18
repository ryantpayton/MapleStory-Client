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

		Player player;

		Playable* playable = nullptr;
		int32_t currentmapid = 0;
		bool active = false;

		void init()
		{
			portals.init();
		}

		bool loadplayer(int32_t charid)
		{
			const Net::CharEntry& entry = 
				Net::Session::getlogin().getaccount().getcharbyid(charid);
			if (entry.getcid() == charid)
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

		void loadmap(int32_t mapid)
		{
			active = false;

			layers.clear();
			portals.clear();
			chars.clear();
			npcs.clear();
			mobs.clear();

			string strid = std::to_string(mapid);
			strid.insert(0, 9 - strid.length(), '0');
			node src = nl::nx::map["Map"]["Map" + std::to_string(mapid / 100000000)][strid + ".img"];

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
			if (mapinfo.hasnewbgm())
				Audioplayer::playbgm(mapinfo.getbgm());

			vector2d<int16_t> startpos = portals.getspawnpoint(player.getstats().getportal());
			player.respawn(startpos);
			camera.setposition(startpos);
			camera.updateview(mapinfo.getwalls(), mapinfo.getborders());

			active = true;
		}

		void draw(float inter)
		{
			if (active)
			{
				vector2d<int16_t> viewpos = camera.getposition(inter);

				for (uint8_t i = 0; i < MapLayer::NUM_LAYERS; i++)
				{
					layers.at(i).draw(viewpos, inter);
					npcs.draw(i, viewpos, inter);
					mobs.draw(i, viewpos, inter);
					chars.draw(i, viewpos, inter);

					if (i == player.getlayer())
					{
						player.draw(viewpos, inter);
					}
				}
				portals.draw(viewpos, inter);
			}
		}

		void update()
		{
			if (active)
			{
				for (uint8_t i = 0; i < MapLayer::NUM_LAYERS; i++)
				{
					layers[i].update();
				}

				npcs.update(physics);
				mobs.update(physics);
				chars.update(physics);
				player.update(physics);
				portals.update(player.getbounds());
				camera.update(player.getposition());

				using::Gameplay::MovementInfo;
				const MovementInfo& playermovement = player.getmovement();
				if (playermovement.getsize() > 0)
				{
					using::Net::MovePlayerPacket83;
					Net::Session::dispatch(MovePlayerPacket83(playermovement));

					player.clearmovement();
				}
			}
		}

		void checkportals()
		{
			const WarpInfo* warpinfo = portals.findportal(player.getbounds());
			if (warpinfo)
			{
				if (warpinfo->mapid == currentmapid)
				{
					vector2d<int16_t> spawnpoint = portals.getspawnpoint(warpinfo->portal);
					player.respawn(spawnpoint);
				}
				else if (warpinfo->valid)
				{
					using::Net::ChangeMapPacket83;
					Net::Session::dispatch(ChangeMapPacket83(false, warpinfo->mapid, warpinfo->portal, false));
				}
			}
		}

		void checkseats()
		{
			if (player.issitting())
				return;

			const Seat* seat = mapinfo.findseat(player.getposition());
			player.setseat(seat);
		}

		void checkladders(bool up)
		{
			if (player.isclimbing())
				return;

			const Ladder* ladder = mapinfo.findladder(player.getposition(), up);
			player.setladder(ladder);
		}

		void sendkey(IO::Keytype type, int32_t action, bool down)
		{
			switch (type)
			{
			case IO::KT_ACTION:
				if (playable)
				{
					if (down)
					{
						switch (action)
						{
						case IO::KA_UP:
							checkladders(true);
							checkportals();
							break;
						case IO::KA_DOWN:
							checkladders(false);
							break;
						case IO::KA_SIT:
							checkseats();
							break;
						}
					}

					Keyaction ka = static_cast<Keyaction>(action);
					playable->sendaction(ka, down);
				}
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
