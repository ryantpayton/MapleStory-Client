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
#include "Program\TimeConstants.h"
#include "IO\Components\MapleButton.h"
#include "IO\Components\AreaButton.h"
#include "Graphics\Sprite.h"
#include "Net\Packets\SelectCharPackets83.h"
#include "nlnx\nx.hpp"

#define button_ptr unique_ptr<Button>

namespace IO
{
	UICharSelect::UICharSelect(UIInterface& u, Session& ses) : ui(u), session(ses)
	{
		node charsel = nl::nx::ui["Login.img"]["CharSelect"];

		using::Graphics::Sprite;
		sprites.push_back(Sprite(nl::nx::ui["Login.img"]["Title"]["worldsel"], vector2d<int32_t>()));
		sprites.push_back(Sprite(nl::nx::ui["Login.img"]["Common"]["frame"], vector2d<int32_t>(400, 290)));
		sprites.push_back(Sprite(nl::nx::ui["Login.img"]["Common"]["selectWorld"], vector2d<int32_t>(580, 42)));
		sprites.push_back(Sprite(charsel["selectedWorld"]["icon"]["15"], vector2d<int32_t>(580, 42)));
		sprites.push_back(Sprite(charsel["selectedWorld"]["name"]["15"], vector2d<int32_t>(580, 42)));
		sprites.push_back(Sprite(charsel["selectedWorld"]["ch"][std::to_string(session.getlogin().getchannelid())], vector2d<int32_t>(580, 42)));
		sprites.push_back(Sprite(charsel["charInfo"], vector2d<int32_t>(662, 355)));

		buttons[BT_ARBEIT] = button_ptr(new MapleButton(charsel["arbeit"], vector2d<int32_t>(580, 115)));
		buttons[BT_CARDS] = button_ptr(new MapleButton(charsel["characterCard"], vector2d<int32_t>(665, 115)));
		buttons[BT_CREATECHAR] = button_ptr(new MapleButton(charsel["BtNew"], vector2d<int32_t>(200, 495)));
		buttons[BT_DELETECHAR] = button_ptr(new MapleButton(charsel["BtDelete"], vector2d<int32_t>(320, 495)));
		buttons[BT_SELECTCHAR] = button_ptr(new MapleButton(charsel["BtSelect"], vector2d<int32_t>(586, 427)));
		buttons[BT_PAGELEFT] = button_ptr(new MapleButton(charsel["pageL"], vector2d<int32_t>(100, 490)));
		buttons[BT_PAGERIGHT] = button_ptr(new MapleButton(charsel["pageR"], vector2d<int32_t>(490, 490)));

		levelset = Charset(charsel["lv"], Charset::CENTER);
		statsset = Charset(nl::nx::ui["StatusBar2.img"]["mainBar"]["gauge"]["number"], Charset::RIGHT);

		using::Net::Account;
		const Account& account = session.getlogin().getaccount();
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
			sprites.push_back(Sprite(charsel["buyCharacter"], vector2d<int32_t>(130 + (120 * (i % 4)), 250 + (200 * (i > 3)))));
		}
		//selected = config.getconfig()->defaultchar;
		selected = 0;
		if (selected >= charcount)
		{
			selected = 0;
		}
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
			using::Net::StatsEntry;
			const StatsEntry& stats = account.getchar(selected).getstats();
			namelabel = Textlabel(Textlabel::DWF_20MC, Textlabel::TXC_WHITE, stats.getname(), 0);
			joblabel = Textlabel(Textlabel::DWF_12MR, Textlabel::TXC_WHITE, stats.getjobname(), 0);
		}

		position = vector2d<int32_t>(0, 0);
		dimension = vector2d<int32_t>(800, 600);
		active = true;
	}

	void UICharSelect::draw(float inter) const
	{
		UIElement::draw(inter);

		if (active)
		{
			for (uint8_t i = 0; i < charcount; i++)
			{
				vector2d<int32_t> charpos = getcharpos(i);
				charlooks[i].draw(charpos, inter);
				nametags[i].draw(charpos);
			}

			namelabel.draw(vector2d<int32_t>(662, 268));
			joblabel.draw(vector2d<int32_t>(732, 305));

			if (selected < charcount)
			{
				using::Net::StatsEntry;
				const StatsEntry& stats = session.getlogin().getaccount().getchar(selected).getstats();

				using::Graphics::DrawArgument;
				statsset.draw(std::to_string(stats.getrank().first), DrawArgument(732, 335));
				statsset.draw(std::to_string(stats.getjobrank().first), DrawArgument(732, 355));

				int32_t lvx = levelset.draw(std::to_string(stats.getstat(Character::MS_LEVEL)), DrawArgument(685, 262));
				levelset.draw('l', DrawArgument(655 - lvx / 2, 262));

				statsset.draw(std::to_string(stats.getstat(Character::MS_STR)), DrawArgument(655, 385));
				statsset.draw(std::to_string(stats.getstat(Character::MS_DEX)), DrawArgument(655, 407));
				statsset.draw(std::to_string(stats.getstat(Character::MS_INT)), DrawArgument(732, 385));
				statsset.draw(std::to_string(stats.getstat(Character::MS_LUK)), DrawArgument(732, 407));
			}
		}
	}

	void UICharSelect::update()
	{
		UIElement::update();

		if (active)
		{
			for (vector<CharLook>::iterator chit = charlooks.begin(); chit != charlooks.end(); ++chit)
			{
				chit->update(Constants::TIMESTEP);
			}
		}
	}

	void UICharSelect::buttonpressed(uint16_t bid)
	{
		if (bid >= BT_CHAR0)
		{
			buttons[BT_CHAR0 + selected]->setstate(Button::NORMAL);
			nametags[selected].setselected(false);
			charlooks[selected].setstance("stand");

			selected = bid - BT_CHAR0;
			nametags[selected].setselected(true);
			charlooks[selected].setstance("walk");

			using::Net::StatsEntry;
			const StatsEntry& stats = session.getlogin().getaccount().getchar(selected).getstats();
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
				ui.remove(UI_CHARCREATION);
				ui.add(ElementCharcreation(session, ui));
				break;
			}

			buttons[bid]->setstate(Button::MOUSEOVER);
		}
	}

	void UICharSelect::selectchar()
	{
		if (selected < charcount)
		{
			int32_t cid = session.getlogin().getaccount().getchar(selected).getcid();
			session.getlogin().setcharid(cid);
			switch (session.getlogin().getaccount().getpic())
			{
			case 0:
				ui.add(ElementSoftkey(UISoftkey::REGISTER, ui, session));
				break;
			case 1:
				ui.add(ElementSoftkey(UISoftkey::CHARSELECT, ui, session));
				break;
			case 2:
				ui.disable();

				using::Net::SelectCharPacket83;
				session.dispatch(SelectCharPacket83(cid));
				break;
			}
		}
	}

	void UICharSelect::addchar(uint8_t index)
	{
		CharLook look = CharLook(session.getlogin().getaccount().getchar(index).getlook());
		short buttonindex = BT_CHAR0 + index;
		buttons[buttonindex] = button_ptr(new AreaButton(
			vector2d<int32_t>(105 + (120 * (index % 4)), 170 + (200 * (index > 3))), 
			vector2d<int32_t>(50, 80)
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

	vector2d<int32_t> UICharSelect::getcharpos(size_t i) const
	{
		return vector2d<int32_t>(130 + (120 * (i % 4)), 250 + (200 * (i > 3)));
	}
}
