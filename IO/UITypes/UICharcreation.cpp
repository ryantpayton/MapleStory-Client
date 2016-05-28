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
#include "UICharcreation.h"

#include "..\UI.h"
#include "..\Components\MapleButton.h"
#include "..\UITypes\UILoginNotice.h"
#include "..\UITypes\UICharSelect.h"

#include "..\..\Constants.h"
#include "..\..\Net\Session.h"
#include "..\..\Net\Packets\CharCreationPackets.h"

#include "nlnx\nx.hpp"

namespace jrc
{
	UICharcreation::UICharcreation()
	{
		female = Session::get().getlogin().getaccount().female;

		nl::node src = nl::nx::ui["Login.img"];
		nl::node bgsrc = nl::nx::map["Back"]["login.img"]["back"];
		nl::node crsrc = src["NewChar"];

		sky = bgsrc["2"];
		cloud = bgsrc["27"];

		sprites.emplace_back(bgsrc["15"], Point<int16_t>(153, 685));
		sprites.emplace_back(bgsrc["16"], Point<int16_t>(200, 400));
		sprites.emplace_back(bgsrc["17"], Point<int16_t>(160, 263));
		sprites.emplace_back(bgsrc["18"], Point<int16_t>(349, 1220));
		sprites.emplace_back(src["Common"]["frame"], Point<int16_t>(400, 290));

		nameboard = crsrc["charName"];

		lookboard.emplace_back(crsrc["charSet"], Point<int16_t>(450, 115));
		lookboard.emplace_back(crsrc["avatarSel"]["0"]["normal"], Point<int16_t>(461, 217));
		lookboard.emplace_back(crsrc["avatarSel"]["1"]["normal"], Point<int16_t>(461, 236));
		lookboard.emplace_back(crsrc["avatarSel"]["2"]["normal"], Point<int16_t>(461, 255));
		lookboard.emplace_back(crsrc["avatarSel"]["3"]["normal"], Point<int16_t>(461, 274));
		lookboard.emplace_back(crsrc["avatarSel"]["4"]["normal"], Point<int16_t>(461, 293));
		lookboard.emplace_back(crsrc["avatarSel"]["5"]["normal"], Point<int16_t>(461, 312));
		lookboard.emplace_back(crsrc["avatarSel"]["6"]["normal"], Point<int16_t>(461, 331));
		lookboard.emplace_back(crsrc["avatarSel"]["7"]["normal"], Point<int16_t>(461, 350));
		lookboard.emplace_back(crsrc["avatarSel"]["8"]["normal"], Point<int16_t>(461, 369));

		buttons[BT_CHARC_OK] = std::make_unique<MapleButton>(crsrc["BtYes"], Point<int16_t>(482, 292));
		buttons[BT_CHARC_CANCEL] = std::make_unique<MapleButton>(crsrc["BtNo"], Point<int16_t>(555, 292));
		buttons[BT_CHARC_FACEL] = std::make_unique<MapleButton>(crsrc["BtLeft"], Point<int16_t>(521, 216));
		buttons[BT_CHARC_FACER] = std::make_unique<MapleButton>(crsrc["BtRight"], Point<int16_t>(645, 216));
		buttons[BT_CHARC_HAIRL] = std::make_unique<MapleButton>(crsrc["BtLeft"], Point<int16_t>(521, 235));
		buttons[BT_CHARC_HAIRR] = std::make_unique<MapleButton>(crsrc["BtRight"], Point<int16_t>(645, 235));
		buttons[BT_CHARC_HAIRCL] = std::make_unique<MapleButton>(crsrc["BtLeft"], Point<int16_t>(521, 254));
		buttons[BT_CHARC_HAIRCR] = std::make_unique<MapleButton>(crsrc["BtRight"], Point<int16_t>(645, 254));
		buttons[BT_CHARC_SKINL] = std::make_unique<MapleButton>(crsrc["BtLeft"], Point<int16_t>(521, 273));
		buttons[BT_CHARC_SKINR] = std::make_unique<MapleButton>(crsrc["BtRight"], Point<int16_t>(645, 273));
		buttons[BT_CHARC_TOPL] = std::make_unique<MapleButton>(crsrc["BtLeft"], Point<int16_t>(521, 292));
		buttons[BT_CHARC_TOPR] = std::make_unique<MapleButton>(crsrc["BtRight"], Point<int16_t>(645, 292));
		buttons[BT_CHARC_BOTL] = std::make_unique<MapleButton>(crsrc["BtLeft"], Point<int16_t>(521, 311));
		buttons[BT_CHARC_BOTR] = std::make_unique<MapleButton>(crsrc["BtRight"], Point<int16_t>(645, 311));
		buttons[BT_CHARC_SHOESL] = std::make_unique<MapleButton>(crsrc["BtLeft"], Point<int16_t>(521, 330));
		buttons[BT_CHARC_SHOESR] = std::make_unique<MapleButton>(crsrc["BtRight"], Point<int16_t>(645, 330));
		buttons[BT_CHARC_WEPL] = std::make_unique<MapleButton>(crsrc["BtLeft"], Point<int16_t>(521, 349));
		buttons[BT_CHARC_WEPR] = std::make_unique<MapleButton>(crsrc["BtRight"], Point<int16_t>(645, 348));
		buttons[BT_CHARC_GENDERL] = std::make_unique<MapleButton>(crsrc["BtLeft"], Point<int16_t>(521, 368));
		buttons[BT_CHARC_GEMDERR] = std::make_unique<MapleButton>(crsrc["BtRight"], Point<int16_t>(645, 368));

		buttons[BT_CHARC_FACEL]->setactive(false);
		buttons[BT_CHARC_FACER]->setactive(false);
		buttons[BT_CHARC_HAIRL]->setactive(false);
		buttons[BT_CHARC_HAIRR]->setactive(false);
		buttons[BT_CHARC_HAIRCL]->setactive(false);
		buttons[BT_CHARC_HAIRCR]->setactive(false);
		buttons[BT_CHARC_SKINL]->setactive(false);
		buttons[BT_CHARC_SKINR]->setactive(false);
		buttons[BT_CHARC_TOPL]->setactive(false);
		buttons[BT_CHARC_TOPR]->setactive(false);
		buttons[BT_CHARC_BOTL]->setactive(false);
		buttons[BT_CHARC_BOTR]->setactive(false);
		buttons[BT_CHARC_SHOESL]->setactive(false);
		buttons[BT_CHARC_SHOESR]->setactive(false);
		buttons[BT_CHARC_WEPL]->setactive(false);
		buttons[BT_CHARC_WEPR]->setactive(false);
		buttons[BT_CHARC_GENDERL]->setactive(false);
		buttons[BT_CHARC_GEMDERR]->setactive(false);

		Rectangle<int16_t> txfae = Rectangle<int16_t>(
			Point<int16_t>(490, 219),
			Point<int16_t>(630, 243)
			);
		namechar = Textfield(Text::A13M, Text::LEFT, Text::WHITE, txfae, 12);
		namechar.setstate(Textfield::FOCUSED);

		UI::get().focustextfield(&namechar);

		facename = Text(Text::A11M, Text::CENTER, Text::BLACK);
		hairname = Text(Text::A11M, Text::CENTER, Text::BLACK);
		haircname = Text(Text::A11M, Text::CENTER, Text::BLACK);
		bodyname = Text(Text::A11M, Text::CENTER, Text::BLACK);
		topname = Text(Text::A11M, Text::CENTER, Text::BLACK);
		botname = Text(Text::A11M, Text::CENTER, Text::BLACK);
		shoename = Text(Text::A11M, Text::CENTER, Text::BLACK);
		wepname = Text(Text::A11M, Text::CENTER, Text::BLACK);
		gendername = Text(Text::A11M, Text::CENTER, Text::BLACK);

		nl::node mkinfo = nl::nx::etc["MakeCharInfo.img"]["Info"];
		for (int32_t i = 0; i < 2; i++)
		{
			bool f;
			nl::node mk_n;
			if (i == 0)
			{
				f = true;
				mk_n = mkinfo["CharFemale"];
			}
			else
			{
				f = false;
				mk_n = mkinfo["CharMale"];
			}

			for (auto subnode : mk_n)
			{
				int num = stoi(subnode.name());
				for (auto idnode : subnode)
				{
					int value = idnode;
					switch (num)
					{
					case 0:
						faces[f].push_back(value);
						break;
					case 1:
						hairs[f].push_back(value);
						break;
					case 2:
						haircolors[f].push_back(static_cast<uint8_t>(value));
						break;
					case 3:
						skins[f].push_back(static_cast<uint8_t>(value));
						break;
					case 4:
						tops[f].push_back(value);
						break;
					case 5:
						bots[f].push_back(value);
						break;
					case 6:
						shoes[f].push_back(value);
						break;
					case 7:
						weapons[f].push_back(value);
						break;
					}
				}
			}
		}

		randomizelook();

		newchar.setflip(true);

		position = Point<int16_t>(0, 0);
		dimension = Point<int16_t>(800, 600);
		active = true;
		cloudfx = 200.0f;
	}

