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
#include "Util\BoolPair.h"
#include "Util\Optional.h"
#include "Util\rectangle2d.h"
#include <cstdint>
#include <string>
#include <unordered_map>

namespace Gameplay
{
	using std::int32_t;
	using std::string;
	using std::unordered_map;
	using std::vector;
	using Graphics::Texture;
	using Graphics::Animation;

	class Skill
	{
	public:
		enum Name
		{
			BRANDISH = 1121008,
			SACRIFICE = 1311005,
			DRAGONS_ROAR = 1311006
		};

		enum IconType
		{
			NORMAL,
			MOUSEOVER,
			DISABLED
		};

		struct Level
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
			BoolPair<vector<uint16_t>> hitdelays;
		};

		Skill(int32_t);
		Skill();
		~Skill();

		bool isoffensive() const;
		bool canbetwohanded() const;
		int32_t getid() const;
		string getaction(bool twohanded) const;

		Animation gethitanimation(bool twohanded) const;
		Animation geteffect(bool twohanded) const;
		const Texture& geticon(IconType type) const;
		const Level* getlevel(int32_t level) const;

		static void init();

	private:
		int32_t skillid;
		unordered_map<int32_t, Level> levels;
		unordered_map<IconType, Texture> icons;
		BoolPair<Animation> useeffects;
		bool oneuseeffect;
		BoolPair<Animation> hiteffects;
		bool onehiteffect;
		BoolPair<string> actions;
		bool oneaction;
		Animation affected;
		string preparestance;
		int32_t preparetime;
		bool offensive;

		static unordered_map<int32_t, BoolPair<vector<int16_t>>> attackframes;
	};
}