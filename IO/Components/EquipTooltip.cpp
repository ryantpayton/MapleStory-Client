/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015 Daniel Allendorf                                               //
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
#include "Gameplay\Stage.h"
#include "nlnx\nx.hpp"
#include "nlnx\node.hpp"

namespace IO
{
	using namespace Character;

	EquipTooltip::EquipTooltip() 
	{
		node itemtt = nl::nx::ui["UIToolTip.img"]["Item"];

		top = Texture(itemtt["Frame"]["top"]);
		mid = Texture(itemtt["Frame"]["line"]);
		line = Texture(itemtt["Frame"]["dotline"]);
		bot = Texture(itemtt["Frame"]["bottom"]);
		base = Texture(itemtt["ItemIcon"]["base"]);
		cover = Texture(itemtt["ItemIcon"]["cover"]);
		shade = Texture(itemtt["ItemIcon"]["shade"]);

		potential[Equip::POT_NONE] = Texture();
		potential[Equip::POT_HIDDEN] = Texture(itemtt["ItemIcon"]["0"]);
		potential[Equip::POT_RARE] = Texture(itemtt["ItemIcon"]["1"]);
		potential[Equip::POT_EPIC] = Texture(itemtt["ItemIcon"]["2"]);
		potential[Equip::POT_UNIQUE] = Texture(itemtt["ItemIcon"]["3"]);
		potential[Equip::POT_LEGENDARY] = Texture(itemtt["ItemIcon"]["4"]);

		requirements.push_back(Maplestat::LEVEL);
		requirements.push_back(Maplestat::STR);
		requirements.push_back(Maplestat::DEX);
		requirements.push_back(Maplestat::INT);
		requirements.push_back(Maplestat::LUK);

		reqstattextures[Maplestat::LEVEL][false] = Texture(itemtt["Equip"]["Cannot"]["reqLEV"]);
		reqstattextures[Maplestat::LEVEL][true] = Texture(itemtt["Equip"]["Can"]["reqLEV"]);
		reqstattextures[Maplestat::FAME][false] = Texture(itemtt["Equip"]["Cannot"]["reqPOP"]);
		reqstattextures[Maplestat::FAME][true] = Texture(itemtt["Equip"]["Can"]["reqPOP"]);
		reqstattextures[Maplestat::STR][false] = Texture(itemtt["Equip"]["Cannot"]["reqSTR"]);
		reqstattextures[Maplestat::STR][true] = Texture(itemtt["Equip"]["Can"]["reqSTR"]);
		reqstattextures[Maplestat::DEX][false] = Texture(itemtt["Equip"]["Cannot"]["reqDEX"]);
		reqstattextures[Maplestat::DEX][true] = Texture(itemtt["Equip"]["Can"]["reqDEX"]);
		reqstattextures[Maplestat::INT][false] = Texture(itemtt["Equip"]["Cannot"]["reqINT"]);
		reqstattextures[Maplestat::INT][true] = Texture(itemtt["Equip"]["Can"]["reqINT"]);
		reqstattextures[Maplestat::LUK][false] = Texture(itemtt["Equip"]["Cannot"]["reqLUK"]);
		reqstattextures[Maplestat::LUK][true] = Texture(itemtt["Equip"]["Can"]["reqLUK"]);

		reqstatpositions[Maplestat::LEVEL] = Point<int16_t>(98, 48);
		reqstatpositions[Maplestat::STR] = Point<int16_t>(98, 64);
		reqstatpositions[Maplestat::DEX] = Point<int16_t>(98, 72);
		reqstatpositions[Maplestat::INT] = Point<int16_t>(173, 64);
		reqstatpositions[Maplestat::LUK] = Point<int16_t>(173, 72);

		reqset[false] = Charset(itemtt["Equip"]["Cannot"], Charset::LEFT);
		reqset[true] = Charset(itemtt["Equip"]["Can"], Charset::LEFT);

		jobsback = Texture(itemtt["Equip"]["Job"]["normal"]);
		jobs[false][0] = Texture(itemtt["Equip"]["Job"]["disable"]["0"]);
		jobs[false][1] = Texture(itemtt["Equip"]["Job"]["disable"]["1"]);
		jobs[false][2] = Texture(itemtt["Equip"]["Job"]["disable"]["2"]);
		jobs[false][3] = Texture(itemtt["Equip"]["Job"]["disable"]["3"]);
		jobs[false][4] = Texture(itemtt["Equip"]["Job"]["disable"]["4"]);
		jobs[false][5] = Texture(itemtt["Equip"]["Job"]["disable"]["5"]);
		jobs[true][0] = Texture(itemtt["Equip"]["Job"]["enable"]["0"]);
		jobs[true][1] = Texture(itemtt["Equip"]["Job"]["enable"]["1"]);
		jobs[true][2] = Texture(itemtt["Equip"]["Job"]["enable"]["2"]);
		jobs[true][3] = Texture(itemtt["Equip"]["Job"]["enable"]["3"]);
		jobs[true][4] = Texture(itemtt["Equip"]["Job"]["enable"]["4"]);
		jobs[true][5] = Texture(itemtt["Equip"]["Job"]["enable"]["5"]);

		invpos = 0;
	}

