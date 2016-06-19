/////////////////////////////////////////////////////////////////////////////
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
#include "UIStatusbar.h"

#include "UIStatsinfo.h"
#include "UIItemInventory.h"
#include "UIEquipInventory.h"
#include "UISkillbook.h"

#include "..\UI.h"
#include "..\Components\MapleButton.h"

#include "..\..\Gameplay\Stage.h"

#include <nlnx\nx.hpp>

namespace jrc
{
	UIStatusbar::UIStatusbar() 
		: UIElement(POSITION, DIMENSION), chatbar(POSITION), stats(Stage::get().get_player().get_stats()) {

		nl::node mainbar = nl::nx::ui["StatusBar2.img"]["mainBar"];

		sprites.emplace_back(mainbar["backgrnd"]);
		sprites.emplace_back(mainbar["gaugeBackgrd"]);
		sprites.emplace_back(mainbar["notice"]);
		sprites.emplace_back(mainbar["lvBacktrnd"]);
		sprites.emplace_back(mainbar["lvCover"]);

		expbar = { 
			mainbar.resolve("gauge/exp/0"), 
			mainbar.resolve("gauge/exp/1"),
			mainbar.resolve("gauge/exp/2"), 
			308, 0.0f
		};
		hpbar = {
			mainbar.resolve("gauge/hp/0"),
			mainbar.resolve("gauge/hp/1"),
			mainbar.resolve("gauge/hp/2"),
			137, 0.0f
		};
		mpbar = {
			mainbar.resolve("gauge/mp/0"),
			mainbar.resolve("gauge/mp/1"),
			mainbar.resolve("gauge/mp/2"),
			137, 0.0f
		};

		statset = { mainbar["gauge"]["number"], Charset::RIGHT };
		levelset = { mainbar["lvNumber"], Charset::LEFT };

		joblabel = { Text::A11M, Text::LEFT, Text::YELLOW };
		namelabel = { Text::A13M, Text::LEFT, Text::WHITE };

		buttons[BT_WHISPER] = std::make_unique<MapleButton>(mainbar["BtChat"]);
		buttons[BT_CALLGM] = std::make_unique<MapleButton>(mainbar["BtClaim"]);

		buttons[BT_CASHSHOP] = std::make_unique<MapleButton>(mainbar["BtCashShop"]);
		buttons[BT_TRADE] = std::make_unique<MapleButton>(mainbar["BtMTS"]);
		buttons[BT_MENU] = std::make_unique<MapleButton>(mainbar["BtMenu"]);
		buttons[BT_OPTIONS] = std::make_unique<MapleButton>(mainbar["BtSystem"]);

		buttons[BT_CHARACTER] = std::make_unique<MapleButton>(mainbar["BtCharacter"]);
		buttons[BT_STATS] = std::make_unique<MapleButton>(mainbar["BtStat"]);
		buttons[BT_QUEST] = std::make_unique<MapleButton>(mainbar["BtQuest"]);
		buttons[BT_INVENTORY] = std::make_unique<MapleButton>(mainbar["BtInven"]);
		buttons[BT_EQUIPS] = std::make_unique<MapleButton>(mainbar["BtEquip"]);
		buttons[BT_SKILL] = std::make_unique<MapleButton>(mainbar["BtSkill"]);
	}

	void UIStatusbar::draw(float alpha) const
	{
		UIElement::draw(alpha);

		expbar.draw(position + Point<int16_t>(-261, -15));
		hpbar.draw(position + Point<int16_t>(-261, -31));
		mpbar.draw(position + Point<int16_t>(-90, -31));

		int16_t level = stats.get_stat(Maplestat::LEVEL);
		int16_t hp = stats.get_stat(Maplestat::HP);
		int16_t mp = stats.get_stat(Maplestat::MP);
		int32_t maxhp = stats.get_total(Equipstat::HP);
		int32_t maxmp = stats.get_total(Equipstat::MP);
		int64_t exp = stats.get_exp();

		std::string expstring = std::to_string(100 * getexppercent());
		statset.draw(
			std::to_string(exp) + "[" + expstring.substr(0, expstring.find('.') + 3) + "%]",
			position + Point<int16_t>(47, -13)
			);
		statset.draw(
			"[" + std::to_string(hp) + "/" + std::to_string(maxhp) + "]",
			position + Point<int16_t>(-124, -29)
			);
		statset.draw(
			"[" + std::to_string(mp) + "/" + std::to_string(maxmp) + "]",
			position + Point<int16_t>(47, -29)
			);
		levelset.draw(
			std::to_string(level),
			position + Point<int16_t>(-480, -24)
			);

		joblabel.draw(position + Point<int16_t>(-435, -21));
		namelabel.draw(position + Point<int16_t>(-435, -36));

		chatbar.draw(alpha);
	}

	void UIStatusbar::update()
	{
		UIElement::update();

		chatbar.update();

		expbar.update(getexppercent());
		hpbar.update(gethppercent());
		mpbar.update(getmppercent());

		namelabel.change_text(stats.get_name());
		joblabel.change_text(stats.get_jobname());

		for (auto iter : message_cooldowns)
		{
			iter.second -= Constants::TIMESTEP;
		}
	}

