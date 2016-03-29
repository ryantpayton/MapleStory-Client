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
#include "Attack.h"
#include "Character\Char.h"

namespace Gameplay
{
	using Character::Char;

	class SkillAction
	{
	public:
		virtual ~SkillAction() {}

		virtual void apply(Char& target, Attack::Type atype) const = 0;
	};


	class NoAction : public SkillAction
	{
	public:
		void apply(Char&, Attack::Type) const override {}
	};


	class RegularAction : public SkillAction
	{
	public:
		void apply(Char& target, Attack::Type atype) const override;
	};


	class SingleAction : public SkillAction
	{
	public:
		SingleAction(node src);

		void apply(Char& target, Attack::Type atype) const override;

	private:
		string action;
	};


	class TwoHAction : public SkillAction
	{
	public:
		TwoHAction(node src);

		void apply(Char& target, Attack::Type atype) const override;

	private:
		BoolPair<string> actions;
	};


	class ByLevelAction : public SkillAction
	{
	public:
		ByLevelAction(node src, int32_t skillid);

		void apply(Char& target, Attack::Type atype) const override;

	private:
		unordered_map<int32_t, string> actions;
		int32_t skillid;
	};
}