	void UICharcreation::randomizelook()
	{
		hair = randomizer.nextint(0, hairs[female].size() - 1);
		face = randomizer.nextint(0, faces[female].size() - 1);
		skin = randomizer.nextint(0, skins[female].size() - 1);
		haircolor = randomizer.nextint(0, haircolors[female].size() - 1);
		top = randomizer.nextint(0, tops[female].size() - 1);
		bot = randomizer.nextint(0, bots[female].size() - 1);
		shoe = randomizer.nextint(0, shoes[female].size() - 1);
		weapon = randomizer.nextint(0, weapons[female].size() - 1);

		newchar.setbody(skins[female][skin]);
		newchar.setface(faces[female][face]);
		newchar.sethair(hairs[female][hair] + haircolors[female][haircolor]);
		newchar.addequip(tops[female][top]);
		newchar.addequip(bots[female][bot]);
		newchar.addequip(shoes[female][shoe]);
		newchar.addequip(weapons[female][weapon]);

		bodyname.settext(newchar.getbodytype()->getname());
		facename.settext(newchar.getfacetype()->getname());
		hairname.settext(newchar.gethairstyle()->getname());
		haircname.settext(newchar.gethairstyle()->getcolor());

		topname.settext(newchar.getequips().getequipname(Equipslot::TOP));
		botname.settext(newchar.getequips().getequipname(Equipslot::PANTS));
		shoename.settext(newchar.getequips().getequipname(Equipslot::SHOES));
		wepname.settext(newchar.getequips().getequipname(Equipslot::WEAPON));
		gendername.settext(female ? "Female" : "Male");
	}

