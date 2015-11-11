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
#include "Graphics\Animation.h"
#include "Util\rectangle2d.h"
#include <cstdint>
#include <map>

namespace Gameplay
{
	using::std::int32_t;
	using::std::string;
	using::std::pair;
	using::Util::vector2d;
	using::Util::rectangle2d;
	using::Graphics::Animation;

	class Portal
	{
	public:
		enum PtType
		{
			SPAWN,
			INVISIBLE,
			REGULAR,
			TOUCH,
			TYPE4,
			TYPE5,
			WARP,
			SCRIPTED,
			SCRIPTED_INVISIBLE,
			SCRIPTED_TOUCH,
			HIDDEN,
			SCRIPTED_HIDDEN,
			SPRING1,
			SPRING2,
			TYPE14
		};

		Portal(const Animation*, PtType, string, bool, vector2d<int32_t>, int32_t, string);
		Portal();
		~Portal();
		void draw(vector2d<int32_t>, float) const;
		void settouch(bool);
		string getname() const;
		PtType gettype() const;
		vector2d<int32_t> getposition() const;
		rectangle2d<int32_t> bounds() const;
		const pair<int32_t, string>* getwarpinfo() const;
	private:
		const Animation* anim;
		PtType type;
		string name;
		bool intermap;
		vector2d<int32_t> pos;
		pair<int32_t, string> warpinfo;
		bool touched;
	};
}

