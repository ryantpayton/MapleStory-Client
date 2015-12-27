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
#include "Charset.h"
#include "Graphics\Textlabel.h"
#include "Character\Look\Clothing.h"
#include "Character\Look\Weapon.h"
#include "Character\Inventory\Equip.h"

namespace IO
{
	using std::vector;
	using Util::vector2d;
	using Character::Equip;
	using Graphics::Textlabel;

	class EquipTooltip
	{
	public:
		EquipTooltip();
		~EquipTooltip();

		void setequip(const Equip* equip, int16_t invpos);
		void draw(vector2d<int16_t> position) const;
		void clear();

	private:
		int16_t invpos;
		int16_t filllength;
		bool hasslots;
		bool isweapon;
		map<Character::Maplestat, string> reqstatstrings;
		Texture itemicon;

		Textlabel name;
		Textlabel potflag;
		Textlabel flag;
		Textlabel category;
		Textlabel wepspeed;
		Textlabel slots;
		Textlabel hammers;
		map<Character::Equipstat, Textlabel> statlabels;

		Texture top;
		Texture mid;
		Texture line;
		Texture bot;
		Texture base;

		map<Equip::Potential, Texture> potential;
		Equip::Potential prank;

		Texture cover;
		Texture shade;

		vector<Character::Maplestat> requirements;
		map<Character::Maplestat, map<bool, Texture>> reqstattextures;
		map<Character::Maplestat, bool> canequip;
		map<Character::Maplestat, vector2d<int16_t>> reqstatpositions;
		map<bool, Charset> reqset;

		Texture jobsback;
		map<bool, map<uint8_t, Texture>> jobs;
		vector<uint8_t> okjobs;
	};
}