	void UICharcreation::buttonpressed(uint16_t id)
	{
		switch (id)
		{
		case BT_CHARC_OK:
			if (named)
			{
				std::string cname = namechar.gettext();
				uint16_t cjob = 1;
				int32_t cface = faces[female][face];
				int32_t chair = hairs[female][hair];
				uint8_t chairc = haircolors[female][haircolor];
				uint8_t cskin = skins[female][skin];
				int32_t ctop = tops[female][top];
				int32_t cbot = bots[female][bot];
				int32_t cshoe = shoes[female][shoe];
				int32_t cwep = weapons[female][weapon];
				CreateCharPacket(cname, cjob, cface, chair, chairc, cskin, ctop, cbot, cshoe, cwep, female).dispatch();
			}
			else
			{
				std::string name = namechar.gettext();
				if (name.size() >= 4)
				{
					namechar.setstate(Textfield::NORMAL);

					UI::get().disable();
					UI::get().focustextfield(nullptr);
					NameCharPacket(name).dispatch();
				}
				else
				{
					UI::get().add(ElementTag<UILoginNotice, int8_t>(10));
					buttons[id]->setstate(Button::NORMAL);
				}
			}
			break;
		case BT_CHARC_CANCEL:
			if (named)
			{
				buttons[BT_CHARC_OK]->setposition(Point<int16_t>(482, 292));
				buttons[BT_CHARC_CANCEL]->setposition(Point<int16_t>(555, 292));
				buttons[BT_CHARC_FACEL]->setactive(false);
				buttons[BT_CHARC_FACER]->setactive(false);
				buttons[BT_CHARC_HAIRL]->setactive(false);
				buttons[BT_CHARC_HAIRR]->setactive(false);
				buttons[BT_CHARC_HAIRCL]->setactive(false);
				buttons[BT_CHARC_HAIRCR]->setactive(false);
				buttons[BT_CHARC_SKINL]->setactive(false);
				buttons[BT_CHARC_SKINR]->setactive(false);
				buttons[BT_CHARC_TOPL]->setactive(false);
				buttons[BT_CHARC_TOPR]->setactive(false);
				buttons[BT_CHARC_BOTL]->setactive(false);
				buttons[BT_CHARC_BOTR]->setactive(false);
				buttons[BT_CHARC_SHOESL]->setactive(false);
				buttons[BT_CHARC_SHOESR]->setactive(false);
				buttons[BT_CHARC_WEPL]->setactive(false);
				buttons[BT_CHARC_WEPR]->setactive(false);
				buttons[BT_CHARC_GENDERL]->setactive(false);
				buttons[BT_CHARC_GEMDERR]->setactive(false);
				buttons[BT_CHARC_CANCEL]->setstate(Button::NORMAL);
				namechar.setstate(Textfield::NORMAL);
				named = false;
			}
			else
			{
				active = false;
				UI::get().remove(CHARSELECT);
				UI::get().add(ElementTag<UICharSelect>());
			}
			break;
		}

		if (id >= BT_CHARC_FACEL && id <= BT_CHARC_GEMDERR)
		{
			switch (id)
			{
			case BT_CHARC_FACEL:
				face = (face > 0) ? face - 1 : faces[female].size() - 1;
				newchar.setface(faces[female][face]);
				facename.settext(newchar.getfacetype()->getname());
				break;
			case BT_CHARC_FACER:
				face = (face < faces[female].size() - 1) ? face + 1 : 0;
				newchar.setface(faces[female][face]);
				facename.settext(newchar.getfacetype()->getname());
				break;
			case BT_CHARC_HAIRL:
				hair = (hair > 0) ? hair - 1 : hairs[female].size() - 1;
				newchar.sethair(hairs[female][hair] + haircolors[female][haircolor]);
				hairname.settext(newchar.gethairstyle()->getname());
				break;
			case BT_CHARC_HAIRR:
				hair = (hair < hairs[female].size() - 1) ? hair + 1 : 0;
				newchar.sethair(hairs[female][hair] + haircolors[female][haircolor]);
				hairname.settext(newchar.gethairstyle()->getname());
				break;
			case BT_CHARC_HAIRCL:
				haircolor = (haircolor > 0) ? haircolor - 1 : haircolors[female].size() - 1;
				newchar.sethair(hairs[female][hair] + haircolors[female][haircolor]);
				haircname.settext(newchar.gethairstyle()->getcolor());
				break;
			case BT_CHARC_HAIRCR:
				haircolor = (haircolor < haircolors[female].size() - 1) ? haircolor + 1 : 0;
				newchar.sethair(hairs[female][hair] + haircolors[female][haircolor]);
				haircname.settext(newchar.gethairstyle()->getcolor());
				break;
			case BT_CHARC_SKINL:
				skin = (skin > 0) ? skin - 1 : skins[female].size() - 1;
				newchar.setbody(skins[female][skin]);
				bodyname.settext(newchar.getbodytype()->getname());
				break;
			case BT_CHARC_SKINR:
				skin = (skin < skins[female].size() - 1) ? skin + 1 : 0;
				newchar.setbody(skins[female][skin]);
				bodyname.settext(newchar.getbodytype()->getname());
				break;
			case BT_CHARC_TOPL:
				top = (top > 0) ? top - 1 : tops[female].size() - 1;
				newchar.addequip(tops[female][top]);
				topname.settext(newchar.getequips().getequipname(Equipslot::TOP));
				break;
			case BT_CHARC_TOPR:
				top = (top < tops[female].size() - 1) ? top + 1 : 0;
				newchar.addequip(tops[female][top]);
				topname.settext(newchar.getequips().getequipname(Equipslot::TOP));
				break;
			case BT_CHARC_BOTL:
				bot = (bot > 0) ? bot - 1 : bots[female].size() - 1;
				newchar.addequip(bots[female][bot]);
				botname.settext(newchar.getequips().getequipname(Equipslot::PANTS));
				break;
			case BT_CHARC_BOTR:
				bot = (bot < bots[female].size() - 1) ? bot + 1 : 0;
				newchar.addequip(bots[female][bot]);
				botname.settext(newchar.getequips().getequipname(Equipslot::PANTS));
				break;
			case BT_CHARC_SHOESL:
				shoe = (shoe > 0) ? shoe - 1 : shoes[female].size() - 1;
				newchar.addequip(shoes[female][shoe]);
				shoename.settext(newchar.getequips().getequipname(Equipslot::SHOES));
				break;
			case BT_CHARC_SHOESR:
				shoe = (shoe < shoes[female].size() - 1) ? shoe + 1 : 0;
				newchar.addequip(shoes[female][shoe]);
				shoename.settext(newchar.getequips().getequipname(Equipslot::SHOES));
				break;
			case BT_CHARC_WEPL:
				weapon = (weapon > 0) ? weapon - 1 : weapons[female].size() - 1;
				newchar.addequip(weapons[female][weapon]);
				wepname.settext(newchar.getequips().getequipname(Equipslot::WEAPON));
				break;
			case BT_CHARC_WEPR:
				weapon = (weapon < weapons[female].size() - 1) ? weapon + 1 : 0;
				newchar.addequip(weapons[female][weapon]);
				wepname.settext(newchar.getequips().getequipname(Equipslot::WEAPON));
				break;
			case BT_CHARC_GENDERL:
			case BT_CHARC_GEMDERR:
				female = !female;
				randomizelook();
				break;
			}
			buttons[id]->setstate(Button::MOUSEOVER);
		}
	}

