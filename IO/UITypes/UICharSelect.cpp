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
#pragma once
#include "UICharSelect.h"
#include "UISoftkey.h"
#include "UICharcreation.h"
#include "IO\UI.h"
#include "Net\Session.h"
#include "Program\Constants.h"
#include "IO\Components\MapleButton.h"
#include "IO\Components\AreaButton.h"
#include "Graphics\Sprite.h"
#include "Net\Packets\SelectCharPackets83.h"
#include "Program\Configuration.h"
#include "nlnx\nx.hpp"

namespace IO
{
	UICharSelect::UICharSelect()
	{
		node charsel = nl::nx::ui["Login.img"]["CharSelect"];

		using Graphics::Sprite;
		sprites.push_back(Sprite(nl::nx::ui["Login.img"]["Title"]["worldsel"]));
		sprites.push_back(Sprite(nl::nx::ui["Login.img"]["Common"]["frame"], vector2d<int16_t>(400, 290)));
		sprites.push_back(Sprite(nl::nx::ui["Login.img"]["Common"]["selectWorld"], vector2d<int16_t>(580, 42)));
		sprites.push_back(Sprite(charsel["selectedWorld"]["icon"]["15"], vector2d<int16_t>(580, 42)));
		sprites.push_back(Sprite(charsel["selectedWorld"]["name"]["15"], vector2d<int16_t>(580, 42)));

		string channelid = std::to_string(Net::Session::getlogin().getchannelid());
		sprites.push_back(Sprite(charsel["selectedWorld"]["ch"][channelid], vector2d<int16_t>(580, 42)));
		sprites.push_back(Sprite(charsel["charInfo"], vector2d<int16_t>(662, 355)));

		buttons[BT_ARBEIT] = unique_ptr<Button>(new MapleButton(charsel["arbeit"], vector2d<int16_t>(580, 115)));
		buttons[BT_CARDS] = unique_ptr<Button>(new MapleButton(charsel["characterCard"], vector2d<int16_t>(665, 115)));
		buttons[BT_CREATECHAR] = unique_ptr<Button>(new MapleButton(charsel["BtNew"], vector2d<int16_t>(200, 495)));
		buttons[BT_DELETECHAR] = unique_ptr<Button>(new MapleButton(charsel["BtDelete"], vector2d<int16_t>(320, 495)));
		buttons[BT_SELECTCHAR] = unique_ptr<Button>(new MapleButton(charsel["BtSelect"], vector2d<int16_t>(586, 427)));
		buttons[BT_PAGELEFT] = unique_ptr<Button>(new MapleButton(charsel["pageL"], vector2d<int16_t>(100, 490)));
		buttons[BT_PAGERIGHT] = unique_ptr<Button>(new MapleButton(charsel["pageR"], vector2d<int16_t>(490, 490)));

		levelset = Charset(charsel["lv"], Charset::CENTER);
		statsset = Charset(nl::nx::ui["StatusBar2.img"]["mainBar"]["gauge"]["number"], Charset::RIGHT);

		using::Net::Account;
		const Account& account = Net::Session::getlogin().getaccount();
		charcount = static_cast<uint8_t>(account.getcharcount());

		if (charcount + account.getslots() < 9)
		{
			buttons[BT_PAGELEFT]->setstate(Button::DISABLED);
			buttons[BT_PAGERIGHT]->setstate(Button::DISABLED);
		}
		uint8_t createcount = account.getslots();
		if (createcount > 8)
		{
			createcount = 8;
		}
		for (uint8_t i = charcount; i < createcount; i++)
		{
			sprites.push_back(Sprite(
				charsel["buyCharacter"], 
				vector2d<int16_t>(130 + (120 * (i % 4)), 250 + (200 * (i > 3))))
				);
		}

		selected = Program::Configuration::getbyte("Character");
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
			nametags.push_back(
				Nametag(charsel["nameTag"], 
				Textlabel::DWF_14MC, Textlabel::TXC_WHITE, 
				account.getchar(i).getstats().getname()
				));
		}
		nametags[selected].setselected(true);

		if (charcount > 0)
		{
			const Net::StatsEntry& stats = account.getchar(selected).getstats();
			namelabel = Textlabel(Textlabel::DWF_20MC, Textlabel::TXC_WHITE, stats.getname(), 0);
			joblabel = Textlabel(Textlabel::DWF_12MR, Textlabel::TXC_WHITE, stats.getjobname(), 0);
		}

