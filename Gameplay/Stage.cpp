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
#include "Net\Packets\GameplayPackets83.h"
#include "nlnx\nx.hpp"
#include "nlnx\audio.hpp"

namespace Gameplay
{
	Stage::Stage(ClientInterface& cl) : client(cl)
	{
		playerid = 0;
		active = false;
	}

	void Stage::init()
	{
		portals.init();
	}

	bool Stage::loadplayer(int32_t cid)
	{
		using::Net::CharEntry;
		const CharEntry& entry = client.getsession().getlogin().getaccount().getcharbyid(cid);
		if (entry.getcid() == cid)
		{
			player = Player(cid, entry.getlook(), entry.getstats());
			playerid = cid;
			playable = &player;
			return true;
		}
		else
		{
			return false;
		}
	}

	void Stage::loadmap(int32_t mapid)
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
		for (uint8_t i = 0; i < NUM_LAYERS; i++)
		{
			layers[i] = MapLayer(src[std::to_string(i)]);
		}

		currentmapid = mapid;
	}

	void Stage::respawn()
	{
		if (mapinfo.hasnewbgm())
		{
			using::nl::audio;
			audio toplay = nl::nx::sound.resolve(mapinfo.getbgm()).get_audio();
			if (toplay.data())
			{
				client.getaudio().playbgm((void*)toplay.data(), toplay.length());
			}
		}

		vector2d<int32_t> startpos = portals.getspawnpoint(player.getstats().getportal());
		player.respawn(startpos);
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

	void Stage::update()
	{
		if (active)
		{
			for (uint8_t i = 0; i < NUM_LAYERS; i++)
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
				client.getsession().dispatch(MovePlayerPacket83(playermovement));

				player.clearmovement();
			}
		}
	}

	void Stage::sendkey(Keytype type, int32_t action, bool down)
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
						checkladders();
						checkportals();
						break;
					case IO::KA_DOWN:
						checkladders();
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

	void Stage::checkportals()
	{
		const WarpInfo* warpinfo = portals.findportal(player.getbounds());
		if (warpinfo)
		{
			if (warpinfo->mapid == currentmapid)
			{
				vector2d<int32_t> spawnpoint = portals.getspawnpoint(warpinfo->portal);
				player.respawn(spawnpoint);
			}
			else if (warpinfo->valid)
			{
				using::Net::ChangeMapPacket83;
				client.getsession().dispatch(ChangeMapPacket83(false, warpinfo->mapid, warpinfo->portal, false));
			}
		}
	}

	void Stage::checkseats()
	{
		if (player.issitting())
			return;

		const Seat* seat = mapinfo.findseat(player.getposition());
		player.setseat(seat);
	}

	void Stage::checkladders()
	{
		if (player.isclimbing())
			return;

		const Ladder* ladder = mapinfo.findladder(player.getposition());
		player.setladder(ladder);
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

	Player& Stage::getplayer()
	{
		return player;
	}
}
