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
#include "BodyAction.h"
#include "vector2d.h"
#include <map>

using namespace Util;

namespace Data
{
	class BodyDrawinfo
	{
	public:
		BodyDrawinfo(){}
		~BodyDrawinfo(){}
		void init();
		vector2d<int> getbodypos(string, uint8_t);
		vector2d<int> getarmpos(string, uint8_t);
		vector2d<int> gethandpos(string, uint8_t);
		vector2d<int> getheadpos(string, uint8_t);
		vector2d<int> gethairpos(string, uint8_t);
		vector2d<int> getfacepos(string, uint8_t);
		uint8_t nextframe(string, uint8_t);
		short getdelay(string, uint8_t);
		uint8_t nextacframe(string, uint8_t);
		BodyAction* getaction(string, uint8_t);
	private:
		map<string, map<uint8_t, vector2d<int>>> bodyposmap;
		map<string, map<uint8_t, vector2d<int>>> armposmap;
		map<string, map<uint8_t, vector2d<int>>> handposmap;
		map<string, map<uint8_t, vector2d<int>>> headposmap;
		map<string, map<uint8_t, vector2d<int>>> hairposmap;
		map<string, map<uint8_t, vector2d<int>>> faceposmap;
		map<string, map<uint8_t, short>> stancedelays;
		map<string, map<uint8_t, BodyAction>> bodyactions;
	};
}