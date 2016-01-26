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
#include "UIStatsinfo.h"
#include "IO\UI.h"
#include "Net\Session.h"
#include "IO\Components\MapleButton.h"
#include "Net\Packets\PlayerPackets.h"
#include "Program\Configuration.h"
#include "nlnx\nx.hpp"

namespace IO
{
	UIStatsinfo::UIStatsinfo(const CharStats& st) :
		UIDragElement("PosSTATS", vector2d<int16_t>(212, 20)),
		stats(st) {

		using nl::node;
		node src = nl::nx::ui["UIWindow4.img"]["Stat"]["main"];
		node detail = nl::nx::ui["UIWindow4.img"]["Stat"]["detail"];

		sprites.push_back(Sprite(src["backgrnd"]));
		sprites.push_back(Sprite(src["backgrnd2"]));
		sprites.push_back(Sprite(src["backgrnd3"]));

		detailtextures.push_back(Texture(detail["backgrnd"]));
		detailtextures.push_back(Texture(detail["backgrnd2"]));
		detailtextures.push_back(Texture(detail["backgrnd3"]));

		abilities["rare"] = Texture(detail["abilityTitle"]["rare"]["0"]);
		abilities["epic"] = Texture(detail["abilityTitle"]["epic"]["0"]);
		abilities["unique"] = Texture(detail["abilityTitle"]["unique"]["0"]);
		abilities["legendary"] = Texture(detail["abilityTitle"]["legendary"]["0"]);
		abilities["none"] = Texture(detail["abilityTitle"]["normal"]["0"]);

		buttons[BT_HP] = unique_ptr<Button>(new MapleButton(src["BtHpUp"]));
		buttons[BT_MP] = unique_ptr<Button>(new MapleButton(src["BtMpUp"]));
		buttons[BT_STR] = unique_ptr<Button>(new MapleButton(src["BtStrUp"]));
		buttons[BT_DEX] = unique_ptr<Button>(new MapleButton(src["BtDexUp"]));
		buttons[BT_LUK] = unique_ptr<Button>(new MapleButton(src["BtLukUp"]));
		buttons[BT_INT] = unique_ptr<Button>(new MapleButton(src["BtIntUp"]));

		buttons[BT_DETAILOPEN] = unique_ptr<Button>(new MapleButton(src["BtDetailOpen"]));
		buttons[BT_DETAILCLOSE] = unique_ptr<Button>(new MapleButton(src["BtDetailClose"]));
		buttons[BT_DETAILCLOSE]->setactive(false);

		updateap();

		for (size_t i = 0; i < NUMLABELS; i++)
		{
			statlabels[i] = Text(Text::A11M, Text::LEFT, Text::DARKGREY);
		}
		statoffsets[NAME] = vector2d<int16_t>(73, 27);
		statoffsets[JOB] = vector2d<int16_t>(73, 45);
		statoffsets[GUILD] = vector2d<int16_t>(73, 63);
		statoffsets[FAME] = vector2d<int16_t>(73, 81);
		statoffsets[DAMAGE] = vector2d<int16_t>(73, 99);
		statoffsets[HP] = vector2d<int16_t>(73, 117);
		statoffsets[MP] = vector2d<int16_t>(73, 135);
		statoffsets[AP] = vector2d<int16_t>(70, 177);
		statoffsets[STR] = vector2d<int16_t>(73, 204);
		statoffsets[DEX] = vector2d<int16_t>(73, 222);
		statoffsets[INT] = vector2d<int16_t>(73, 240);
		statoffsets[LUK] = vector2d<int16_t>(73, 258);
		statoffsets[ATTACK] = vector2d<int16_t>(73, 37);
		statoffsets[CRIT] = vector2d<int16_t>(73, 55);
		statoffsets[MINCRIT] = vector2d<int16_t>(73, 73);
		statoffsets[MAXCRIT] = vector2d<int16_t>(168, 73);
		statoffsets[BDM] = vector2d<int16_t>(73, 91);
		statoffsets[IGNOREDEF] = vector2d<int16_t>(168, 91);
		statoffsets[RESIST] = vector2d<int16_t>(73, 109);
		statoffsets[STANCE] = vector2d<int16_t>(168, 109);
		statoffsets[WDEF] = vector2d<int16_t>(73, 127);
		statoffsets[MDEF] = vector2d<int16_t>(73, 145);
		statoffsets[ACCURACY] = vector2d<int16_t>(73, 163);
		statoffsets[AVOID] = vector2d<int16_t>(73, 199);
		statoffsets[SPEED] = vector2d<int16_t>(73, 235);
		statoffsets[JUMP] = vector2d<int16_t>(168, 235);
		statoffsets[HONOR] = vector2d<int16_t>(73, 353);

		dimension = vector2d<int16_t>(212, 318);
		showdetail = false;
	}

