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
#include "EquipTooltip.h"

#include "../../Gameplay/Stage.h"
#include "../../Data/EquipData.h"
#include "../../Data/WeaponData.h"

#include "nlnx/nx.hpp"
#include "nlnx/node.hpp"

namespace jrc
{
	EquipTooltip::EquipTooltip() 
	{
		nl::node itemtt = nl::nx::ui["UIToolTip.img"]["Item"];

		top = itemtt["Frame"]["top"];
		mid = itemtt["Frame"]["line"];
		line = itemtt["Frame"]["dotline"];
		bot = itemtt["Frame"]["bottom"];
		base = itemtt["ItemIcon"]["base"];
		cover = itemtt["ItemIcon"]["cover"];
		shade = itemtt["ItemIcon"]["shade"];

		potential[Equip::POT_NONE] = {};
		potential[Equip::POT_HIDDEN] = itemtt["ItemIcon"]["0"];
		potential[Equip::POT_RARE] = itemtt["ItemIcon"]["1"];
		potential[Equip::POT_EPIC] = itemtt["ItemIcon"]["2"];
		potential[Equip::POT_UNIQUE] = itemtt["ItemIcon"]["3"];
		potential[Equip::POT_LEGENDARY] = itemtt["ItemIcon"]["4"];

		requirements.push_back(Maplestat::LEVEL);
		requirements.push_back(Maplestat::STR);
		requirements.push_back(Maplestat::DEX);
		requirements.push_back(Maplestat::INT);
		requirements.push_back(Maplestat::LUK);

		reqstattextures[Maplestat::LEVEL][false] = itemtt["Equip"]["Cannot"]["reqLEV"];
		reqstattextures[Maplestat::LEVEL][true] = itemtt["Equip"]["Can"]["reqLEV"];
		reqstattextures[Maplestat::FAME][false] = itemtt["Equip"]["Cannot"]["reqPOP"];
		reqstattextures[Maplestat::FAME][true] = itemtt["Equip"]["Can"]["reqPOP"];
		reqstattextures[Maplestat::STR][false] = itemtt["Equip"]["Cannot"]["reqSTR"];
		reqstattextures[Maplestat::STR][true] = itemtt["Equip"]["Can"]["reqSTR"];
		reqstattextures[Maplestat::DEX][false] = itemtt["Equip"]["Cannot"]["reqDEX"];
		reqstattextures[Maplestat::DEX][true] = itemtt["Equip"]["Can"]["reqDEX"];
		reqstattextures[Maplestat::INT][false] = itemtt["Equip"]["Cannot"]["reqINT"];
		reqstattextures[Maplestat::INT][true] = itemtt["Equip"]["Can"]["reqINT"];
		reqstattextures[Maplestat::LUK][false] = itemtt["Equip"]["Cannot"]["reqLUK"];
		reqstattextures[Maplestat::LUK][true] = itemtt["Equip"]["Can"]["reqLUK"];

		reqstatpositions[Maplestat::LEVEL] = { 98, 48 };
		reqstatpositions[Maplestat::STR] = { 98, 64 };
		reqstatpositions[Maplestat::DEX] = { 98, 72 };
		reqstatpositions[Maplestat::INT] = { 173, 64 };
		reqstatpositions[Maplestat::LUK] = { 173, 72 };

		reqset[false] = { itemtt["Equip"]["Cannot"], Charset::LEFT };
		reqset[true] = { itemtt["Equip"]["Can"], Charset::LEFT };

		jobsback = itemtt["Equip"]["Job"]["normal"];
		jobs[false][0] = itemtt["Equip"]["Job"]["disable"]["0"];
		jobs[false][1] = itemtt["Equip"]["Job"]["disable"]["1"];
		jobs[false][2] = itemtt["Equip"]["Job"]["disable"]["2"];
		jobs[false][3] = itemtt["Equip"]["Job"]["disable"]["3"];
		jobs[false][4] = itemtt["Equip"]["Job"]["disable"]["4"];
		jobs[false][5] = itemtt["Equip"]["Job"]["disable"]["5"];
		jobs[true][0] = itemtt["Equip"]["Job"]["enable"]["0"];
		jobs[true][1] = itemtt["Equip"]["Job"]["enable"]["1"];
		jobs[true][2] = itemtt["Equip"]["Job"]["enable"]["2"];
		jobs[true][3] = itemtt["Equip"]["Job"]["enable"]["3"];
		jobs[true][4] = itemtt["Equip"]["Job"]["enable"]["4"];
		jobs[true][5] = itemtt["Equip"]["Job"]["enable"]["5"];

		invpos = 0;
	}

