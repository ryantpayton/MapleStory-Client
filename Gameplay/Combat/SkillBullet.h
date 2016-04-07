//////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2016 Daniel Allendorf                                        //
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
#include "Character\Char.h"
#include "Graphics\Animation.h"

#include <unordered_map>

namespace Gameplay
{
	using Character::Char;
	using Graphics::Animation;

	class SkillBullet
	{
	public:
		virtual ~SkillBullet() {}

		virtual Animation get(const Char& user, int32_t bulletid) const = 0;

	protected:
		struct Ball
		{
			Animation animation;

			Ball(node src)
			{
				animation = src;
			}

			Ball() {}
		};
	};


	class RegularBullet : public SkillBullet
	{
	public:
		Animation get(const Char& user, int32_t bulletid) const override;
	};


	class SingleBullet : public SkillBullet
	{
	public:
		SingleBullet(node src);

		Animation get(const Char& user, int32_t bulletid) const override;

	private:
		Ball ball;
	};


	class BySkillLevelBullet : public SkillBullet
	{
	public:
		BySkillLevelBullet(node src, int32_t skillid);

		Animation get(const Char& user, int32_t bulletid) const override;

	private:
		std::unordered_map<int32_t, Ball> bullets;
		int32_t skillid;
	};
}