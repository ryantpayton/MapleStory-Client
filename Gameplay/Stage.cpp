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
#pragma once
#include "Stage.h"
#include "Maplemap\Npc.h"
#include "Net\Packets\GameplayPackets83.h"
#include "nlnx\nx.hpp"
#include "nlnx\audio.hpp"

namespace Gameplay
{
	Stage::Stage(ClientInterface& cl) : client(cl)
	{
		playerid = -1;
		active = false;
	}

	void Stage::init()
	{
		portals.init();
	}

	void Stage::loadplayer(int32_t cid, CharLook look, Charstats stats)
	{
		if (playerid < 0)
		{
			player = Player(cid, look, stats);
			playerid = cid;
			playable = &player;
		}
	}

	void Stage::loadmap(int32_t mid)
	{
		active = false;

		layers.clear();
		portals.clear();
		chars.clear();
		npcs.clear();

		string strid = std::to_string(mid);
		strid.insert(0, 9 - strid.length(), '0');
		node src = nl::nx::map["Map"]["Map" + std::to_string(mid / 100000000)][strid + ".img"];

		physics.loadfht(src["foothold"]);
		mapinfo.loadinfo(src, physics.getfht().getwalls(), physics.getfht().getborders());
		portals.load(src["portal"], mid);
		//backgrounds = mapbackgrounds(src["back"]);
		for (uint8_t i = 0; i < NUM_LAYERS; i++)
		{
			layers[i] = MapLayer(src[std::to_string(i)]);
		}

		mapid = mid;
	}

	void Stage::respawn(Audioplayer& audiopb)
	{
		if (mapinfo.hasnewbgm())
		{
			using::nl::audio;
			audio toplay = nl::nx::sound.resolve(mapinfo.getbgm()).get_audio();
			if (toplay.data())
			{
				audiopb.playbgm((void*)toplay.data(), toplay.length());
			}
		}

		vector2d<int32_t> startpos = portals.getspawnpoint(player.getstats().getportal()) - vector2d<int32_t>(0, 40);
		player.setposition(startpos.x(), startpos.y());
		camera.setposition(startpos);
		camera.updateview(mapinfo.getwalls(), mapinfo.getborders());

		active = true;
	}

	void Stage::draw(float inter) const
	{
		if (active)
		{
			vector2d<int32_t> viewpos = camera.getposition(inter);

			for (uint8_t i = 0; i < NUM_LAYERS; i++)
			{
				layers.at(i).draw(viewpos, inter);
				npcs.draw(i, viewpos, inter);
				chars.draw(i, viewpos, inter);

				if (i == player.getlayer())
				{
					player.draw(viewpos, inter);
				}
			}
			portals.draw(viewpos, inter);
		}
	}

	void Stage::update()
	{
		if (active)
		{
			for (uint8_t i = 0; i < NUM_LAYERS; i++)
			{
				layers[i].update();
			}

			npcs.update(physics);
			chars.update(physics);
			player.update(physics);
			portals.update(player.bounds());
			camera.update(player.getposition());

			using::Gameplay::MovementInfo;
			const MovementInfo& playermovement = player.getmovement();
			if (playermovement.getsize() > 1)
			{
				using::Net::MovePlayerPacket83;
				client.getsession().dispatch(MovePlayerPacket83(playermovement));

				player.clearmovement();
			}
		}
	}

	void Stage::sendkey(Keytype type, int32_t action, bool down)
	{
		if (type == IO::KT_ACTION)
		{
			if (down)
			{
				switch (action)
				{
				case IO::KA_UP:
					checkportal();
					break;
				case IO::KA_SIT:
					checkseats();
					break;
				}
			}

			Keyaction ka = static_cast<Keyaction>(action);
			if (playable != 0)
			{
				playable->sendaction(ka, down);
			}
		}
	}

	void Stage::checkportal()
	{
		using::std::pair;
		const pair<int32_t, string>* warpinfo = portals.findportal(player.bounds());
		if (warpinfo)
		{
			int32_t mid = warpinfo->first;
			string pname = warpinfo->second;
			if (mid < 999999999)
			{
				if (mid == mapid)
				{
					vector2d<int32_t> spawnpoint = portals.getspawnpoint(pname) - vector2d<int32_t>(0, 40);
					player.setposition(spawnpoint.x(), spawnpoint.y());
				}
				else
				{
					using::Net::ChangeMapPacket83;
					client.getsession().dispatch(ChangeMapPacket83(false, mid, pname, false));
				}
			}
		}
	}

	void Stage::checkseats()
	{
		const vector2d<int32_t>* seat = mapinfo.getseat(player.getposition());
		if (seat)
		{
			player.setposition(seat->x(), seat->y());
			player.setstance(Character::PST_SIT);
		}
	}

	void Stage::addnpc(int32_t id, int32_t oid, bool flip, uint16_t fhid, int32_t x, int32_t y)
	{
		npcs.add(new Npc(id, oid, flip, fhid, x, y));
	}

	Mapmobs& Stage::getmobs()
	{
		return mobs;
	}

	Player& Stage::getplayer()
	{
		return player;
	}
}