	void EquipTooltip::set_equip(Parent parent, int16_t ivp)
	{
		if (ivp == invpos)
			return;

		invpos = ivp;

		const Player& player = Stage::get().get_player();

		InventoryType::Id invtype;
		switch (parent)
		{
		case ITEMINVENTORY:
		case SHOP:
			invtype = InventoryType::EQUIP;
			break;
		case EQUIPINVENTORY:
			invtype = InventoryType::EQUIPPED;
			break;
		default:
			invtype = InventoryType::NONE;
		}

		auto oequip = player.get_inventory().get_equip(invtype, ivp);
		if (!oequip)
			return;

		const Equip& equip = *oequip;

		int32_t item_id = equip.get_item_id();

		const EquipData& equipdata = EquipData::get(item_id);
		const ItemData& itemdata = equipdata.get_itemdata();
		const CharStats& stats = player.get_stats();
		
		height = 500;

		itemicon = itemdata.get_icon(true);

		for (auto& ms : requirements)
		{
			canequip[ms] = stats.get_stat(ms) >= equipdata.get_reqstat(ms);
			std::string reqstr = std::to_string(equipdata.get_reqstat(ms));
			reqstr.insert(0, 3 - reqstr.size(), '0');
			reqstatstrings[ms] = reqstr;
		}

		okjobs.clear();
		switch (equipdata.get_reqstat(Maplestat::JOB))
		{
		case 0:
			okjobs.push_back(0);
			okjobs.push_back(1);
			okjobs.push_back(2);
			okjobs.push_back(3);
			okjobs.push_back(4);
			okjobs.push_back(5);
			canequip[Maplestat::JOB] = true;
			break;
		case 1:
			okjobs.push_back(1);
			canequip[Maplestat::JOB] = (stats.get_stat(Maplestat::JOB) / 100 == 1) || (stats.get_stat(Maplestat::JOB) / 100 >= 20);
			break;
		case 2:
			okjobs.push_back(2);
			canequip[Maplestat::JOB] = stats.get_stat(Maplestat::JOB) / 100 == 2;
			break;
		case 4:
			okjobs.push_back(3);
			canequip[Maplestat::JOB] = stats.get_stat(Maplestat::JOB) / 100 == 3;
			break;
		case 8:
			okjobs.push_back(4);
			canequip[Maplestat::JOB] = stats.get_stat(Maplestat::JOB) / 100 == 4;
			break;
		case 16:
			okjobs.push_back(5);
			canequip[Maplestat::JOB] = stats.get_stat(Maplestat::JOB) / 100 == 5;
			break;
		default:
			canequip[Maplestat::JOB] = false;
		}

		prank = equip.get_potrank();
		switch (prank)
		{
		case Equip::POT_HIDDEN:
			potflag = Text(Text::A11M, Text::CENTER, Text::RED);
			potflag.change_text("(Hidden Potential)");
			break;
		case Equip::POT_RARE:
			potflag = Text(Text::A11M, Text::CENTER, Text::WHITE);
			potflag.change_text("(Rare Item)");
			break;
		case Equip::POT_EPIC:
			potflag = Text(Text::A11M, Text::CENTER, Text::WHITE);
			potflag.change_text("(Epic Item)");
			break;
		case Equip::POT_UNIQUE:
			potflag = Text(Text::A11M, Text::CENTER, Text::WHITE);
			potflag.change_text("(Unique Item)");
			break;
		case Equip::POT_LEGENDARY:
			potflag = Text(Text::A11M, Text::CENTER, Text::WHITE);
			potflag.change_text("(Legendary Item)");
			break;
		default:
			height -= 16;
		}

		Text::Color namecolor;
		switch (equip.get_quality())
		{
		case EquipQuality::GREY:
			namecolor = Text::LIGHTGREY;
			break;
		case EquipQuality::ORANGE:
			namecolor = Text::ORANGE;
			break;
		case EquipQuality::BLUE:
			namecolor = Text::MEDIUMBLUE;
			break;
		case EquipQuality::VIOLET:
			namecolor = Text::VIOLET;
			break;
		case EquipQuality::GOLD:
			namecolor = Text::YELLOW;
			break;
		default:
			namecolor = Text::WHITE;
		}

		std::string namestr = itemdata.get_name();
		if (equip.get_level() > 0)
		{
			namestr.append(" (+");
			namestr.append(std::to_string(equip.get_level()));
			namestr.append(")");
		}
		name = { Text::A12B, Text::CENTER, namecolor, namestr, 400 };

		std::string desctext = itemdata.get_desc();
		hasdesc = desctext.size() > 0;
		if (hasdesc)
		{
			desc = { Text::A12M, Text::LEFT, Text::WHITE, desctext, 250 };
			height += desc.height() + 10;
		}

		category = { Text::A11M, Text::LEFT, Text::WHITE, "CATEGORY: " + equipdata.get_type() };

		is_weapon = equipdata.is_weapon();
		if (is_weapon)
		{
			const WeaponData& weapon = WeaponData::get(item_id);
			wepspeed = { Text::A11M, Text::LEFT, Text::WHITE, "ATTACK SPEED: " + weapon.getspeedstring() };
		}
		else
		{
			height -= 18;
		}

		hasslots = (equip.get_slots() > 0) || (equip.get_level() > 0);
		if (hasslots)
		{
			slots = { Text::A11M, Text::LEFT, Text::WHITE, "UPGRADES AVAILABLE: " + std::to_string(equip.get_slots()) };

			std::string vicious = std::to_string(equip.get_vicious());
			if (equip.get_vicious() > 1)
				vicious.append(" (MAX) ");
			hammers = { Text::A11M, Text::LEFT, Text::WHITE, "VICIOUS HAMMERS USED: " + vicious };
		}
		else
		{
			height -= 36;
		}

		statlabels.clear();
		for (Equipstat::Id es = Equipstat::STR; es <= Equipstat::JUMP; es = static_cast<Equipstat::Id>(es + 1))
		{
			if (equip.get_stat(es) > 0)
			{
				int16_t delta = equip.get_stat(es) - equipdata.get_defstat(es);
				std::string statstr = std::to_string(equip.get_stat(es));
				if (delta != 0)
				{
					statstr.append(" (");
					statstr.append((delta < 0) ? "-" : "+");
					statstr.append(std::to_string(abs(delta)) + ")");
				}
				statlabels[es] = { Text::A11M, Text::LEFT, Text::WHITE, Equipstat::names[es] + std::string(": ") + statstr };
			}
			else
			{
				height -= 18;
			}
		}
	}

