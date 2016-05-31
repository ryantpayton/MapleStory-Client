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
#include "UICharSelect.h"
#include "UISoftkey.h"
#include "UICharcreation.h"

#include "..\UI.h"
#include "..\Components\MapleButton.h"
#include "..\Components\AreaButton.h"

#include "..\..\Constants.h"
#include "..\..\Configuration.h"
#include "..\..\Audio\Audio.h"
#include "..\..\Net\Session.h"
#include "..\..\Net\Packets\SelectCharPackets.h"
#include "..\..\Graphics\Sprite.h"

#include "nlnx\nx.hpp"

namespace jrc
{
	UICharSelect::UICharSelect()
	{
		Login& login = Session::get().getlogin();
		std::string channelid = std::to_string(login.getchannelid());
		charcount = static_cast<uint8_t>(login.getaccount().chars.size());
		charslots = static_cast<uint8_t>(login.getaccount().slots);
		haschars = charcount > 0;

		nl::node title = nl::nx::ui["Login.img"]["Title"];
		nl::node common = nl::nx::ui["Login.img"]["Common"];
		nl::node charselect = nl::nx::ui["Login.img"]["CharSelect"];

		sprites.emplace_back(title["worldsel"]);
		sprites.emplace_back(common["frame"], Point<int16_t>(400, 290));

		// Post BB
		/*selworldpos = Point<int16_t>(578, 42);
		charinfopos = Point<int16_t>(662, 355);
		buttons[BT_ARBEIT] = std::make_unique<MapleButton>(charsel["arbeit"], Point<int16_t>(580, 115)));
		buttons[BT_CARDS] = std::make_unique<MapleButton>(charsel["characterCard"], Point<int16_t>(665, 115)));*/

		charinfopos = Point<int16_t>(662, 305);
		selworldpos = Point<int16_t>(578, 112);

		sprites.emplace_back(charselect["charInfo"], charinfopos);
		sprites.emplace_back(common["selectWorld"], selworldpos);
		sprites.emplace_back(charselect["selectedWorld"]["icon"]["15"], selworldpos);
		sprites.emplace_back(charselect["selectedWorld"]["name"]["15"], selworldpos);
		sprites.emplace_back(charselect["selectedWorld"]["ch"][channelid], selworldpos);

		buttons[BT_SELECTCHAR] = std::make_unique<MapleButton>(charselect["BtSelect"], charinfopos + Point<int16_t>(-76, 72));
		buttons[BT_CREATECHAR] = std::make_unique<MapleButton>(charselect["BtNew"], Point<int16_t>(200, 495));
		buttons[BT_DELETECHAR] = std::make_unique<MapleButton>(charselect["BtDelete"], Point<int16_t>(320, 495));
		buttons[BT_PAGELEFT] = std::make_unique<MapleButton>(charselect["pageL"], Point<int16_t>(100, 490));
		buttons[BT_PAGERIGHT] = std::make_unique<MapleButton>(charselect["pageR"], Point<int16_t>(490, 490));

		levelset = Charset(charselect["lv"], Charset::CENTER);

		if (charslots == charcount)
			buttons[BT_CREATECHAR]->setstate(Button::DISABLED);
		if (charcount + charslots < 9)
		{
			buttons[BT_PAGELEFT]->setstate(Button::DISABLED);
			buttons[BT_PAGERIGHT]->setstate(Button::DISABLED);
		}

		if (charslots > 8)
			charslots = 8;

		for (uint8_t i = charcount; i < charslots; i++)
		{
			sprites.push_back(Sprite(
				charselect["buyCharacter"],
				Point<int16_t>(130 + (120 * (i % 4)), 250 + (200 * (i > 3))))
				);
		}

		selected = Setting<DefaultCharacter>::get().load();
		if (selected >= charcount)
			selected = 0;
		page = selected % 8;

		uint8_t displaycount = charcount;
		if (displaycount > 8)
		{
			displaycount = 8;
		}

		for (char i = 0; i < displaycount; i++)
		{
			addchar(i);

			nametags.emplace_back(
				charselect["nameTag"], Text::A13M, Text::WHITE, login.getchar(i).getstats().name
			);
		}

		if (haschars)
		{
			namelabel = { Text::A18M, Text::CENTER };
			for (size_t i = 0; i < NUM_LABELS; i++)
			{
				infolabels[i] = { Text::A11M, Text::RIGHT };
			}

			updateinfo();
		}

		position = Point<int16_t>(0, 0);
		dimension = Point<int16_t>(800, 600);
		active = true;
	}

	void UICharSelect::draw(float inter) const
	{
		UIElement::draw(inter);

		if (!haschars)
			return;

		for (uint8_t i = 0; i < charcount; i++)
		{
			Point<int16_t> charpos = getcharpos(i);
			charlooks[i].draw(charpos, inter);
			nametags[i].draw(charpos);
		}

		if (selected < charcount)
		{
			const StatsEntry& stats = Session::get().getlogin().getchar(selected).getstats();

			std::string levelstr = std::to_string(stats.stats[Maplestat::LEVEL]);
			int16_t lvx = levelset.draw(levelstr, charinfopos + Point<int16_t>(23, -93));
			levelset.draw('l', charinfopos + Point<int16_t>(-7 - lvx / 2, -93));

			namelabel.draw(charinfopos + Point<int16_t>(0, -85));
			for (size_t i = 0; i < NUM_LABELS; i++)
			{
				Point<int16_t> labelpos = charinfopos + getlabelpos(i);
				infolabels[i].draw(labelpos);
			}
		}
	}

