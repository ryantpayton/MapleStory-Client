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

		requirements.push_back(MS_LEVEL);
		requirements.push_back(MS_STR);
		requirements.push_back(MS_DEX);
		requirements.push_back(MS_INT);
		requirements.push_back(MS_LUK);

		reqstattextures[MS_LEVEL][false] = Texture(itemtt["Equip"]["Cannot"]["reqLEV"]);
		reqstattextures[MS_LEVEL][true] = Texture(itemtt["Equip"]["Can"]["reqLEV"]);
		reqstattextures[MS_FAME][false] = Texture(itemtt["Equip"]["Cannot"]["reqPOP"]);
		reqstattextures[MS_FAME][true] = Texture(itemtt["Equip"]["Can"]["reqPOP"]);
		reqstattextures[MS_STR][false] = Texture(itemtt["Equip"]["Cannot"]["reqSTR"]);
		reqstattextures[MS_STR][true] = Texture(itemtt["Equip"]["Can"]["reqSTR"]);
		reqstattextures[MS_DEX][false] = Texture(itemtt["Equip"]["Cannot"]["reqDEX"]);
		reqstattextures[MS_DEX][true] = Texture(itemtt["Equip"]["Can"]["reqDEX"]);
		reqstattextures[MS_INT][false] = Texture(itemtt["Equip"]["Cannot"]["reqINT"]);
		reqstattextures[MS_INT][true] = Texture(itemtt["Equip"]["Can"]["reqINT"]);
		reqstattextures[MS_LUK][false] = Texture(itemtt["Equip"]["Cannot"]["reqLUK"]);
		reqstattextures[MS_LUK][true] = Texture(itemtt["Equip"]["Can"]["reqLUK"]);

		reqstatpositions[MS_LEVEL] = vector2d<int16_t>(98, 48);
		reqstatpositions[MS_STR] = vector2d<int16_t>(98, 64);
		reqstatpositions[MS_DEX] = vector2d<int16_t>(98, 72);
		reqstatpositions[MS_INT] = vector2d<int16_t>(173, 64);
		reqstatpositions[MS_LUK] = vector2d<int16_t>(173, 72);

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

	void EquipTooltip::setequip(const Equip* equip, int16_t ivp)
	{
		if (ivp == invpos)
			return;

		invpos = ivp;

		if (equip == nullptr)
			return;

		const Clothing& cloth = equip->getcloth();
		const CharStats& stats = Gameplay::Stage::getplayer().getstats();
		
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
		switch (cloth.getreqstat(MS_JOB))
		{
		case 0:
			okjobs.push_back(0);
			okjobs.push_back(1);
			okjobs.push_back(2);
			okjobs.push_back(3);
			okjobs.push_back(4);
			okjobs.push_back(5);
			canequip[MS_JOB] = true;
			break;
		case 1:
			okjobs.push_back(1);
			canequip[MS_JOB] = (stats.getstat(MS_JOB) / 100 == 1) || (stats.getstat(MS_JOB) / 100 >= 20);
			break;
		case 2:
			okjobs.push_back(2);
			canequip[MS_JOB] = stats.getstat(MS_JOB) / 100 == 2;
			break;
		case 4:
			okjobs.push_back(3);
			canequip[MS_JOB] = stats.getstat(MS_JOB) / 100 == 3;
			break;
		case 8:
			okjobs.push_back(4);
			canequip[MS_JOB] = stats.getstat(MS_JOB) / 100 == 4;
			break;
		case 16:
			okjobs.push_back(5);
			canequip[MS_JOB] = stats.getstat(MS_JOB) / 100 == 5;
			break;
		default:
			canequip[MS_JOB] = false;
		}

		prank = equip->getpotrank();
		switch (prank)
		{
		case Equip::POT_HIDDEN:
			potflag = Textlabel(Textlabel::DWF_12MC, Textlabel::TXC_RED, "(Hidden Potential)", 0);
			break;
		case Equip::POT_RARE:
			potflag = Textlabel(Textlabel::DWF_12MC, Textlabel::TXC_WHITE, "(Rare Item)", 0);
			break;
		case Equip::POT_EPIC:
			potflag = Textlabel(Textlabel::DWF_12MC, Textlabel::TXC_WHITE, "(Epic Item)", 0);
			break;
		case Equip::POT_UNIQUE:
			potflag = Textlabel(Textlabel::DWF_12MC, Textlabel::TXC_WHITE, "(Unique Item)", 0);
			break;
		case Equip::POT_LEGENDARY:
			potflag = Textlabel(Textlabel::DWF_12MC, Textlabel::TXC_WHITE, "(Legendary Item)", 0);
			break;
		default:
			filllength -= 16;
		}

		Textlabel::Textcolor namecolor;
		switch (equip->getquality())
		{
		case Equip::EQQ_GREY:
			namecolor = Textlabel::TXC_GREY;
			break;
		case Equip::EQQ_ORANGE:
			namecolor = Textlabel::TXC_ORANGE;
			break;
		case Equip::EQQ_BLUE:
			namecolor = Textlabel::TXC_MBLUE;
			break;
		case Equip::EQQ_VIOLET:
			namecolor = Textlabel::TXC_VIOLET;
			break;
		case Equip::EQQ_GOLD:
			namecolor = Textlabel::TXC_YELLOW;
			break;
		default:
			namecolor = Textlabel::TXC_WHITE;
		}

		string namestr = cloth.getname();
		if (equip->getlevel() > 0)
		{
			namestr.append(" (+");
			namestr.append(std::to_string(equip->getlevel()));
			namestr.append(")");
		}
		name = Textlabel(Textlabel::DWF_14MC, namecolor, namestr, 0);

		/*itemtext* desc = cloth.getdtext();
		if (desc)
		{
			filllength += 9 + desc->getheight();
		}*/

		isweapon = cloth.isweapon();
		category = Textlabel(Textlabel::DWF_12LL, Textlabel::TXC_WHITE, "CATEGORY: " + cloth.gettype(), 0);
		if (isweapon)
		{
			const Weapon& weapon = reinterpret_cast<const Weapon&>(cloth);
			wepspeed = Textlabel(Textlabel::DWF_12LL, Textlabel::TXC_WHITE, "ATTACK SPEED: " + weapon.getspeedstring(), 0);
		}
		else
		{
			filllength -= 18;
		}

		hasslots = (equip->getslots() > 0) || (equip->getlevel() > 0);
		if (hasslots)
		{
			slots = Textlabel(Textlabel::DWF_12LL, Textlabel::TXC_WHITE, "UPGRADES AVAILABLE: " + std::to_string(equip->getslots()), 0);

			string vicious = std::to_string(equip->getvicious());
			if (equip->getvicious() > 1)
				vicious.append(" (MAX) ");
			hammers = Textlabel(Textlabel::DWF_12LL, Textlabel::TXC_WHITE, "VICIOUS HAMMERS USED: " + vicious, 0);
		}
		else
		{
			filllength -= 36;
		}

		statlabels.clear();
		for (Equipstat es = ES_STR; es <= ES_JUMP; es = static_cast<Equipstat>(es + 1))
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
				statlabels[es] = Textlabel(Textlabel::DWF_12LL, Textlabel::TXC_WHITE, cloth.getstatstr(es) + ": " + statstr, 0);
			}
			else
			{
				filllength -= 18;
			}
		}
	}

	void EquipTooltip::draw(vector2d<int16_t> pos) const
	{
		if (invpos == 0)
			return;

		using Graphics::DrawArgument;
		top.draw(DrawArgument(pos));
		mid.draw(DrawArgument(pos + vector2d<int16_t>(0, 13), vector2d<int16_t>(0, filllength)));
		bot.draw(DrawArgument(pos + vector2d<int16_t>(0, filllength + 13)));

		name.draw(pos + vector2d<int16_t>(130, 3));
		if (prank != Equip::POT_NONE)
		{
			potflag.draw(pos + vector2d<int16_t>(130, 20));
			pos = pos + vector2d<int16_t>(0, 16);
		}
		pos = pos + vector2d<int16_t>(0, 28);

		line.draw(DrawArgument(pos));

		DrawArgument iconargs = DrawArgument(pos + vector2d<int16_t>(10, 10));
		base.draw(iconargs);
		shade.draw(iconargs);
		itemicon.draw(
			DrawArgument(pos + vector2d<int16_t>(20, 82), 2.0f, 2.0f)
			);
		potential.at(prank).draw(iconargs);
		cover.draw(iconargs);

		pos = pos + vector2d<int16_t>(0, 12);

		for (auto& ms : requirements)
		{
			vector2d<int16_t> reqpos = reqstatpositions.at(ms);
			bool reqok = canequip.at(ms);
			reqstattextures.at(ms).at(reqok).draw(DrawArgument(pos + reqpos));
			reqset.at(reqok).draw(reqstatstrings.at(ms), 6, DrawArgument(pos + reqpos + vector2d<int16_t>(54, 0)));
		}

		pos = pos + vector2d<int16_t>(0, 88);

		DrawArgument jobargs = DrawArgument(pos + vector2d<int16_t>(8, 0));
		jobsback.draw(jobargs);
		for (auto& jbit : okjobs)
		{
			jobs.at(canequip.at(MS_JOB)).at(jbit).draw(jobargs);
		}

		line.draw(DrawArgument(pos + vector2d<int16_t>(0, 30)));

		pos = pos + vector2d<int16_t>(0, 32);

		category.draw(pos + vector2d<int16_t>(10, 0));

		pos = pos + vector2d<int16_t>(0, 18);

		if (isweapon)
		{
			wepspeed.draw(pos + vector2d<int16_t>(10, 0));
			pos = pos + vector2d<int16_t>(0, 18);
		}

		for (auto& stit : statlabels)
		{
			stit.second.draw(pos + vector2d<int16_t>(10, 0));
			pos = pos + vector2d<int16_t>(0, 18);
		}

		if (hasslots)
		{
			slots.draw(pos + vector2d<int16_t>(10, 0));
			pos = pos + vector2d<int16_t>(0, 18);
			hammers.draw(pos + vector2d<int16_t>(10, 0));
			pos = pos + vector2d<int16_t>(0, 18);
		}

		/*itemtext* desc = cloth.getdtext();
		if (desc)
		{
			line.draw(pos + vector2d<int16_t>(0, 5));
			desc->draw(pos + vector2d<int16_t>(10, 6));
		}*/
	}
}