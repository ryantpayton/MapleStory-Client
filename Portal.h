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
#include "Animation.h"

using namespace Graphics;

namespace Gameplay
{
	enum PortalType
	{
		PT_SPAWN,
		PT_INVISIBLE,
		PT_REGULAR,
		PT_TOUCH,
		PT_4,
		PT_5,
		PT_WARP,
		PT_SCRIPTED,
		PT_SCRIPTED_INVISIBLE,
		PT_SCRIPTED_TOUCH,
		PT_HIDDEN,
		PT_SCRIPTED_HIDDEN,
		PT_SPRING1,
		PT_SPRING2,
		PT_14
	};

	class Portal
	{
	public:
		Portal(PortalType, string, int, bool, string, vector2d<int>);
		virtual ~Portal(){}
		virtual void draw(vector2d<int>){}
		virtual void settouch(bool){}
		string getname();
		PortalType gettype();
		vector2d<int> getposition();
		rectangle2d<int> bounds();
		pair<int, string> getwarpinfo();
	protected:
		PortalType type;
		string name;
		int targetid;
		bool intermap;
		string targetpname;
		vector2d<int> position;
	};
}