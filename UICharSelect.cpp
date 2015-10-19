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
#pragma once
#include "UICharSelect.h"
#include "MapleButton.h"
#include "AreaButton.h"
#include "Sprite.h"
#include "SelectCharPackets.h"
#include "UISoftkey.h"
#include "nx.hpp"

namespace IO
{
	UICharSelect::UICharSelect(Equipcache& eq, UI& u, Login& lg, Session& ses) : equips(eq), ui(u), login(lg), session(ses)
	{
		node charsel = nx::ui["Login.img"]["CharSelect"];

		graphics.add(new Sprite(nx::ui["Login.img"]["Title"]["worldsel"], vector2d<int>()));
		graphics.add(new Sprite(nx::ui["Login.img"]["Common"]["frame"], vector2d<int>(400, 290)));
		graphics.add(new Sprite(nx::ui["Login.img"]["Common"]["selectWorld"], vector2d<int>(580, 42)));
		graphics.add(new Sprite(charsel["selectedWorld"]["icon"]["15"], vector2d<int>(580, 42)));
		graphics.add(new Sprite(charsel["selectedWorld"]["name"]["15"], vector2d<int>(580, 42)));
		graphics.add(new Sprite(charsel["selectedWorld"]["ch"][to_string(login.getchannelid())], vector2d<int>(580, 42)));
		graphics.add(new Sprite(charsel["charInfo"], vector2d<int>(662, 355)));

		buttons.add(BT_ARBEIT, new MapleButton(charsel["arbeit"], vector2d<int>(580, 115)));
		buttons.add(BT_CARDS, new MapleButton(charsel["characterCard"], vector2d<int>(665, 115)));
		buttons.add(BT_CREATECHAR, new MapleButton(charsel["BtNew"], vector2d<int>(200, 495)));
		buttons.add(BT_DELETECHAR, new MapleButton(charsel["BtDelete"], vector2d<int>(320, 495)));
		buttons.add(BT_SELECTCHAR, new MapleButton(charsel["BtSelect"], vector2d<int>(586, 427)));
		buttons.add(BT_PAGELEFT, new MapleButton(charsel["pageL"], vector2d<int>(100, 490)));
		buttons.add(BT_PAGERIGHT, new MapleButton(charsel["pageR"], vector2d<int>(490, 490)));

		levelset = new Charset(charsel["lv"], CHA_CENTER);
		statsset = new Charset(nx::ui["StatusBar2.img"]["mainBar"]["gauge"]["number"], CHA_RIGHT);

		Account& account = login.getaccount();
		charcount = (char)account.getcharcount();
		if (charcount + account.getslots() < 9)
		{
			buttons.get(BT_PAGELEFT)->setstate(BTS_DISABLED);
			buttons.get(BT_PAGERIGHT)->setstate(BTS_DISABLED);
		}
		char createcount = account.getslots();
		if (createcount > 8)
		{
			createcount = 8;
		}
		for (char i = charcount; i < createcount; i++)
		{
			graphics.add(new Sprite(charsel["buyCharacter"], vector2d<int>(130 + (120 * (i % 4)), 250 + (200 * (i > 3)))));
		}
		//selected = config.getconfig()->defaultchar;
		selected = 0;
		if (selected >= charcount)
		{
			selected = 0;
		}
		page = selected % 8;
		char displaycount = (char)account.getcharcount();
		if (displaycount > 8)
		{
			displaycount = 8;
		}
		for (char i = 0; i < displaycount; i++)
		{
			addchar(i);
			nametags.add(new Nametag(charsel["nameTag"], DWF_14MC, TXC_WHITE, account.getchar(i).getstats().getname()));
		}
		nametags.get(selected)->setselected(true);
		if (charcount > 0)
		{
			const StatsEntry& stats = account.getchar(selected).getstats();
			namelabel = new Textlabel(DWF_20MC, TXC_WHITE, stats.getname(), 0);
			joblabel = new Textlabel(DWF_12MR, TXC_WHITE, stats.getjobname(), 0);
		}

		position = vector2d<int>(0, 0);
		dimension = vector2d<int>(800, 600);
		active = true;
	}

