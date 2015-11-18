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
#include "Net\Packets\PlayerPackets83.h"
#include "nlnx\nx.hpp"

namespace IO
{
	UIStatsinfo::UIStatsinfo(const Charstats& st) : stats(st) 
	{
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

		if (stats.getstat(Character::MS_AP) == 0)
		{
			buttons[BT_HP]->setstate(Button::DISABLED);
			buttons[BT_MP]->setstate(Button::DISABLED);
			buttons[BT_STR]->setstate(Button::DISABLED);
			buttons[BT_DEX]->setstate(Button::DISABLED);
			buttons[BT_LUK]->setstate(Button::DISABLED);
			buttons[BT_INT]->setstate(Button::DISABLED);
		}

		buttons[BT_DETAILOPEN] = unique_ptr<Button>(new MapleButton(src["BtDetailOpen"]));
		buttons[BT_DETAILCLOSE] = unique_ptr<Button>(new MapleButton(src["BtDetailClose"]));
		buttons[BT_DETAILCLOSE]->setactive(false);

		statlabel = Textlabel(Textlabel::DWF_12ML, Textlabel::TXC_BLACK, "", 0);

		position = vector2d<int16_t>(200, 100);
		dimension = vector2d<int16_t>(212, 318);
		active = true;
		showdetail = false;
	}