	EquipTooltip::~EquipTooltip() {}

	void EquipTooltip::clear()
	{
		invpos = 0;
	}

	void EquipTooltip::setequip(const Equip* equip, int16_t ivp)
	{
		if (ivp == invpos)
			return;

		invpos = ivp;

		if (equip == nullptr)
			return;

		const Clothing& cloth = equip->getcloth();

		using Gameplay::Stage;
		const CharStats& stats = Stage::get().getplayer().getstats();
		
		filllength = 500;

		itemicon = cloth.geticon(true);

		for (auto& ms : requirements)
		{
			canequip[ms] = stats.getstat(ms) >= cloth.getreqstat(ms);
			string reqstr = std::to_string(cloth.getreqstat(ms));
			reqstr.insert(0, 3 - reqstr.size(), '0');
			reqstatstrings[ms] = reqstr;
		}

		okjobs.clear();
		switch (cloth.getreqstat(Maplestat::JOB))
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
			canequip[Maplestat::JOB] = (stats.getstat(Maplestat::JOB) / 100 == 1) || (stats.getstat(Maplestat::JOB) / 100 >= 20);
			break;
		case 2:
			okjobs.push_back(2);
			canequip[Maplestat::JOB] = stats.getstat(Maplestat::JOB) / 100 == 2;
			break;
		case 4:
			okjobs.push_back(3);
			canequip[Maplestat::JOB] = stats.getstat(Maplestat::JOB) / 100 == 3;
			break;
		case 8:
			okjobs.push_back(4);
			canequip[Maplestat::JOB] = stats.getstat(Maplestat::JOB) / 100 == 4;
			break;
		case 16:
			okjobs.push_back(5);
			canequip[Maplestat::JOB] = stats.getstat(Maplestat::JOB) / 100 == 5;
			break;
		default:
			canequip[Maplestat::JOB] = false;
		}

		prank = equip->getpotrank();
		switch (prank)
		{
		case Equip::POT_HIDDEN:
			potflag = Text(Text::A11M, Text::CENTER, Text::RED);
			potflag.settext("(Hidden Potential)");
			break;
		case Equip::POT_RARE:
			potflag = Text(Text::A11M, Text::CENTER, Text::WHITE);
			potflag.settext("(Rare Item)");
			break;
		case Equip::POT_EPIC:
			potflag = Text(Text::A11M, Text::CENTER, Text::WHITE);
			potflag.settext("(Epic Item)");
			break;
		case Equip::POT_UNIQUE:
			potflag = Text(Text::A11M, Text::CENTER, Text::WHITE);
			potflag.settext("(Unique Item)");
			break;
		case Equip::POT_LEGENDARY:
			potflag = Text(Text::A11M, Text::CENTER, Text::WHITE);
			potflag.settext("(Legendary Item)");
			break;
		default:
			filllength -= 16;
		}

		Text::Color namecolor;
		switch (equip->getquality())
		{
		case Equip::EQQ_GREY:
			namecolor = Text::LIGHTGREY;
			break;
		case Equip::EQQ_ORANGE:
			namecolor = Text::ORANGE;
			break;
		case Equip::EQQ_BLUE:
			namecolor = Text::MEDIUMBLUE;
			break;
		case Equip::EQQ_VIOLET:
			namecolor = Text::VIOLET;
			break;
		case Equip::EQQ_GOLD:
			namecolor = Text::YELLOW;
			break;
		default:
			namecolor = Text::WHITE;
		}

		string namestr = cloth.getname();
		if (equip->getlevel() > 0)
		{
			namestr.append(" (+");
			namestr.append(std::to_string(equip->getlevel()));
			namestr.append(")");
		}
		name = Text(Text::A12B, Text::CENTER, namecolor);
		name.settext(namestr, 400);

		desc = Itemtext(cloth.getdesc(), 150);
		int16_t descdelta = desc.getheight() - 80;
		if (descdelta > 0)
			filllength += descdelta;

