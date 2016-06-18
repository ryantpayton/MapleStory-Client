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
		female = Session::get()
			.get_login()
			.getaccount()
			.female;

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

		sprites_lookboard.emplace_back(crsrc["charSet"], Point<int16_t>(450, 115));
		sprites_lookboard.emplace_back(crsrc["avatarSel"]["0"]["normal"], Point<int16_t>(461, 217));
		sprites_lookboard.emplace_back(crsrc["avatarSel"]["1"]["normal"], Point<int16_t>(461, 236));
		sprites_lookboard.emplace_back(crsrc["avatarSel"]["2"]["normal"], Point<int16_t>(461, 255));
		sprites_lookboard.emplace_back(crsrc["avatarSel"]["3"]["normal"], Point<int16_t>(461, 274));
		sprites_lookboard.emplace_back(crsrc["avatarSel"]["4"]["normal"], Point<int16_t>(461, 293));
		sprites_lookboard.emplace_back(crsrc["avatarSel"]["5"]["normal"], Point<int16_t>(461, 312));
		sprites_lookboard.emplace_back(crsrc["avatarSel"]["6"]["normal"], Point<int16_t>(461, 331));
		sprites_lookboard.emplace_back(crsrc["avatarSel"]["7"]["normal"], Point<int16_t>(461, 350));
		sprites_lookboard.emplace_back(crsrc["avatarSel"]["8"]["normal"], Point<int16_t>(461, 369));

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

		buttons[BT_CHARC_FACEL]->set_active(false);
		buttons[BT_CHARC_FACER]->set_active(false);
		buttons[BT_CHARC_HAIRL]->set_active(false);
		buttons[BT_CHARC_HAIRR]->set_active(false);
		buttons[BT_CHARC_HAIRCL]->set_active(false);
		buttons[BT_CHARC_HAIRCR]->set_active(false);
		buttons[BT_CHARC_SKINL]->set_active(false);
		buttons[BT_CHARC_SKINR]->set_active(false);
		buttons[BT_CHARC_TOPL]->set_active(false);
		buttons[BT_CHARC_TOPR]->set_active(false);
		buttons[BT_CHARC_BOTL]->set_active(false);
		buttons[BT_CHARC_BOTR]->set_active(false);
		buttons[BT_CHARC_SHOESL]->set_active(false);
		buttons[BT_CHARC_SHOESR]->set_active(false);
		buttons[BT_CHARC_WEPL]->set_active(false);
		buttons[BT_CHARC_WEPR]->set_active(false);
		buttons[BT_CHARC_GENDERL]->set_active(false);
		buttons[BT_CHARC_GEMDERR]->set_active(false);

		namechar = { Text::A13M, Text::LEFT, Text::WHITE, { { 490, 219 }, { 630, 243 } }, 12 };
		namechar.set_state(Textfield::FOCUSED);

		facename = { Text::A11M, Text::CENTER, Text::BLACK };
		hairname = { Text::A11M, Text::CENTER, Text::BLACK };
		haircname = { Text::A11M, Text::CENTER, Text::BLACK };
		bodyname = { Text::A11M, Text::CENTER, Text::BLACK };
		topname = { Text::A11M, Text::CENTER, Text::BLACK };
		botname = { Text::A11M, Text::CENTER, Text::BLACK };
		shoename = { Text::A11M, Text::CENTER, Text::BLACK };
		wepname = { Text::A11M, Text::CENTER, Text::BLACK };
		gendername = { Text::A11M, Text::CENTER, Text::BLACK };

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

		randomize_look();

		newchar.set_direction(true);

		position = { 0, 0 };
		dimension = { 800, 600 };
		active = true;
		cloudfx = 200.0f;
	}

	void UICharcreation::randomize_look()
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
		newchar.add_equip(tops[female][top]);
		newchar.add_equip(bots[female][bot]);
		newchar.add_equip(shoes[female][shoe]);
		newchar.add_equip(weapons[female][weapon]);

		bodyname.change_text(newchar.getbodytype()->get_name());
		facename.change_text(newchar.getfacetype()->get_name());
		hairname.change_text(newchar.gethairstyle()->get_name());
		haircname.change_text(newchar.gethairstyle()->getcolor());

		topname.change_text(newchar.get_equips().get_equipname(Equipslot::TOP));
		botname.change_text(newchar.get_equips().get_equipname(Equipslot::PANTS));
		shoename.change_text(newchar.get_equips().get_equipname(Equipslot::SHOES));
		wepname.change_text(newchar.get_equips().get_equipname(Equipslot::WEAPON));
		gendername.change_text(female ? "Female" : "Male");
	}

	Button::State UICharcreation::button_pressed(uint16_t id)
	{
		switch (id)
		{
		case BT_CHARC_OK:
			if (named)
			{
				std::string cname = namechar.get_text();
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
				return Button::PRESSED;
			}
			else
			{
				std::string name = namechar.get_text();
				if (name.size() >= 4)
				{
					namechar.set_state(Textfield::NORMAL);

					UI::get().disable();
					UI::get().focus_textfield(nullptr);
					NameCharPacket(name).dispatch();
					return Button::PRESSED;
				}
				else
				{
					UI::get().add(Element<UILoginNotice, int8_t>(10));
					return Button::NORMAL;
				}
			}
		case BT_CHARC_CANCEL:
			if (named)
			{
				buttons[BT_CHARC_OK]->set_position({ 482, 292 });
				buttons[BT_CHARC_CANCEL]->set_position({ 555, 292 });
				buttons[BT_CHARC_FACEL]->set_active(false);
				buttons[BT_CHARC_FACER]->set_active(false);
				buttons[BT_CHARC_HAIRL]->set_active(false);
				buttons[BT_CHARC_HAIRR]->set_active(false);
				buttons[BT_CHARC_HAIRCL]->set_active(false);
				buttons[BT_CHARC_HAIRCR]->set_active(false);
				buttons[BT_CHARC_SKINL]->set_active(false);
				buttons[BT_CHARC_SKINR]->set_active(false);
				buttons[BT_CHARC_TOPL]->set_active(false);
				buttons[BT_CHARC_TOPR]->set_active(false);
				buttons[BT_CHARC_BOTL]->set_active(false);
				buttons[BT_CHARC_BOTR]->set_active(false);
				buttons[BT_CHARC_SHOESL]->set_active(false);
				buttons[BT_CHARC_SHOESR]->set_active(false);
				buttons[BT_CHARC_WEPL]->set_active(false);
				buttons[BT_CHARC_WEPR]->set_active(false);
				buttons[BT_CHARC_GENDERL]->set_active(false);
				buttons[BT_CHARC_GEMDERR]->set_active(false);
				buttons[BT_CHARC_CANCEL]->set_state(Button::NORMAL);
				namechar.set_state(Textfield::NORMAL);
				named = false;
			}
			else
			{
				active = false;
				UI::get().add(Element<UICharSelect>());
			}
			return Button::PRESSED;
		}

		if (id >= BT_CHARC_FACEL && id <= BT_CHARC_GEMDERR)
		{
			switch (id)
			{
			case BT_CHARC_FACEL:
				face = (face > 0) ? face - 1 : faces[female].size() - 1;
				newchar.setface(faces[female][face]);
				facename.change_text(newchar.getfacetype()->get_name());
				break;
			case BT_CHARC_FACER:
				face = (face < faces[female].size() - 1) ? face + 1 : 0;
				newchar.setface(faces[female][face]);
				facename.change_text(newchar.getfacetype()->get_name());
				break;
			case BT_CHARC_HAIRL:
				hair = (hair > 0) ? hair - 1 : hairs[female].size() - 1;
				newchar.sethair(hairs[female][hair] + haircolors[female][haircolor]);
				hairname.change_text(newchar.gethairstyle()->get_name());
				break;
			case BT_CHARC_HAIRR:
				hair = (hair < hairs[female].size() - 1) ? hair + 1 : 0;
				newchar.sethair(hairs[female][hair] + haircolors[female][haircolor]);
				hairname.change_text(newchar.gethairstyle()->get_name());
				break;
			case BT_CHARC_HAIRCL:
				haircolor = (haircolor > 0) ? haircolor - 1 : haircolors[female].size() - 1;
				newchar.sethair(hairs[female][hair] + haircolors[female][haircolor]);
				haircname.change_text(newchar.gethairstyle()->getcolor());
				break;
			case BT_CHARC_HAIRCR:
				haircolor = (haircolor < haircolors[female].size() - 1) ? haircolor + 1 : 0;
				newchar.sethair(hairs[female][hair] + haircolors[female][haircolor]);
				haircname.change_text(newchar.gethairstyle()->getcolor());
				break;
			case BT_CHARC_SKINL:
				skin = (skin > 0) ? skin - 1 : skins[female].size() - 1;
				newchar.setbody(skins[female][skin]);
				bodyname.change_text(newchar.getbodytype()->get_name());
				break;
			case BT_CHARC_SKINR:
				skin = (skin < skins[female].size() - 1) ? skin + 1 : 0;
				newchar.setbody(skins[female][skin]);
				bodyname.change_text(newchar.getbodytype()->get_name());
				break;
			case BT_CHARC_TOPL:
				top = (top > 0) ? top - 1 : tops[female].size() - 1;
				newchar.add_equip(tops[female][top]);
				topname.change_text(newchar.get_equips().get_equipname(Equipslot::TOP));
				break;
			case BT_CHARC_TOPR:
				top = (top < tops[female].size() - 1) ? top + 1 : 0;
				newchar.add_equip(tops[female][top]);
				topname.change_text(newchar.get_equips().get_equipname(Equipslot::TOP));
				break;
			case BT_CHARC_BOTL:
				bot = (bot > 0) ? bot - 1 : bots[female].size() - 1;
				newchar.add_equip(bots[female][bot]);
				botname.change_text(newchar.get_equips().get_equipname(Equipslot::PANTS));
				break;
			case BT_CHARC_BOTR:
				bot = (bot < bots[female].size() - 1) ? bot + 1 : 0;
				newchar.add_equip(bots[female][bot]);
				botname.change_text(newchar.get_equips().get_equipname(Equipslot::PANTS));
				break;
			case BT_CHARC_SHOESL:
				shoe = (shoe > 0) ? shoe - 1 : shoes[female].size() - 1;
				newchar.add_equip(shoes[female][shoe]);
				shoename.change_text(newchar.get_equips().get_equipname(Equipslot::SHOES));
				break;
			case BT_CHARC_SHOESR:
				shoe = (shoe < shoes[female].size() - 1) ? shoe + 1 : 0;
				newchar.add_equip(shoes[female][shoe]);
				shoename.change_text(newchar.get_equips().get_equipname(Equipslot::SHOES));
				break;
			case BT_CHARC_WEPL:
				weapon = (weapon > 0) ? weapon - 1 : weapons[female].size() - 1;
				newchar.add_equip(weapons[female][weapon]);
				wepname.change_text(newchar.get_equips().get_equipname(Equipslot::WEAPON));
				break;
			case BT_CHARC_WEPR:
				weapon = (weapon < weapons[female].size() - 1) ? weapon + 1 : 0;
				newchar.add_equip(weapons[female][weapon]);
				wepname.change_text(newchar.get_equips().get_equipname(Equipslot::WEAPON));
				break;
			case BT_CHARC_GENDERL:
			case BT_CHARC_GEMDERR:
				female = !female;
				randomize_look();
				break;
			}
			return Button::MOUSEOVER;
		}
		return Button::PRESSED;
	}

	Cursor::State UICharcreation::send_cursor(bool clicked, Point<int16_t> cursorpos)
	{
		if (Cursor::State new_state = namechar.send_cursor(cursorpos, clicked))
			return new_state;

		return UIElement::send_cursor(clicked, cursorpos);
	}

	void UICharcreation::send_naming_result(bool nameused)
	{
		if (!named)
		{
			if (nameused)
			{
				namechar.change_text("");
			}
			else
			{
				named = true;
				buttons[BT_CHARC_OK]->set_position(Point<int16_t>(486, 445));
				buttons[BT_CHARC_CANCEL]->set_position(Point<int16_t>(560, 445));
				buttons[BT_CHARC_FACEL]->set_active(true);
				buttons[BT_CHARC_FACER]->set_active(true);
				buttons[BT_CHARC_HAIRL]->set_active(true);
				buttons[BT_CHARC_HAIRR]->set_active(true);
				buttons[BT_CHARC_HAIRCL]->set_active(true);
				buttons[BT_CHARC_HAIRCR]->set_active(true);
				buttons[BT_CHARC_SKINL]->set_active(true);
				buttons[BT_CHARC_SKINR]->set_active(true);
				buttons[BT_CHARC_TOPL]->set_active(true);
				buttons[BT_CHARC_TOPR]->set_active(true);
				buttons[BT_CHARC_BOTL]->set_active(true);
				buttons[BT_CHARC_BOTR]->set_active(true);
				buttons[BT_CHARC_SHOESL]->set_active(true);
				buttons[BT_CHARC_SHOESR]->set_active(true);
				buttons[BT_CHARC_WEPL]->set_active(true);
				buttons[BT_CHARC_WEPR]->set_active(true);
				buttons[BT_CHARC_GENDERL]->set_active(true);
				buttons[BT_CHARC_GEMDERR]->set_active(true);
				namechar.set_state(Textfield::DISABLED);
			}
			buttons[BT_CHARC_OK]->set_state(Button::NORMAL);
		}
	}

	void UICharcreation::draw(float alpha) const
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
			nameboard.draw(Point<int16_t>(455, 115 ));
			namechar.draw(position);
		}
		else
		{
			for (auto& sprite : sprites_lookboard)
			{
				sprite.draw(position, alpha);
			}
		}

		UIElement::draw(alpha);

		newchar.draw({ 360, 348 }, alpha);

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

		if (named)
		{
			for (auto& sprite : sprites_lookboard)
			{
				sprite.update();
			}
		}

		newchar.update(Constants::TIMESTEP);
		namechar.update(position);

		cloudfx += 0.25f;
	}
}