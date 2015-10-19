/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015 SYJourney                                               //
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
#include "nx.hpp"

namespace Gameplay
{
	Stage::Stage()
	{
		playerid = -1;
		active = false;
	}

	Stage::~Stage()
	{
	}

	void Stage::init()
	{
		portals.init();
	}

	void Stage::loadplayer(int cid, CharLook look, Charstats stats)
	{
		if (playerid < 0)
		{
			player = Player(cid, look, stats);
			playerid = cid;
		}
	}

	void Stage::loadmap(int mapid)
	{
		active = false;

		layers.clear();
		portals.clear();
		chars.clear();

		/*sdata->clear();

		mapdata* mdata = Game::getcache()->getmap();
		mdata->load(mapid);*/

		string strid = to_string(mapid);
		strid.insert(0, 9 - strid.length(), '0');
		node src = nx::map["Map"]["Map" + to_string(mapid / 100000000)][strid + ".img"];

		portals.load(src["portal"], mapid);
		//backgrounds = mapbackgrounds(src["back"]);
		layers.load(src);
	}

	void Stage::respawn()
	{
		/*if (mapinfo.hasnewbgm())
		{
			audio toplay = nx::sound.resolve(mapinfo.getbgm()).get_audio();
			if (toplay.data())
			{
				audiopb.playbgm((void*)toplay.data(), toplay.length());
			}
		}*/

		vector2d<int> startpos = portals.getspawnpoint(player.getstats().getportal());
		//player.updatefht();
		player.setposition(startpos);
		camera.setposition(startpos);
		camera.updateview(mapinfo.getwalls(), mapinfo.getborders());

		active = true;
	}

	void Stage::draw()
	{
		if (active)
		{
			vector2d<int> viewpos = camera.getposition();
			for (char i = 0; i < 8; i++)
			{
				layers.draw(i, viewpos);
				chars.draw(i, viewpos);
				if (i == player.getlayer())
				{
					player.draw(viewpos);
				}
			}
			portals.draw(viewpos);
		}
	}

	void Stage::update(short dpf)
	{
		if (active)
		{
			layers.update(dpf);
			chars.update(dpf);
			player.update(dpf);
			portals.update(player.bounds(), dpf);
			camera.update(player.getposition(), dpf);
		}
	}

	Player& Stage::getplayer()
	{
		return player;
	}
}
