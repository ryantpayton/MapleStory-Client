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
#include "Char.h"
#include "Charstats.h"
#include "Inventory.h"
#include "Skillbook.h"
#include "Questlog.h"
#include "Telerock.h"
#include "Monsterbook.h"

using namespace Gameplay;

namespace Character
{
	class Player : public Char
	{
	public:
		Player(int, CharLook, Charstats);
		Player();
		Charstats& getstats();
		Inventory& getinvent();
		Skillbook& getskills();
		Questlog& getquests();
		Telerock& gettrock();
		Monsterbook& getmonsterbook();
		rectangle2d<int> bounds();
	private:
		Charstats stats;
		Inventory inventory;
		Skillbook skillbook;
		Questlog questlog;
		Telerock telerock;
		Monsterbook monsterbook;
	};
}