	Button::State UIStatusbar::button_pressed(uint16_t id)
	{
		switch (id)
		{
		/*case BT_BAR_SYSOP:
			uinterface.add(UI_SYSTEM);
			break;*/
		case BT_STATS:
			UI::get().add(Element<UIStatsinfo>());
			return Button::NORMAL;
		case BT_INVENTORY:
			UI::get().add(Element<UIItemInventory>());
			return Button::NORMAL;
		case BT_EQUIPS:
			UI::get().add(Element<UIEquipInventory>());
			return Button::NORMAL;
		case BT_SKILL:
			UI::get().add(Element<UISkillbook>());
			return Button::NORMAL;
		}
		return Button::PRESSED;
	}

	bool UIStatusbar::is_in_range(Point<int16_t> cursorpos) const
	{
		Rectangle<int16_t> bounds(
			position - Point<int16_t>(512, 84),
			position - Point<int16_t>(512, 84) + dimension
			);
		return bounds.contains(cursorpos) || chatbar.is_in_range(cursorpos);
	}

	bool UIStatusbar::remove_cursor(bool clicked, Point<int16_t> cursorpos)
	{
		if (chatbar.remove_cursor(clicked, cursorpos))
			return true;

		return UIElement::remove_cursor(clicked, cursorpos);
	}

	Cursor::State UIStatusbar::send_cursor(bool pressed, Point<int16_t> cursorpos)
	{
		if (chatbar.is_in_range(cursorpos))
		{
			UIElement::send_cursor(pressed, cursorpos);
			return chatbar.send_cursor(pressed, cursorpos);
		}
		else
		{
			chatbar.send_cursor(pressed, cursorpos);
			return UIElement::send_cursor(pressed, cursorpos);
		}
	}

	void UIStatusbar::send_chatline(const std::string& line, UIChatbar::LineType type)
	{
		chatbar.send_line(line, type);
	}

	void UIStatusbar::display_message(Messages::Type line, UIChatbar::LineType type)
	{
		if (message_cooldowns[line] > 0)
			return;

		std::string message = messages.stringfor(line);
	    chatbar.send_line(message, type);

		message_cooldowns[line] = MESSAGE_COOLDOWN;
	}

	constexpr size_t EXPCAP = 201;
	constexpr int64_t exptable[EXPCAP] =
	{
		1, 15, 34, 57, 92, 135, 372, 560, 840, 1242, 1144, 1573, 2144, 2800, 3640, 4700, 5893, 7360, 9144,
		11120, 13477, 16268, 19320, 22880, 27008, 31477, 36600, 42444, 48720, 55813, 63800, 86784, 98208,
		110932, 124432, 139372, 155865, 173280, 192400, 213345, 235372, 259392, 285532, 312928, 342624,
		374760, 408336, 445544, 483532, 524160, 567772, 598886, 631704, 666321, 702836, 741351, 781976,
		824828, 870028, 917625, 967995, 1021041, 1076994, 1136013, 1198266, 1263930, 1333194, 1406252,
		1483314, 1564600, 1650340, 1740778, 1836173, 1936794, 2042930, 2154882, 2272970, 2397528, 2528912,
		2667496, 2813674, 2967863, 3130502, 3302053, 3483005, 3673873, 3875201, 4087562, 4311559, 4547832,
		4797053, 5059931, 5337215, 5629694, 5938202, 6263614, 6606860, 6968915, 7350811, 7753635, 8178534,
		8626718, 9099462, 9598112, 10124088, 10678888, 11264090, 11881362, 12532461, 13219239, 13943653,
		14707765, 15513750, 16363902, 17260644, 18206527, 19204245, 20256637, 21366700, 22537594, 23772654,
		25075395, 26449526, 27898960, 29427822, 31040466, 32741483, 34535716, 36428273, 38424542, 40530206,
		42751262, 45094030, 47565183, 50171755, 52921167, 55821246, 58880250, 62106888, 65510344, 69100311,
		72887008, 76881216, 81094306, 85594273, 90225770, 95170142, 100385466, 105886589, 111689174, 117809740,
		124265714, 131075474, 138258410, 145834970, 153826726, 162256430, 171148082, 180526997, 190419876,
		200854885, 211861732, 223471711, 223471711, 248635353, 262260570, 276632449, 291791906, 307782102,
		324648562, 342439302, 361204976, 380999008, 401877754, 423900654, 447130410, 471633156, 497478653,
		524740482, 553496261, 583827855, 615821622, 649568646, 685165008, 722712050, 762316670, 804091623,
		848155844, 894634784, 943660770, 995373379, 1049919840, 1107455447, 1168144006, 1232158297,
		1299680571, 1370903066, 1446028554, 1525246918, 1608855764, 1697021059
	};

	float UIStatusbar::getexppercent() const
	{
		int16_t level = stats.get_stat(Maplestat::LEVEL);
		int64_t exp = stats.get_exp();

		return static_cast<float>(
			static_cast<double>(exp) / exptable[level]
			);
	}

	float UIStatusbar::gethppercent() const
	{
		int16_t hp = stats.get_stat(Maplestat::HP);
		int32_t maxhp = stats.get_total(Equipstat::HP);

		return static_cast<float>(hp) / maxhp;
	}

	float UIStatusbar::getmppercent() const
	{
		int16_t mp = stats.get_stat(Maplestat::MP);
		int32_t maxmp = stats.get_total(Equipstat::MP);

		return static_cast<float>(mp) / maxmp;
	}
}