	void UIStatsinfo::draw(float inter) const
	{
		UIElement::draw(inter);

		if (showdetail)
		{
			for (auto& dtit : detailtextures)
			{
				dtit.draw(position + vector2d<int16_t>(213, 0));
			}
			abilities.at("none").draw(position + vector2d<int16_t>(213, 0));
		}

		size_t last = showdetail ? NUMLABELS : NUMNORMAL;
		for (size_t i = 0; i < last; i++)
		{
			vector2d<int16_t> labelpos = position + statoffsets[i];
			if (i >= NUMNORMAL)
				labelpos.shiftx(213);

			statlabels[i].draw(labelpos);
		}
	}

	void UIStatsinfo::update()
	{
		statlabels[NAME].settext(stats.getname());
		statlabels[JOB].settext(stats.getjobname());
		statlabels[GUILD].settext("");
		statlabels[FAME].settext(std::to_string(stats.getstat(Maplestat::FAME)));
		statlabels[DAMAGE].settext(std::to_string(stats.getmindamage()) + " ~ " + std::to_string(stats.getmaxdamage()));
		statlabels[HP].settext(std::to_string(stats.getstat(Maplestat::HP)) + " / " + std::to_string(stats.gettotal(Equipstat::HP)));
		statlabels[MP].settext(std::to_string(stats.getstat(Maplestat::MP)) + " / " + std::to_string(stats.gettotal(Equipstat::MP)));
		statlabels[AP].settext(std::to_string(stats.getstat(Maplestat::AP)));
		statlabels[STR].settext(std::to_string(stats.gettotal(Equipstat::STR)) + " (" + std::to_string(stats.getstat(Maplestat::STR)) + " + " +
			std::to_string(stats.gettotal(Equipstat::STR) - stats.getstat(Maplestat::STR)) + ")");
		statlabels[DEX].settext(std::to_string(stats.gettotal(Equipstat::DEX)) + " (" + std::to_string(stats.getstat(Maplestat::DEX)) + " + " +
			std::to_string(stats.gettotal(Equipstat::DEX) - stats.getstat(Maplestat::DEX)) + ")");
		statlabels[INT].settext(std::to_string(stats.gettotal(Equipstat::INT)) + " (" + std::to_string(stats.getstat(Maplestat::INT)) + " + " +
			std::to_string(stats.gettotal(Equipstat::INT) - stats.getstat(Maplestat::INT)) + ")");
		statlabels[LUK].settext(std::to_string(stats.gettotal(Equipstat::LUK)) + " (" + std::to_string(stats.getstat(Maplestat::LUK)) + " + " +
			std::to_string(stats.gettotal(Equipstat::LUK) - stats.getstat(Maplestat::LUK)) + ")");

		if (showdetail)
		{
			statlabels[ATTACK].settext(std::to_string(stats.gettotal(Equipstat::WATK)));
			statlabels[CRIT].settext(std::to_string(static_cast<int32_t>(stats.getcritical() * 100)) + "%");
			statlabels[MINCRIT].settext(std::to_string(static_cast<int32_t>(stats.getmincrit() * 100)) + "%");
			statlabels[MAXCRIT].settext(std::to_string(static_cast<int32_t>(stats.getmaxcrit() * 100)) + "%");
			statlabels[BDM].settext(std::to_string(static_cast<int32_t>(stats.getbossdmg() * 100)) + "%");
			statlabels[IGNOREDEF].settext(std::to_string(static_cast<int32_t>(stats.getignoredef() * 100)) + "%");
			statlabels[RESIST].settext(std::to_string(static_cast<int32_t>(stats.getresist() * 100)) + "%");
			statlabels[STANCE].settext(std::to_string(static_cast<int32_t>(stats.getstance() * 100)) + "%");
			statlabels[WDEF].settext(std::to_string(stats.gettotal(Equipstat::WDEF)));
			statlabels[MDEF].settext(std::to_string(stats.gettotal(Equipstat::MDEF)));
			statlabels[ACCURACY].settext(std::to_string(stats.gettotal(Equipstat::ACC)));
			statlabels[AVOID].settext(std::to_string(stats.gettotal(Equipstat::AVOID)));
			statlabels[SPEED].settext(std::to_string(stats.gettotal(Equipstat::SPEED)) + "%");
			statlabels[JUMP].settext(std::to_string(stats.gettotal(Equipstat::JUMP)) + "%");
			statlabels[HONOR].settext(std::to_string(stats.gethonor()));
		}
	}