	Cursor::State UICharcreation::sendmouse(bool clicked, Point<int16_t> cursorpos)
	{
		if (namechar.getstate() == Textfield::NORMAL)
		{
			Cursor::State tstate = namechar.sendcursor(cursorpos, clicked);
			if (tstate != Cursor::IDLE)
			{
				return tstate;
			}
		}

		return UIElement::sendmouse(clicked, cursorpos);
	}

	void UICharcreation::nameresult(bool nameused)
	{
		if (!named)
		{
			if (nameused)
			{
				namechar.settext("");
			}
			else
			{
				named = true;
				buttons[BT_CHARC_OK]->setposition(Point<int16_t>(486, 445));
				buttons[BT_CHARC_CANCEL]->setposition(Point<int16_t>(560, 445));
				buttons[BT_CHARC_FACEL]->setactive(true);
				buttons[BT_CHARC_FACER]->setactive(true);
				buttons[BT_CHARC_HAIRL]->setactive(true);
				buttons[BT_CHARC_HAIRR]->setactive(true);
				buttons[BT_CHARC_HAIRCL]->setactive(true);
				buttons[BT_CHARC_HAIRCR]->setactive(true);
				buttons[BT_CHARC_SKINL]->setactive(true);
				buttons[BT_CHARC_SKINR]->setactive(true);
				buttons[BT_CHARC_TOPL]->setactive(true);
				buttons[BT_CHARC_TOPR]->setactive(true);
				buttons[BT_CHARC_BOTL]->setactive(true);
				buttons[BT_CHARC_BOTR]->setactive(true);
				buttons[BT_CHARC_SHOESL]->setactive(true);
				buttons[BT_CHARC_SHOESR]->setactive(true);
				buttons[BT_CHARC_WEPL]->setactive(true);
				buttons[BT_CHARC_WEPR]->setactive(true);
				buttons[BT_CHARC_GENDERL]->setactive(true);
				buttons[BT_CHARC_GEMDERR]->setactive(true);
				namechar.setstate(Textfield::DISABLED);
			}
			buttons[BT_CHARC_OK]->setstate(Button::NORMAL);
		}
	}

