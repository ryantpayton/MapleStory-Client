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
#include "Session.h"
#include "vector2d.h"

using namespace Util;

namespace Net
{
	class Packetcreator
	{
	public:
		Packetcreator() {}
		~Packetcreator() {}
		void init(Session*);
		void c_login(string, string);
		void accept_tos();
		void pong();
		void serverlrequest();
		void charlrequest(char, char);
		void deletechar(string, int);
		void registerpic(int, string);
		void selectcharpic(int, string);
		void selectchar(int);
		void playerlogin(int);
		void checkcharname(string);
		void createchar(string, int, int, int, int, int, int, int, int, int, bool);
		//void moveplayer(vector<movefragment>);
		void changemap(bool, int, string, bool);
		//void close_attack(attackinfo);
		void general_chat(string, bool);
		void talktonpc(int);
		void moveitem(char, short, char, short);
		void useitem(short, int);
		void scrollequip(short, short, bool);
		void spendap(int);
		//void movemonster(int, short, byte, byte, byte, byte, byte, byte, vector2d, vector<movefragment>);
		void pickupitem(int, vector2d<int32_t>);
	private:
		//void writemoves(OutPacket*, vector<movefragment>);
		Session* session;
	};
}