	void EquipTooltip::draw(Point<int16_t> pos) const
	{
		if (invpos == 0)
			return;

		top.draw({ pos });
		mid.draw({ pos + Point<int16_t>(0, 13), Point<int16_t>(0, height) });
		bot.draw({ pos + Point<int16_t>(0, height + 13) });

		name.draw(pos + Point<int16_t>(130, 3));
		if (prank != Equip::POT_NONE)
		{
			potflag.draw(pos + Point<int16_t>(130, 20));
			pos.shift_y(16);
		}
		pos.shift_y(26);

		line.draw({ pos });

		base.draw(pos + Point<int16_t>(10, 10));
		shade.draw(pos + Point<int16_t>(10, 10));
		itemicon.draw({ pos + Point<int16_t>(20, 82), 2.0f, 2.0f });
		potential[prank].draw(pos + Point<int16_t>(10, 10));
		cover.draw(pos + Point<int16_t>(10, 10));

		pos.shift_y(12);

		for (Maplestat::Id ms : requirements)
		{
			Point<int16_t> reqpos = reqstatpositions[ms];
			bool reqok = canequip[ms];
			reqstattextures[ms][reqok].draw({ pos + reqpos });
			reqset[reqok].draw(reqstatstrings[ms], 6, { pos + reqpos + Point<int16_t>(54, 0) });
		}

		pos.shift_y(88);

		Point<int16_t> job_position(pos + Point<int16_t>(8, 0));
		jobsback.draw(job_position);
		for (auto& jbit : okjobs)
		{
			jobs[canequip[Maplestat::JOB]]
				.at(jbit)
				.draw(job_position);
		}

		line.draw({ pos + Point<int16_t>(0, 30) });

		pos.shift_y(32);

		category.draw(pos + Point<int16_t>(10, 0));

		pos.shift_y(18);

		if (is_weapon)
		{
			wepspeed.draw(pos + Point<int16_t>(10, 0));
			pos.shift_y(18);
		}

		for (const Text& label : statlabels.values())
		{
			if (label.empty())
				continue;

			label.draw(pos + Point<int16_t>(10, 0));
			pos.shift_y(18);
		}

		if (hasslots)
		{
			slots.draw(pos + Point<int16_t>(10, 0));
			pos.shift_y(18);
			hammers.draw(pos + Point<int16_t>(10, 0));
			pos.shift_y(18);
		}

		if (hasdesc)
		{
			line.draw({ pos + Point<int16_t>(0, 5) });
			desc.draw({ pos + Point<int16_t>(10, 6) });
		}
	}
}