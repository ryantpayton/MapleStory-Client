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
#include "UIStatusbar.h"
#include "UIStatsinfo.h"
#include "UIItemInventory.h"
#include "UIEquipInventory.h"
#include "IO\UI.h"
#include "IO\Components\MapleButton.h"
#include "nlnx\nx.hpp"

namespace IO
{
	UIStatusbar::UIStatusbar(const CharStats& st, const Inventory& inv) 
		: stats(st), inventory(inv) {

		node mainbar = nl::nx::ui["StatusBar2.img"]["mainBar"];

		sprites.push_back(Sprite(mainbar["backgrnd"]));
		sprites.push_back(Sprite(mainbar["gaugeBackgrd"]));
		sprites.push_back(Sprite(mainbar["notice"]));
		sprites.push_back(Sprite(mainbar["lvBacktrnd"]));
		sprites.push_back(Sprite(mainbar["lvCover"]));

		expbar = Bar(
			Texture(mainbar.resolve("gauge/exp/0")), 
			Texture(mainbar.resolve("gauge/exp/1")), 
			Texture(mainbar.resolve("gauge/exp/2")), 
			308, getexppercent()
			);
		hpbar = Bar(
			Texture(mainbar.resolve("gauge/hp/0")), 
			Texture(mainbar.resolve("gauge/hp/1")), 
			Texture(mainbar.resolve("gauge/hp/2")), 
			137, gethppercent()
			);
		mpbar = Bar(
			Texture(mainbar.resolve("gauge/mp/0")), 
			Texture(mainbar.resolve("gauge/mp/1")), 
			Texture(mainbar.resolve("gauge/mp/2")), 
			137, getmppercent()
			);

		statset = Charset(mainbar.resolve("gauge/number"), Charset::RIGHT);
		levelset = Charset(mainbar.resolve("lvNumber"), Charset::LEFT);

		joblabel = Text(Text::A11M, Text::LEFT, Text::YELLOW);
		joblabel.settext(stats.getjobname());
		namelabel = Text(Text::A13M, Text::LEFT, Text::WHITE);
		namelabel.settext(stats.getname());

		buttons[BT_WHISPER] = unique_ptr<Button>(new MapleButton(mainbar["BtChat"]));
		buttons[BT_CALLGM] = unique_ptr<Button>(new MapleButton(mainbar["BtClaim"]));

		buttons[BT_CASHSHOP] = unique_ptr<Button>(new MapleButton(mainbar["BtCashShop"]));
		buttons[BT_TRADE] = unique_ptr<Button>(new MapleButton(mainbar["BtMTS"]));
		buttons[BT_MENU] = unique_ptr<Button>(new MapleButton(mainbar["BtMenu"]));
		buttons[BT_OPTIONS] = unique_ptr<Button>(new MapleButton(mainbar["BtSystem"]));

		buttons[BT_CHARACTER] = unique_ptr<Button>(new MapleButton(mainbar["BtCharacter"]));
		buttons[BT_STATS] = unique_ptr<Button>(new MapleButton(mainbar["BtStat"]));
		buttons[BT_QUEST] = unique_ptr<Button>(new MapleButton(mainbar["BtQuest"]));
		buttons[BT_INVENTORY] = unique_ptr<Button>(new MapleButton(mainbar["BtInven"]));
		buttons[BT_EQUIPS] = unique_ptr<Button>(new MapleButton(mainbar["BtEquip"]));
		buttons[BT_SKILL] = unique_ptr<Button>(new MapleButton(mainbar["BtSkill"]));

		position = vector2d<int16_t>(512, 590);
		dimension = vector2d<int16_t>(1366, 80);

		chatbar = unique_ptr<Chatbar>(new Chatbar(position));
	}

	void UIStatusbar::draw(float inter) const
	{
		UIElement::draw(inter);

		int64_t currentexp = stats.getexp();
		uint16_t currenthp = stats.getstat(Maplestat::HP);
		uint16_t currentmp = stats.getstat(Maplestat::MP);
		uint32_t maxhp = stats.gettotal(Equipstat::HP);
		uint32_t maxmp = stats.gettotal(Equipstat::MP);

		expbar.draw(position + vector2d<int16_t>(-261, -15));
		hpbar.draw(position + vector2d<int16_t>(-261, -31));
		mpbar.draw(position + vector2d<int16_t>(-90, -31));

		string expstring = std::to_string(100 * getexppercent());
		statset.draw(
			std::to_string(currentexp) + "[" + expstring.substr(0, expstring.find('.') + 3) + "%]",
			DrawArgument(position + vector2d<int16_t>(47, -13))
			);
		statset.draw(
			"[" + std::to_string(currenthp) + "/" + std::to_string(maxhp) + "]",
			DrawArgument(position + vector2d<int16_t>(-124, -29))
			);
		statset.draw(
			"[" + std::to_string(currentmp) + "/" + std::to_string(maxmp) + "]",
			DrawArgument(position + vector2d<int16_t>(47, -29))
			);
		levelset.draw(
			std::to_string(stats.getstat(Maplestat::LEVEL)),
			DrawArgument(position + vector2d<int16_t>(-480, -24))
			);

		joblabel.draw(position + vector2d<int16_t>(-435, -21));
		namelabel.draw(position + vector2d<int16_t>(-435, -36));

		chatbar->draw(inter);
	}

	void UIStatusbar::update()
	{
		UIElement::update();

		chatbar->update();

		expbar.update(getexppercent());
		hpbar.update(gethppercent());
		mpbar.update(getmppercent());
	}

	void UIStatusbar::buttonpressed(uint16_t id)
	{
		switch (id)
		{
		/*case BT_BAR_SYSOP:
			uinterface.add(UI_SYSTEM);
			break;*/
		case BT_STATS:
			UI::get().add(ElementStatsinfo(stats));
			break;
		case BT_INVENTORY:
			UI::get().add(ElementItemInventory(inventory));
			break;
		case BT_EQUIPS:
			UI::get().add(ElementEquipInventory(inventory));
			break;
		}

		buttons[id].get()->setstate(Button::MOUSEOVER);
	}

	rectangle2d<int16_t> UIStatusbar::bounds() const
	{
		return rectangle2d<int16_t>(
			position - vector2d<int16_t>(512, 84), 
			position - vector2d<int16_t>(512, 84) + dimension
			);
	}

	Cursor::Mousestate UIStatusbar::sendmouse(bool pressed, vector2d<int16_t> cursorpos)
	{
		if (chatbar->bounds().contains(cursorpos))
		{
			UIElement::sendmouse(pressed, cursorpos);
			return chatbar->sendmouse(pressed, cursorpos);
		}
		else
		{
			chatbar->sendmouse(pressed, cursorpos);
			return UIElement::sendmouse(pressed, cursorpos);
		}
	}

	void UIStatusbar::sendchatline(string line, int8_t type)
	{
		chatbar->sendline(line, type);
	}

	float UIStatusbar::getexppercent() const
	{
		int64_t currentexp = stats.getexp();
		int64_t expneeded = stats.getexpneeded();

		return static_cast<float>(static_cast<double>(currentexp) / expneeded);
	}

	float UIStatusbar::gethppercent() const
	{
		uint16_t currenthp = stats.getstat(Maplestat::HP);
		uint32_t maxhp = stats.gettotal(Equipstat::HP);

		return static_cast<float>(currenthp) / maxhp;
	}

	float UIStatusbar::getmppercent() const
	{
		uint16_t currentmp = stats.getstat(Maplestat::MP);
		uint32_t maxmp = stats.gettotal(Equipstat::MP);

		return static_cast<float>(currentmp) / maxmp;
	}
}