	void UIStatsinfo::buttonpressed(uint16_t id)
	{
		switch (id)
		{
		case BT_DETAILOPEN:
			showdetail = true;
			buttons[BT_DETAILOPEN]->setactive(false);
			buttons[BT_DETAILCLOSE]->setactive(true);
			break;
		case BT_DETAILCLOSE:
			showdetail = false;
			buttons[BT_DETAILCLOSE]->setactive(false);
			buttons[BT_DETAILOPEN]->setactive(true);
			break;
		case BT_HP:
			sendappacket(Maplestat::HP);
			break;
		case BT_MP:
			sendappacket(Maplestat::MP);
			break;
		case BT_STR:
			sendappacket(Maplestat::STR);
			break;
		case BT_DEX:
			sendappacket(Maplestat::DEX);
			break;
		case BT_INT:
			sendappacket(Maplestat::INT);
			break;
		case BT_LUK:
			sendappacket(Maplestat::LUK);
			break;
		}

		buttons[id]->setstate(Button::NORMAL);
	}

	void UIStatsinfo::sendappacket(Maplestat::Value stat)
	{
		using Net::Session;
		using Net::SpendApPacket;
		Session::get().dispatch(SpendApPacket(stat));

		UI::get().disable();
	}

	void UIStatsinfo::updateap()
	{
		Button::State newstate;
		if (stats.getstat(Maplestat::AP) > 0)
		{
			newstate = Button::NORMAL;

			buttons[BT_HP]->setposition(vector2d<int16_t>(20, -36));
			buttons[BT_MP]->setposition(vector2d<int16_t>(20, -18));
			buttons[BT_STR]->setposition(vector2d<int16_t>(20, 51));
			buttons[BT_DEX]->setposition(vector2d<int16_t>(20, 69));
			buttons[BT_INT]->setposition(vector2d<int16_t>(20, 87));
			buttons[BT_LUK]->setposition(vector2d<int16_t>(20, 105));
		}
		else
		{
			newstate = Button::DISABLED;

			buttons[BT_HP]->setposition(vector2d<int16_t>(-48, 14));
			buttons[BT_MP]->setposition(vector2d<int16_t>(-48, 32));
			buttons[BT_STR]->setposition(vector2d<int16_t>(-48, 101));
			buttons[BT_DEX]->setposition(vector2d<int16_t>(-48, 119));
			buttons[BT_INT]->setposition(vector2d<int16_t>(-48, 137));
			buttons[BT_LUK]->setposition(vector2d<int16_t>(-48, 155));
		}

		buttons[BT_HP]->setstate(newstate);
		buttons[BT_MP]->setstate(newstate);
		buttons[BT_STR]->setstate(newstate);
		buttons[BT_DEX]->setstate(newstate);
		buttons[BT_LUK]->setstate(newstate);
		buttons[BT_INT]->setstate(newstate);
	}
}