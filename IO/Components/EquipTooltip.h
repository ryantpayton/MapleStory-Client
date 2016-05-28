//////////////////////////////////////////////////////////////////////////////
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
#include "Tooltip.h"
#include "Charset.h"
#include "Itemtext.h"

#include "..\..\Character\Look\Clothing.h"
#include "..\..\Character\Look\Weapon.h"
#include "..\..\Character\Inventory\Equip.h"

namespace jrc
{
	class EquipTooltip : public Tooltip
	{
	public:
		EquipTooltip();

		void setequip(const Equip* equip, int16_t invpos);
		void draw(Point<int16_t> position) const override;

	private:
		int16_t invpos;
		int16_t filllength;
		bool hasdesc;
		bool hasslots;
		bool isweapon;
		std::map<Maplestat::Value, std::string> reqstatstrings;
		Texture itemicon;

		Text name;
		Itemtext desc;
		Text potflag;
		Text flag;
		Text category;
		Text wepspeed;
		Text slots;
		Text hammers;
		std::map<Equipstat::Value, Text> statlabels;

		Texture top;
		Texture mid;
		Texture line;
		Texture bot;
		Texture base;

		std::map<Equip::Potential, Texture> potential;
		Equip::Potential prank;

		Texture cover;
		Texture shade;

		std::vector<Maplestat::Value> requirements;
		std::map<Maplestat::Value, std::map<bool, Texture>> reqstattextures;
		std::map<Maplestat::Value, bool> canequip;
		std::map<Maplestat::Value, Point<int16_t>> reqstatpositions;
		std::map<bool, Charset> reqset;

		Texture jobsback;
		std::map<bool, std::map<uint8_t, Texture>> jobs;
		std::vector<uint8_t> okjobs;
	};
}