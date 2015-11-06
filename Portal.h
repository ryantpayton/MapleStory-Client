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
#include "rectangle2d.h"
#include <cstdint>
#include <map>

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

	using::std::int32_t;
	using::std::string;
	using::std::pair;
	using::Util::vector2d;
	using::Util::rectangle2d;

	// Class that represents a portal on a map.
	class Portal
	{
	public:
		Portal(PortalType, string, int32_t, bool, string, vector2d<int32_t>);
		virtual ~Portal() {}
		virtual void draw(vector2d<int32_t>) const {}
		virtual void settouch(bool) {}
		const string& getname() const;
		PortalType gettype() const;
		vector2d<int32_t> getposition() const;
		rectangle2d<int32_t> bounds() const;
		const pair<int32_t, string>* getwarpinfo() const;
	protected:
		PortalType type;
		string name;
		pair<int32_t, string> warpinfo;
		bool intermap;
		vector2d<int32_t> position;
	};
}