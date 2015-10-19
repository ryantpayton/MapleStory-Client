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
#pragma once
#include "Mapinfo.h"
#include "Maplayers.h"
#include "Mapportals.h"
#include "Mapchars.h"
#include "Camera.h"
#include "Player.h"

using namespace Character;

namespace Gameplay
{
	class Stage
	{
	public:
		Stage();
		~Stage();
		void init();
		void loadplayer(int, CharLook, Charstats);
		void loadmap(int);
		void respawn();
		void draw();
		void update(short);
		Player& getplayer();
	private:
		Player player;
		Mapinfo mapinfo;
		Maplayers layers;
		Mapportals portals;
		Mapchars chars;
		Camera camera;
		bool active;
		int playerid;
	};
}

