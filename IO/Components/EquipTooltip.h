//////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015-2016 Daniel Allendorf                                   //
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
#include "Tooltip.h"

#include "Charset.h"

#include "../../Character/Maplestat.h"
#include "../../Character/Inventory/Equip.h"
#include "../../Character/Inventory/Weapon.h"
#include "../../Graphics/Text.h"
#include "../../Template/BoolPair.h"
#include "../../Template/EnumMap.h"

namespace jrc
{
	class EquipTooltip : public Tooltip
	{
	public:
		EquipTooltip();

		void set_equip(Parent parent, int16_t invpos);
		void draw(Point<int16_t> position) const override;

	private:
		int16_t invpos;
		int16_t height;
		bool hasdesc;
		bool hasslots;
		bool is_weapon;
		EnumMap<Maplestat::Id, std::string> reqstatstrings;
		Texture itemicon;

		Text name;
		Text desc;
		Text potflag;
		Text flag;
		Text category;
		Text wepspeed;
		Text slots;
		Text hammers;
		EnumMap<Equipstat::Id, Text> statlabels;

		Texture top;
		Texture mid;
		Texture line;
		Texture bot;
		Texture base;

		EnumMap<Equip::Potential, Texture> potential;
		Equip::Potential prank;

		Texture cover;
		Texture shade;

		std::vector<Maplestat::Id> requirements;
		EnumMap<Maplestat::Id, BoolPair<Texture>> reqstattextures;
		EnumMap<Maplestat::Id, bool> canequip;
		EnumMap<Maplestat::Id, Point<int16_t>> reqstatpositions;
		BoolPair<Charset> reqset;

		Texture jobsback;
		BoolPair<std::map<uint8_t, Texture>> jobs;
		std::vector<uint8_t> okjobs;
	};
}