		category = Text(Text::A11L, Text::LEFT, Text::WHITE);
		category.settext("CATEGORY: " + cloth.gettype());

		isweapon = cloth.isweapon();
		if (isweapon)
		{
			const Weapon& weapon = reinterpret_cast<const Weapon&>(cloth);
			wepspeed = Text(Text::A11L, Text::LEFT, Text::WHITE);
			wepspeed.settext("ATTACK SPEED: " + weapon.getspeedstring());
		}
		else
		{
			filllength -= 18;
		}

		hasslots = (equip->getslots() > 0) || (equip->getlevel() > 0);
		if (hasslots)
		{
			slots = Text(Text::A11L, Text::LEFT, Text::WHITE);
			slots.settext("UPGRADES AVAILABLE: " + std::to_string(equip->getslots()));

			string vicious = std::to_string(equip->getvicious());
			if (equip->getvicious() > 1)
				vicious.append(" (MAX) ");
			hammers = Text(Text::A11L, Text::LEFT, Text::WHITE);
			hammers.settext("VICIOUS HAMMERS USED: " + vicious);
		}
		else
		{
			filllength -= 36;
		}

		statlabels.clear();
		for (Equipstat::Value es = Equipstat::STR; es <= Equipstat::JUMP; es = static_cast<Equipstat::Value>(es + 1))
		{
			if (equip->getstat(es) > 0)
			{
				int16_t delta = equip->getstat(es) - cloth.getdefstat(es);
				string statstr = std::to_string(equip->getstat(es));
				if (delta != 0)
				{
					statstr.append(" (");
					statstr.append((delta < 0) ? "-" : "+");
					statstr.append(std::to_string(abs(delta)) + ")");
				}
				statlabels[es] = Text(Text::A11L, Text::LEFT, Text::WHITE);
				statlabels[es].settext(Equipstat::nameof(es) + ": " + statstr);
			}
			else
			{
				filllength -= 18;
			}
		}
	}

	void EquipTooltip::draw(Point<int16_t> pos) const
	{
		if (invpos == 0)
			return;

		using Graphics::DrawArgument;
		top.draw(DrawArgument(pos));
		mid.draw(DrawArgument(pos + Point<int16_t>(0, 13), Point<int16_t>(0, filllength)));
		bot.draw(DrawArgument(pos + Point<int16_t>(0, filllength + 13)));

		name.draw(pos + Point<int16_t>(130, 3));
		if (prank != Equip::POT_NONE)
		{
			potflag.draw(pos + Point<int16_t>(130, 20));
			pos.shifty(16);
		}
		pos.shifty(26);

		line.draw(DrawArgument(pos));

		base.draw(pos + Point<int16_t>(10, 10));
		shade.draw(pos + Point<int16_t>(10, 10));
		itemicon.draw(
			DrawArgument(pos + Point<int16_t>(20, 82), 2.0f, 2.0f)
			);
		potential.at(prank).draw(pos + Point<int16_t>(10, 10));
		cover.draw(pos + Point<int16_t>(10, 10));

		pos.shifty(12);

		for (auto& ms : requirements)
		{
			Point<int16_t> reqpos = reqstatpositions.at(ms);
			bool reqok = canequip.at(ms);
			reqstattextures.at(ms).at(reqok).draw(DrawArgument(pos + reqpos));
			reqset.at(reqok).draw(reqstatstrings.at(ms), 6, DrawArgument(pos + reqpos + Point<int16_t>(54, 0)));
		}

		pos.shifty(88);

		DrawArgument jobargs = DrawArgument(pos + Point<int16_t>(8, 0));
		jobsback.draw(jobargs);
		for (auto& jbit : okjobs)
		{
			jobs.at(canequip.at(Maplestat::JOB)).at(jbit).draw(jobargs);
		}

		line.draw(DrawArgument(pos + Point<int16_t>(0, 30)));

		pos.shifty(32);

		category.draw(pos + Point<int16_t>(10, 0));

		pos.shifty(18);

		if (isweapon)
		{
			wepspeed.draw(pos + Point<int16_t>(10, 0));
			pos.shifty(18);
		}

		for (auto& stit : statlabels)
		{
			stit.second.draw(pos + Point<int16_t>(10, 0));
			pos.shifty(18);
		}

		if (hasslots)
		{
			slots.draw(pos + Point<int16_t>(10, 0));
			pos.shifty(18);
			hammers.draw(pos + Point<int16_t>(10, 0));
			pos.shifty(18);
		}

		if (hasdesc)
		{
			line.draw(pos + Point<int16_t>(0, 5));
			desc.draw(pos + Point<int16_t>(10, 6));
		}
	}
}