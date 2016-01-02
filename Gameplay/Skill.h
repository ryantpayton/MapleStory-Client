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
#include <string>
#include <map>

namespace Gameplay
{
	using std::int32_t;
	using std::string;
	using std::map;
	using std::vector;
	using Util::vector2d;
	using Util::rectangle2d;
	using Graphics::Texture;
	using Graphics::Animation;

	struct SkillLevel
	{
		float chance = 0.0f;
		float damage = 0.0f;
		float critical = 0.0f;
		float ignoredef = 0.0f;
		uint8_t attackcount = 1;
		uint8_t mobcount = 1;
		int32_t hpcost = 0;
		int32_t mpcost = 0;
		rectangle2d<int16_t> range;
	};

	class Skill
	{
	public:
		Skill(int32_t);
		Skill();
		~Skill();

		bool isoffensive() const;
		int32_t getid() const;
		string getaction(bool twohanded) const;
		Animation gethitanimation(bool twohanded) const;
		Animation geteffect(bool twohanded) const;
		const Texture* geticon(uint8_t type) const;
		const SkillLevel* getlevel(int32_t level) const;

	private:
		int32_t id;
		map<uint8_t, Texture> icons;
		vector<Animation> effects;
		vector<Animation> hit;
		Animation affected;
		string preparestance;
		int32_t preparetime;
		bool offensive;
		vector<string> actions;
		map<int32_t, SkillLevel> levels;
	};
}