	void UICharSelect::update()
	{
		UIElement::update();

		for (auto& chit : charlooks)
		{
			chit.update(Constants::TIMESTEP);
		}
	}

	void UICharSelect::buttonpressed(uint16_t bid)
	{
		if (bid >= BT_CHAR0)
		{
			buttons[BT_CHAR0 + selected]->setstate(Button::NORMAL);
			nametags[selected].setselected(false);
			charlooks[selected].setstance(Stance::STAND1);

			selected = static_cast<uint8_t>(bid - BT_CHAR0);
			updateinfo();
		}
		else
		{
			switch (bid)
			{
			case BT_SELECTCHAR:
				selectchar();
				break;
			case BT_CREATECHAR:
				if (charcount < charslots)
				{
					active = false;
					UI::get().remove(CHARCREATION);
					UI::get().add(ElementTag<UICharcreation>());
				}
				break;
			case BT_DELETECHAR:
				if (selected < charcount)
				{

				}
				break;
			}

			buttons[bid]->setstate(Button::MOUSEOVER);
		}
	}

	void UICharSelect::updateinfo()
	{
		if (selected < charcount)
		{
			charlooks[selected].setstance(Stance::WALK1);
			nametags[selected].setselected(true);

			std::string name = Session::get().getlogin().getchar(selected).getstats().name;
			namelabel.settext(name);
			for (size_t i = 0; i < NUM_LABELS; i++)
			{
				infolabels[i].settext(getstringfor(i));
			}
		}
	}

	void UICharSelect::selectchar()
	{
		if (selected < charcount)
		{
			Setting<DefaultCharacter>::get().save(selected);
			int32_t cid = Session::get().getlogin().getchar(selected).cid;
			Session::get().getlogin().setcharid(cid);
			switch (Session::get().getlogin().getaccount().pic)
			{
			case 0:
				UI::get().add(ElementTag<UISoftkey, UISoftkey::SkType>(UISoftkey::REGISTER));
				break;
			case 1:
				UI::get().add(ElementTag<UISoftkey, UISoftkey::SkType>(UISoftkey::CHARSELECT));
				break;
			case 2:
				UI::get().disable();
				Sound(Sound::SELECTCHAR).play();
				SelectCharPacket(cid).dispatch();
				break;
			}
		}
	}

	void UICharSelect::addchar(uint8_t index)
	{
		charlooks.emplace_back(
			Session::get().getlogin().getchar(index).getlook()
		);

		Point<int16_t> lt = { 105 + (120 * (index % 4)), 170 + (200 * (index > 3)) };
		Point<int16_t> rb = { 50, 80 };
		int16_t buttonindex = BT_CHAR0 + index;
		buttons[buttonindex] = std::make_unique<AreaButton>(lt, rb);
		if (index == selected)
			buttons[buttonindex]->setstate(Button::PRESSED);
	}

	void UICharSelect::removechar(uint8_t index)
	{
		if (index < charcount)
		{
			charlooks.erase(charlooks.begin() + index);
			nametags.erase(nametags.begin() + index);
			buttons.erase(BT_CHAR0 + index);
		}
	}

	std::string UICharSelect::getstringfor(size_t label) const
	{
		auto& stats = Session::get().getlogin().getchar(selected).getstats();
		switch (label)
		{
		case JOB:
			return stats.job.getname();
		case WORLDRANK:
			return std::to_string(stats.rank.first);
		case JOBRANK:
			return std::to_string(stats.jobrank.first);
		case STR:
			return std::to_string(stats.stats[Maplestat::STR]);
		case DEX:
			return std::to_string(stats.stats[Maplestat::DEX]);
		case INT:
			return std::to_string(stats.stats[Maplestat::INT]);
		case LUK:
			return std::to_string(stats.stats[Maplestat::LUK]);
		default:
			return "";
		}
	}

	Point<int16_t> UICharSelect::getlabelpos(size_t label) const
	{
		switch (label)
		{
		case JOB:
			return Point<int16_t>(72, -48);
		case WORLDRANK:
			return Point<int16_t>(72, -24);
		case JOBRANK:
			return Point<int16_t>(72, -4);
		case STR:
			return Point<int16_t>(-5, 26);
		case DEX:
			return Point<int16_t>(-5, 48);
		case INT:
			return Point<int16_t>(72, 26);
		case LUK:
			return Point<int16_t>(72, 48);
		default:
			return Point<int16_t>();
		}
	}

	Point<int16_t> UICharSelect::getcharpos(size_t i) const
	{
		int16_t x = 130 + (120 * (i % 4));
		int16_t y = 250 + (200 * (i > 3));
		return Point<int16_t>(x, y);
	}
}
