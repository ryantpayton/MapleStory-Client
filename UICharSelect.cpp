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
#include "nx.hpp"

namespace IO
{
	UICharSelect::UICharSelect(Equipcache* eq, UI* u, Login* lg, Session* ses)
	{
		equips = eq;
		ui = u;
		login = lg;
		session = ses;

		node charsel = nx::ui["Login.img"]["CharSelect"];

		graphics.add(new Sprite(nx::ui["Login.img"]["Title"]["worldsel"], vector2d<int>()));
		graphics.add(new Sprite(nx::ui["Login.img"]["Common"]["frame"], vector2d<int>(400, 290)));
		graphics.add(new Sprite(nx::ui["Login.img"]["Common"]["selectWorld"], vector2d<int>(580, 42)));
		graphics.add(new Sprite(charsel["selectedWorld"]["icon"]["15"], vector2d<int>(580, 42)));
		graphics.add(new Sprite(charsel["selectedWorld"]["name"]["15"], vector2d<int>(580, 42)));
		graphics.add(new Sprite(charsel["selectedWorld"]["ch"][to_string(login->getchannelid())], vector2d<int>(580, 42)));
		graphics.add(new Sprite(charsel["charInfo"], vector2d<int>(662, 355)));

		buttons.add(BT_ARBEIT, new MapleButton(charsel["arbeit"], vector2d<int>(580, 115)));
		buttons.add(BT_CARDS, new MapleButton(charsel["characterCard"], vector2d<int>(665, 115)));
		buttons.add(BT_CREATECHAR, new MapleButton(charsel["BtNew"], vector2d<int>(200, 495)));
		buttons.add(BT_DELETECHAR, new MapleButton(charsel["BtDelete"], vector2d<int>(320, 495)));
		buttons.add(BT_SELECTCHAR, new MapleButton(charsel["BtSelect"], vector2d<int>(586, 427)));
		buttons.add(BT_PAGELEFT, new MapleButton(charsel["pageL"], vector2d<int>(100, 490)));
		buttons.add(BT_PAGERIGHT, new MapleButton(charsel["pageR"], vector2d<int>(490, 490)));

		Account* account = login->getaccount();
		char cslots = account->getslots();
		size_t charcount = account->getchars()->size();
		if (charcount + cslots < 9)
		{
			buttons.get(BT_PAGELEFT)->setstate(BTS_DISABLED);
			buttons.get(BT_PAGERIGHT)->setstate(BTS_DISABLED);
		}

		size_t createcount = cslots;
		if (createcount > 8)
		{
			createcount = 8;
		}
		for (size_t i = charcount; i < createcount; i++)
		{
			graphics.add(new Sprite(charsel["buyCharacter"], vector2d<int>(130 + (120 * (i % 4)), 250 + (200 * (i > 3)))));
		}

		/*map<bool, vector<texture>> nttextures;
		nttextures[false].push_back(texture(charsel["nameTag"]["0"]["0"]));
		nttextures[false].push_back(texture(charsel["nameTag"]["0"]["1"]));
		nttextures[false].push_back(texture(charsel["nameTag"]["0"]["2"]));
		nttextures[true].push_back(texture(charsel["nameTag"]["1"]["0"]));
		nttextures[true].push_back(texture(charsel["nameTag"]["1"]["1"]));
		nttextures[true].push_back(texture(charsel["nameTag"]["1"]["2"]));*/

		//selected = config.getconfig()->defaultchar;
		selected = 0;
		if (selected >= charcount)
		{
			selected = 0;
		}
		page = selected % 8;

		//lvset = charset(charsel["lv"]);
		//statset = charset(nx::ui["StatusBar2.img"]["mainBar"]["gauge"]["number"]);

		size_t displaycount = charcount;
		if (displaycount > 8)
		{
			displaycount = 8;
		}
		for (size_t i = 0; i < displaycount; i++)
		{
			addchar(i);
		}

		if (charcount > 0)
		{
			StatsEntry* stats = account->getchars()->at(selected).getstats();
			namelabel = new Textlabel(DWF_20MC, TXC_WHITE, stats->getname(), 0);
			//joblabel = new Textlabel(DWF_12MR, TXC_WHITE, stats->getjobname(), 0);
		}

		position = vector2d<int>(0, 0);
		dimension = vector2d<int>(800, 600);
		active = true;
	}

	UICharSelect::~UICharSelect()
	{
		delete namelabel;
		delete joblabel;
	}

	void UICharSelect::draw()
	{
		UIElement::draw();

		if (active)
		{
			char i = 0;
			for (vector<CharLook>::iterator chit = charlooks.begin(); chit != charlooks.end(); ++chit)
			{
				chit->draw(getcharpos(i));
				i++;
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

	void UICharSelect::buttonpressed(short id)
	{

	}

	void UICharSelect::addchar(size_t index)
	{
		CharLook look = CharLook(login->getaccount()->getchars()->at(index).getlook());
		equips->loadlook(&look);

		short buttonindex = static_cast<short>(BT_CHAR0 + index);
		buttons.add(buttonindex, new AreaButton(vector2d<int>(105 + (120 * (index % 4)), 170 + (200 * (index > 3))), vector2d<int>(50, 80)));
		if (index == selected)
		{
			look.setstance("walk");
			buttons.get(buttonindex)->setstate(BTS_PRESSED);
		}
		//nametags.push_back(nametag(DWF_14C, TXC_WHITE, nttextures, chars[i]->getstats()->getname(), vector2d(133 + (120 * (i % 4)), 250 + (200 * (i > 3))), (i == selected)));
		charlooks.push_back(look);
	}

	vector2d<int> UICharSelect::getcharpos(char i)
	{
		return vector2d<int>(130 + (120 * (i % 4)), 250 + (200 * (i > 3)));
	}
}