		position = vector2d<int16_t>(0, 0);
		dimension = vector2d<int16_t>(800, 600);
		active = true;
	}

	void UICharSelect::draw(float inter) const
	{
		UIElement::draw(inter);

		for (uint8_t i = 0; i < charcount; i++)
		{
			vector2d<int16_t> charpos = getcharpos(i);
			charlooks[i].draw(charpos, inter);
			nametags[i].draw(charpos);
		}

		namelabel.draw(vector2d<int16_t>(662, 268));
		joblabel.draw(vector2d<int16_t>(732, 305));

		if (selected < charcount)
		{
			const Net::StatsEntry& stats = Net::Session::getlogin().getaccount().getchar(selected).getstats();

			using::Graphics::DrawArgument;
			statsset.draw(std::to_string(stats.getrank().first), DrawArgument(732, 335));
			statsset.draw(std::to_string(stats.getjobrank().first), DrawArgument(732, 355));

			int16_t lvx = levelset.draw(std::to_string(stats.getstat(Character::MS_LEVEL)), DrawArgument(685, 262));
			levelset.draw('l', DrawArgument(655 - lvx / 2, 262));

			statsset.draw(std::to_string(stats.getstat(Character::MS_STR)), DrawArgument(655, 385));
			statsset.draw(std::to_string(stats.getstat(Character::MS_DEX)), DrawArgument(655, 407));
			statsset.draw(std::to_string(stats.getstat(Character::MS_INT)), DrawArgument(732, 385));
			statsset.draw(std::to_string(stats.getstat(Character::MS_LUK)), DrawArgument(732, 407));
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
			charlooks[selected].setstance("stand");

			selected = static_cast<uint8_t>(bid - BT_CHAR0);
			nametags[selected].setselected(true);
			charlooks[selected].setstance("walk");

			const Net::StatsEntry& stats = Net::Session::getlogin().getaccount().getchar(selected).getstats();
			namelabel.settext(stats.getname(), 0);
			joblabel.settext(stats.getjobname(), 0);
		}
		else
		{
			switch (bid)
			{
			case BT_SELECTCHAR:
				selectchar();
				break;
			case BT_CREATECHAR:
				active = false;
				UI::remove(Element::CHARCREATION);
				UI::add(ElementCharcreation());
				break;
			}

			buttons[bid]->setstate(Button::MOUSEOVER);
		}
	}

	void UICharSelect::selectchar()
	{
		if (selected < charcount)
		{
			Program::Configuration::setint("Character", selected);
			int32_t cid = Net::Session::getlogin().getaccount().getchar(selected).getcid();
			Net::Session::getlogin().setcharid(cid);
			switch (Net::Session::getlogin().getaccount().getpic())
			{
			case 0:
				UI::add(ElementSoftkey(UISoftkey::REGISTER));
				break;
			case 1:
				UI::add(ElementSoftkey(UISoftkey::CHARSELECT));
				break;
			case 2:
				UI::disable();
				Net::Session::dispatch(Net::SelectCharPacket83(cid));
				break;
			}
		}
	}

	void UICharSelect::addchar(uint8_t index)
	{
		CharLook look = CharLook(Net::Session::getlogin().getaccount().getchar(index).getlook());
		short buttonindex = BT_CHAR0 + index;
		buttons[buttonindex] = unique_ptr<Button>(new AreaButton(
			vector2d<int16_t>(105 + (120 * (index % 4)), 170 + (200 * (index > 3))),
			vector2d<int16_t>(50, 80)
			));
		if (index == selected)
		{
			look.setstance("walk");
			buttons[buttonindex]->setstate(Button::PRESSED);
		}
		charlooks.push_back(look);
	}

	void UICharSelect::removechar(uint8_t index)
	{
		charlooks.erase(charlooks.begin() + index);
		nametags.erase(nametags.begin() + index);
		buttons[BT_CHAR0 + index].release();
		buttons.erase(BT_CHAR0 + index);
	}

	vector2d<int16_t> UICharSelect::getcharpos(size_t i) const
	{
		return vector2d<int16_t>(130 + (120 * (i % 4)), 250 + (200 * (i > 3)));
	}
}