	UICharSelect::~UICharSelect()
	{
		delete namelabel;
		delete joblabel;
		delete levelset;
		delete statsset;
	}

	void UICharSelect::draw()
	{
		UIElement::draw();

		if (active)
		{
			for (char i = 0; i < charcount; i++)
			{
				vector2d<int> charpos = getcharpos(i);
				charlooks[i].draw(charpos);
				nametags.get(i)->draw(charpos);
			}
			if (selected < charcount)
			{
				const StatsEntry& stats = login.getaccount().getchar(selected).getstats();
				namelabel->draw(vector2d<int>(662, 268));
				joblabel->draw(vector2d<int>(732, 305));
				statsset->draw(to_string(stats.getrank().first), PosArgument(vector2d<int>(732, 335)));
				statsset->draw(to_string(stats.getjobrank().first), PosArgument(vector2d<int>(732, 355)));
				int lvx = levelset->draw(to_string(stats.getstat(MS_LEVEL)), PosArgument(vector2d<int>(685, 262)));
				levelset->draw('l', PosArgument(vector2d<int>(655 - lvx / 2, 262)));
				statsset->draw(to_string(stats.getstat(MS_STR)), PosArgument(vector2d<int>(655, 385)));
				statsset->draw(to_string(stats.getstat(MS_DEX)), PosArgument(vector2d<int>(655, 407)));
				statsset->draw(to_string(stats.getstat(MS_INT)), PosArgument(vector2d<int>(732, 385)));
				statsset->draw(to_string(stats.getstat(MS_LUK)), PosArgument(vector2d<int>(732, 407)));
			}
		}
	}

	void UICharSelect::update(short dpf)
	{
		UIElement::update(dpf);

		if (active)
		{
			for (vector<CharLook>::iterator chit = charlooks.begin(); chit != charlooks.end(); ++chit)
			{
				chit->update(dpf);
			}
		}
	}

	void UICharSelect::buttonpressed(short bid)
	{
		if (bid >= BT_CHAR0)
		{
			buttons.get(BT_CHAR0 + selected)->setstate(BTS_NORMAL);
			nametags.get(selected)->setselected(false);
			charlooks[selected].setstance("stand");

			selected = bid - BT_CHAR0;
			nametags.get(selected)->setselected(true);
			charlooks[selected].setstance("walk");
			namelabel->settext(login.getaccount().getchar(selected).getstats().getname());
			joblabel->settext(login.getaccount().getchar(selected).getstats().getjobname());
		}
		else
		{
			switch (bid)
			{
			case BT_SELECTCHAR:
				selectchar();
				break;
			case BT_CREATECHAR:
				break;
			}

			buttons.get(bid)->setstate(BTS_MOUSEOVER);
		}
	}

	void UICharSelect::selectchar()
	{
		if (selected < charcount)
		{
			int cid = login.getaccount().getchar(selected).getcid();
			login.setcharid(cid);
			switch (login.getaccount().getpic())
			{
			case 0:
				ui.add(ElementSoftkey(SFTK_REGISTER, ui, login, session));
				break;
			case 1:
				ui.add(ElementSoftkey(SFTK_CHARSELECT, ui, login, session));
				break;
			case 2:
				ui.disable();
				session.dispatch(SelectCharPacket(cid));
				break;
			}
		}
	}

	void UICharSelect::addchar(char index)
	{
		CharLook look;
		equips.loadlook(look, login.getaccount().getchar(index).getlook());

		short buttonindex = BT_CHAR0 + index;
		buttons.add(buttonindex, new AreaButton(vector2d<int>(105 + (120 * (index % 4)), 170 + (200 * (index > 3))), vector2d<int>(50, 80)));
		if (index == selected)
		{
			look.setstance("walk");
			buttons.get(buttonindex)->setstate(BTS_PRESSED);
		}
		charlooks.push_back(look);
	}

	void UICharSelect::removechar(char index)
	{
		nametags.remove(index);
		buttons.remove(index);
		charlooks.erase(charlooks.begin() + index);
	}

	vector2d<int> UICharSelect::getcharpos(size_t i)
	{
		return vector2d<int>(130 + (120 * (i % 4)), 250 + (200 * (i > 3)));
	}
}