	void UIStatsinfo::draw(float inter) const
	{
		UIElement::draw(inter);

		statlabel.drawline(stats.getname(), position + vector2d<int16_t>(73, 25));
		statlabel.drawline(stats.getjobname(), position + vector2d<int16_t>(73, 43));
		statlabel.drawline("", position + vector2d<int16_t>(73, 61)); //guild

		statlabel.drawline(
			std::to_string(stats.getstat(Character::MS_FAME)),
			position + vector2d<int16_t>(73, 79)
			);
		statlabel.drawline(
			std::to_string(stats.getmindamage()) + " ~ " +
			std::to_string(stats.getmaxdamage()),
			position + vector2d<int16_t>(73, 97)
			);
		statlabel.drawline(
			std::to_string(stats.getstat(Character::MS_HP)) + " / " +
			std::to_string(stats.gettotal(Character::ES_HP)),
			position + vector2d<int16_t>(73, 115)
			);
		statlabel.drawline(
			std::to_string(stats.getstat(Character::MS_MP)) + " / " +
			std::to_string(stats.gettotal(Character::ES_MP)),
			position + vector2d<int16_t>(73, 133)
			);
		statlabel.drawline(
			std::to_string(stats.getstat(Character::MS_AP)),
			position + vector2d<int16_t>(79, 175)
			);
		statlabel.drawline(
			std::to_string(stats.gettotal(Character::ES_STR)) + " (" +
			std::to_string(stats.getstat(Character::MS_STR)) + " + " +
			std::to_string(stats.gettotal(Character::ES_STR) - stats.getstat(Character::MS_STR)) + ")",
			position + vector2d<int16_t>(73, 202)
			);
		statlabel.drawline(
			std::to_string(stats.gettotal(Character::ES_DEX)) + " (" +
			std::to_string(stats.getstat(Character::MS_DEX)) + " + " +
			std::to_string(stats.gettotal(Character::ES_DEX) - stats.getstat(Character::MS_DEX)) + ")",
			position + vector2d<int16_t>(73, 220)
			);
		statlabel.drawline(
			std::to_string(stats.gettotal(Character::ES_INT)) + " (" +
			std::to_string(stats.getstat(Character::MS_INT)) + " + " +
			std::to_string(stats.gettotal(Character::ES_INT) - stats.getstat(Character::MS_INT)) + ")",
			position + vector2d<int16_t>(73, 238)
			);
		statlabel.drawline(
			std::to_string(stats.gettotal(Character::ES_LUK)) + " (" +
			std::to_string(stats.getstat(Character::MS_LUK)) + " + " +
			std::to_string(stats.gettotal(Character::ES_LUK) - stats.getstat(Character::MS_LUK)) + ")",
			position + vector2d<int16_t>(73, 256)
			);

		if (showdetail)
		{
			vector2d<int16_t> detailpos = position + vector2d<int16_t>(213, 0);

			for (vector<Texture>::const_iterator dtit = detailtextures.begin(); dtit != detailtextures.end(); ++dtit)
			{
				dtit->draw(detailpos);
			}

			abilities.at("none").draw(detailpos);

			string critstr = std::to_string(static_cast<int32_t>(stats.getcritical() * 100));
			string mincritstr = std::to_string(static_cast<int32_t>(stats.getmincrit() * 100));
			string maxcritstr = std::to_string(static_cast<int32_t>(stats.getmaxcrit() * 100));
			string bdmstr = std::to_string(static_cast<int32_t>(stats.getbossdmg() * 100));
			string idefstr = std::to_string(static_cast<int32_t>(stats.getignoredef() * 100));
			string resistr = std::to_string(static_cast<int32_t>(stats.getresist() * 100));
			string stancestr = std::to_string(static_cast<int32_t>(stats.getstance() * 100));

			statlabel.drawline(std::to_string(stats.getattack()), detailpos + vector2d<int16_t>(73, 35));
			statlabel.drawline(critstr + "%", detailpos + vector2d<int16_t>(73, 53));
			statlabel.drawline(mincritstr + "%", detailpos + vector2d<int16_t>(73, 71));
			statlabel.drawline(maxcritstr + "%", detailpos + vector2d<int16_t>(168, 71));
			statlabel.drawline(bdmstr + "%", detailpos + vector2d<int16_t>(73, 89));
			statlabel.drawline(idefstr + "%", detailpos + vector2d<int16_t>(168, 89));
			statlabel.drawline(resistr + "%", detailpos + vector2d<int16_t>(73, 107));
			statlabel.drawline(stancestr + "%", detailpos + vector2d<int16_t>(168, 107));
			statlabel.drawline(std::to_string(stats.gettotal(Character::ES_WDEF)), detailpos + vector2d<int16_t>(73, 125));
			statlabel.drawline(std::to_string(stats.gettotal(Character::ES_MDEF)), detailpos + vector2d<int16_t>(73, 143));
			statlabel.drawline(std::to_string(stats.gettotal(Character::ES_ACC)), detailpos + vector2d<int16_t>(73, 161));
			statlabel.drawline("0", detailpos + vector2d<int16_t>(73, 179));
			statlabel.drawline(std::to_string(stats.gettotal(Character::ES_AVOID)), detailpos + vector2d<int16_t>(73, 197));
			statlabel.drawline("0", detailpos + vector2d<int16_t>(73, 215));
			statlabel.drawline(std::to_string(stats.gettotal(Character::ES_SPEED)) + "%", detailpos + vector2d<int16_t>(73, 233));
			statlabel.drawline(std::to_string(stats.gettotal(Character::ES_JUMP)) + "%", detailpos + vector2d<int16_t>(168, 233));
			statlabel.drawline(std::to_string(stats.gethonor()), detailpos + vector2d<int16_t>(73, 351));
		}
	}

	void UIStatsinfo::buttonpressed(uint16_t id)
	{
		using Net::SpendApPacket83;

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
			Net::Session::dispatch(SpendApPacket83(Character::MS_HP));
			UI::disable();
			break;
		case BT_MP:
			Net::Session::dispatch(SpendApPacket83(Character::MS_MP));
			UI::disable();
			break;
		case BT_STR:
			Net::Session::dispatch(SpendApPacket83(Character::MS_STR));
			UI::disable();
			break;
		case BT_DEX:
			Net::Session::dispatch(SpendApPacket83(Character::MS_DEX));
			UI::disable();
			break;
		case BT_INT:
			Net::Session::dispatch(SpendApPacket83(Character::MS_INT));
			UI::disable();
			break;
		case BT_LUK:
			Net::Session::dispatch(SpendApPacket83(Character::MS_LUK));
			UI::disable();
			break;
		}

		buttons[id]->setstate(Button::NORMAL);
	}
}