	void UICharcreation::draw(float inter) const
	{
		for (int16_t i = 0; i < 2; i++)
		{
			for (int16_t k = 0; k < 800; k += sky.width())
			{
				sky.draw(Point<int16_t>(k, (400 * i) - 100));
			}
		}

		int16_t cloudx = static_cast<int16_t>(cloudfx) % 800;
		cloud.draw(Point<int16_t>(cloudx - cloud.width(), 300));
		cloud.draw(Point<int16_t>(cloudx, 300));
		cloud.draw(Point<int16_t>(cloudx + cloud.width(), 300));

		if (!named)
		{
			nameboard.draw(Point<int16_t>(455, 115));
			namechar.draw(position);
		}
		else
		{
			for (auto& lbit : lookboard)
			{
				lbit.draw(position, inter);
			}
		}

		UIElement::draw(inter);

		newchar.draw(Point<int16_t>(360, 348), inter);

		if (named)
		{
			facename.draw(Point<int16_t>(591, 214));
			hairname.draw(Point<int16_t>(591, 233));
			haircname.draw(Point<int16_t>(591, 252));
			bodyname.draw(Point<int16_t>(591, 271));
			topname.draw(Point<int16_t>(591, 290));
			botname.draw(Point<int16_t>(591, 309));
			shoename.draw(Point<int16_t>(591, 328));
			wepname.draw(Point<int16_t>(591, 347));
			gendername.draw(Point<int16_t>(591, 366));
		}
	}

	void UICharcreation::update()
	{
		UIElement::update();

		newchar.update(Constants::TIMESTEP);
		namechar.update(position);

		cloudfx += 0.25f;
	}
}