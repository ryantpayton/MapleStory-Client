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

		statlabel = Text(Text::A11M, Text::LEFT, Text::DARKGREY);

		dimension = vector2d<int16_t>(212, 318);
		showdetail = false;
	}

	void UIStatsinfo::draw(float inter) const
	{
		UIElement::draw(inter);

		statlabel.drawline(stats.getname(), position + vector2d<int16_t>(73, 27));
		statlabel.drawline(stats.getjobname(), position + vector2d<int16_t>(73, 45));
		statlabel.drawline("", position + vector2d<int16_t>(73, 63)); //guild

		statlabel.drawline(
			std::to_string(stats.getstat(Character::MS_FAME)),
			position + vector2d<int16_t>(73, 81)
			);
		statlabel.drawline(
			std::to_string(stats.getmindamage()) + " ~ " +
			std::to_string(stats.getmaxdamage()),
			position + vector2d<int16_t>(73, 99)
			);
		statlabel.drawline(
			std::to_string(stats.getstat(Character::MS_HP)) + " / " +
			std::to_string(stats.gettotal(Character::ES_HP)),
			position + vector2d<int16_t>(73, 117)
			);
		statlabel.drawline(
			std::to_string(stats.getstat(Character::MS_MP)) + " / " +
			std::to_string(stats.gettotal(Character::ES_MP)),
			position + vector2d<int16_t>(73, 135)
			);
		statlabel.drawline(
			std::to_string(stats.getstat(Character::MS_AP)),
			position + vector2d<int16_t>(70, 177)
			);
		statlabel.drawline(
			std::to_string(stats.gettotal(Character::ES_STR)) + " (" +
			std::to_string(stats.getstat(Character::MS_STR)) + " + " +
			std::to_string(stats.gettotal(Character::ES_STR) - stats.getstat(Character::MS_STR)) + ")",
			position + vector2d<int16_t>(73, 204)
			);
		statlabel.drawline(
			std::to_string(stats.gettotal(Character::ES_DEX)) + " (" +
			std::to_string(stats.getstat(Character::MS_DEX)) + " + " +
			std::to_string(stats.gettotal(Character::ES_DEX) - stats.getstat(Character::MS_DEX)) + ")",
			position + vector2d<int16_t>(73, 222)
			);
		statlabel.drawline(
			std::to_string(stats.gettotal(Character::ES_INT)) + " (" +
			std::to_string(stats.getstat(Character::MS_INT)) + " + " +
			std::to_string(stats.gettotal(Character::ES_INT) - stats.getstat(Character::MS_INT)) + ")",
			position + vector2d<int16_t>(73, 240)
			);
		statlabel.drawline(
			std::to_string(stats.gettotal(Character::ES_LUK)) + " (" +
			std::to_string(stats.getstat(Character::MS_LUK)) + " + " +
			std::to_string(stats.gettotal(Character::ES_LUK) - stats.getstat(Character::MS_LUK)) + ")",
			position + vector2d<int16_t>(73, 258)
			);

		if (showdetail)
		{
			vector2d<int16_t> detailpos = position + vector2d<int16_t>(213, 0);

			using Graphics::DrawArgument;
			DrawArgument detailargs = DrawArgument(detailpos);
			for (auto& dtit : detailtextures)
			{
				dtit.draw(detailargs);
			}
			abilities.at("none").draw(detailargs);

			string attstr = std::to_string(stats.gettotal(Character::ES_WATK));
			string critstr = std::to_string(static_cast<int32_t>(stats.getcritical() * 100));
			string mincritstr = std::to_string(static_cast<int32_t>(stats.getmincrit() * 100));
			string maxcritstr = std::to_string(static_cast<int32_t>(stats.getmaxcrit() * 100));
			string bdmstr = std::to_string(static_cast<int32_t>(stats.getbossdmg() * 100));
			string idefstr = std::to_string(static_cast<int32_t>(stats.getignoredef() * 100));
			string resistr = std::to_string(static_cast<int32_t>(stats.getresist() * 100));
			string stancestr = std::to_string(static_cast<int32_t>(stats.getstance() * 100));
			string wdefstr = std::to_string(stats.gettotal(Character::ES_WDEF));
			string mdefstr = std::to_string(stats.gettotal(Character::ES_MDEF));
			string accstr = std::to_string(stats.gettotal(Character::ES_ACC));
			string evastr = std::to_string(stats.gettotal(Character::ES_AVOID));
			string speedstr = std::to_string(stats.gettotal(Character::ES_SPEED)) + "%";
			string jumpstr = std::to_string(stats.gettotal(Character::ES_JUMP)) + "%";
			string honorstr = std::to_string(stats.gethonor());

			statlabel.drawline(attstr, detailpos + vector2d<int16_t>(73, 37));
			statlabel.drawline(critstr + "%", detailpos + vector2d<int16_t>(73, 55));
			statlabel.drawline(mincritstr + "%", detailpos + vector2d<int16_t>(73, 73));
			statlabel.drawline(maxcritstr + "%", detailpos + vector2d<int16_t>(168, 73));
			statlabel.drawline(bdmstr + "%", detailpos + vector2d<int16_t>(73, 91));
			statlabel.drawline(idefstr + "%", detailpos + vector2d<int16_t>(168, 91));
			statlabel.drawline(resistr + "%", detailpos + vector2d<int16_t>(73, 109));
			statlabel.drawline(stancestr + "%", detailpos + vector2d<int16_t>(168, 109));
			statlabel.drawline(wdefstr, detailpos + vector2d<int16_t>(73, 127));
			statlabel.drawline(mdefstr, detailpos + vector2d<int16_t>(73, 145));
			statlabel.drawline(accstr, detailpos + vector2d<int16_t>(73, 163));
			statlabel.drawline("0", detailpos + vector2d<int16_t>(73, 181));
			statlabel.drawline(evastr, detailpos + vector2d<int16_t>(73, 199));
			statlabel.drawline("0", detailpos + vector2d<int16_t>(73, 217));
			statlabel.drawline(speedstr, detailpos + vector2d<int16_t>(73, 235));
			statlabel.drawline(jumpstr, detailpos + vector2d<int16_t>(168, 235));
			statlabel.drawline(honorstr, detailpos + vector2d<int16_t>(73, 353));
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
			sendappacket(Character::MS_HP);
			break;
		case BT_MP:
			sendappacket(Character::MS_MP);
			break;
		case BT_STR:
			sendappacket(Character::MS_STR);
			break;
		case BT_DEX:
			sendappacket(Character::MS_DEX);
			break;
		case BT_INT:
			sendappacket(Character::MS_INT);
			break;
		case BT_LUK:
			sendappacket(Character::MS_LUK);
			break;
		}

		buttons[id]->setstate(Button::NORMAL);
	}

	void UIStatsinfo::sendappacket(Maplestat stat)
	{
		using Net::SpendApPacket;
		Net::Session::dispatch(SpendApPacket(stat));

		UI::disable();
	}

	void UIStatsinfo::updateap()
	{
		Button::State newstate;
		if (stats.getstat(Character::MS_AP) > 0)
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