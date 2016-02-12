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
#include "IO\UI.h"
#include "Net\Session.h"
#include "Net\Packets\CharCreationPackets.h"
#include "IO\Components\MapleButton.h"
#include "IO\UITypes\UILoginNotice.h"
#include "IO\UITypes\UICharSelect.h"
#include "Constants.h"
#include "nlnx\nx.hpp"

namespace IO
{
	using Character::Clothing;
	using Character::Equipslot;
	using Net::Session;

	UICharcreation::UICharcreation()
	{
		female = Session::get().getlogin().getaccount().female;

		using nl::node;
		node src = nl::nx::ui["Login.img"];
		node bgsrc = nl::nx::map["Back"]["login.img"]["back"];
		node crsrc = src["NewChar"];

		sky = Texture(bgsrc["2"]);
		cloud = Texture(bgsrc["27"]);

		sprites.push_back(Sprite(bgsrc["15"], vector2d<int16_t>(153, 685)));
		sprites.push_back(Sprite(bgsrc["16"], vector2d<int16_t>(200, 400)));
		sprites.push_back(Sprite(bgsrc["17"], vector2d<int16_t>(160, 263)));
		sprites.push_back(Sprite(bgsrc["18"], vector2d<int16_t>(349, 1220)));
		sprites.push_back(Sprite(src["Common"]["frame"], vector2d<int16_t>(400, 290)));

		nameboard = Texture(crsrc["charName"]);

		lookboard.push_back(Sprite(crsrc["charSet"], vector2d<int16_t>(450, 115)));
		lookboard.push_back(Sprite(crsrc["avatarSel"]["0"]["normal"], vector2d<int16_t>(461, 217)));
		lookboard.push_back(Sprite(crsrc["avatarSel"]["1"]["normal"], vector2d<int16_t>(461, 236)));
		lookboard.push_back(Sprite(crsrc["avatarSel"]["2"]["normal"], vector2d<int16_t>(461, 255)));
		lookboard.push_back(Sprite(crsrc["avatarSel"]["3"]["normal"], vector2d<int16_t>(461, 274)));
		lookboard.push_back(Sprite(crsrc["avatarSel"]["4"]["normal"], vector2d<int16_t>(461, 293)));
		lookboard.push_back(Sprite(crsrc["avatarSel"]["5"]["normal"], vector2d<int16_t>(461, 312)));
		lookboard.push_back(Sprite(crsrc["avatarSel"]["6"]["normal"], vector2d<int16_t>(461, 331)));
		lookboard.push_back(Sprite(crsrc["avatarSel"]["7"]["normal"], vector2d<int16_t>(461, 350)));
		lookboard.push_back(Sprite(crsrc["avatarSel"]["8"]["normal"], vector2d<int16_t>(461, 369)));

		buttons[BT_CHARC_OK] = unique_ptr<Button>(
			new MapleButton(crsrc["BtYes"], vector2d<int16_t>(482, 292))
			);
		buttons[BT_CHARC_CANCEL] = unique_ptr<Button>(
			new MapleButton(crsrc["BtNo"], vector2d<int16_t>(555, 292))
			);

		buttons[BT_CHARC_FACEL] = unique_ptr<Button>(
			new MapleButton(crsrc["BtLeft"], vector2d<int16_t>(521, 216))
			);
		buttons[BT_CHARC_FACER] = unique_ptr<Button>(
			new MapleButton(crsrc["BtRight"], vector2d<int16_t>(645, 216))
			);
		buttons[BT_CHARC_HAIRL] = unique_ptr<Button>(
			new MapleButton(crsrc["BtLeft"], vector2d<int16_t>(521, 235))
			);
		buttons[BT_CHARC_HAIRR] = unique_ptr<Button>(
			new MapleButton(crsrc["BtRight"], vector2d<int16_t>(645, 235))
			);
		buttons[BT_CHARC_HAIRCL] = unique_ptr<Button>(
			new MapleButton(crsrc["BtLeft"], vector2d<int16_t>(521, 254))
			);
		buttons[BT_CHARC_HAIRCR] = unique_ptr<Button>(
			new MapleButton(crsrc["BtRight"], vector2d<int16_t>(645, 254))
			);
		buttons[BT_CHARC_SKINL] = unique_ptr<Button>(
			new MapleButton(crsrc["BtLeft"], vector2d<int16_t>(521, 273))
			);
		buttons[BT_CHARC_SKINR] = unique_ptr<Button>(
			new MapleButton(crsrc["BtRight"], vector2d<int16_t>(645, 273))
			);
		buttons[BT_CHARC_TOPL] = unique_ptr<Button>(
			new MapleButton(crsrc["BtLeft"], vector2d<int16_t>(521, 292))
			);
		buttons[BT_CHARC_TOPR] = unique_ptr<Button>(
			new MapleButton(crsrc["BtRight"], vector2d<int16_t>(645, 292))
			);
		buttons[BT_CHARC_BOTL] = unique_ptr<Button>(
			new MapleButton(crsrc["BtLeft"], vector2d<int16_t>(521, 311))
			);
		buttons[BT_CHARC_BOTR] = unique_ptr<Button>(
			new MapleButton(crsrc["BtRight"], vector2d<int16_t>(645, 311))
			);
		buttons[BT_CHARC_SHOESL] = unique_ptr<Button>(
			new MapleButton(crsrc["BtLeft"], vector2d<int16_t>(521, 330))
			);
		buttons[BT_CHARC_SHOESR] = unique_ptr<Button>(
			new MapleButton(crsrc["BtRight"], vector2d<int16_t>(645, 330))
			);
		buttons[BT_CHARC_WEPL] = unique_ptr<Button>(
			new MapleButton(crsrc["BtLeft"], vector2d<int16_t>(521, 349))
			);
		buttons[BT_CHARC_WEPR] = unique_ptr<Button>(
			new MapleButton(crsrc["BtRight"], vector2d<int16_t>(645, 348))
			);
		buttons[BT_CHARC_GENDERL] = unique_ptr<Button>(
			new MapleButton(crsrc["BtLeft"], vector2d<int16_t>(521, 368))
			);
		buttons[BT_CHARC_GEMDERR] = unique_ptr<Button>(
			new MapleButton(crsrc["BtRight"], vector2d<int16_t>(645, 368))
			);

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

		rectangle2d<int16_t> txfae = rectangle2d<int16_t>(
			vector2d<int16_t>(490, 219),
			vector2d<int16_t>(630, 243)
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

		node mkinfo = nl::nx::etc["MakeCharInfo.img"]["Info"];
		for (int32_t i = 0; i < 2; i++)
		{
			bool f;
			node mk_n;
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

			for (node subnode : mk_n)
			{
				int num = stoi(subnode.name());
				for (node idnode : subnode)
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

		position = vector2d<int16_t>(0, 0);
		dimension = vector2d<int16_t>(800, 600);
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
				using Net::CreateCharPacket;
				Session::get().dispatch(CreateCharPacket(
					namechar.gettext(), 1, faces[female][face], hairs[female][hair], 
					haircolors[female][haircolor], skins[female][skin], tops[female][top], 
					bots[female][bot], shoes[female][shoe], weapons[female][weapon], female)
					);
			}
			else
			{
				string name = namechar.gettext();
				if (name.size() >= 4)
				{
					UI::get().disable();
					UI::get().focustextfield(nullptr);
					namechar.setstate(Textfield::NORMAL);

					using Net::NameCharPacket;
					Session::get().dispatch(NameCharPacket(name));
				}
				else
				{
					UI::get().add(ElementLoginNotice(10));
					buttons[id]->setstate(Button::NORMAL);
				}
			}
			break;
		case BT_CHARC_CANCEL:
			if (named)
			{
				buttons[BT_CHARC_OK]->setposition(vector2d<int16_t>(482, 292));
				buttons[BT_CHARC_CANCEL]->setposition(vector2d<int16_t>(555, 292));
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
				UI::get().focustextfield(nullptr);
				UI::get().remove(Element::CHARSELECT);
				UI::get().add(ElementCharSelect());
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

	Cursor::State UICharcreation::sendmouse(bool down, vector2d<int16_t> pos)
	{
		Cursor::State ret = UIElement::sendmouse(down, pos);

		if (namechar.getbounds(position).contains(pos))
		{
			if (down)
			{
				UI::get().focustextfield(&namechar);
				namechar.setstate(Textfield::FOCUSED);
			}
			else if (namechar.getstate() == Textfield::NORMAL)
			{
				ret = Cursor::CANCLICK;
			}
		}
		else if (down)
		{
			UI::get().focustextfield(nullptr);
			namechar.setstate(Textfield::NORMAL);
		}

		return ret;
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
				buttons[BT_CHARC_OK]->setposition(vector2d<int16_t>(486, 445));
				buttons[BT_CHARC_CANCEL]->setposition(vector2d<int16_t>(560, 445));
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
		using::Graphics::DrawArgument;

		for (int16_t i = 0; i < 2; i++)
		{
			for (int16_t k = 0; k < 800; k += sky.getdimensions().x())
			{
				sky.draw(DrawArgument(vector2d<int16_t>(k, (400 * i) - 100)));
			}
		}

		int16_t cloudx = static_cast<int16_t>(cloudfx) % 800;
		cloud.draw(DrawArgument(vector2d<int16_t>(cloudx - cloud.getdimensions().x(), 300)));
		cloud.draw(DrawArgument(vector2d<int16_t>(cloudx, 300)));
		cloud.draw(DrawArgument(vector2d<int16_t>(cloudx + cloud.getdimensions().x(), 300)));

		if (!named)
		{
			nameboard.draw(DrawArgument(vector2d<int16_t>(455, 115)));
			namechar.draw(position);
		}
		else
		{
			for (auto& lbit : lookboard)
			{
				lbit.draw(DrawArgument(position), inter);
			}
		}

		UIElement::draw(inter);

		newchar.draw(vector2d<int16_t>(360, 348), inter);

		if (named)
		{
			facename.draw(vector2d<int16_t>(591, 214));
			hairname.draw(vector2d<int16_t>(591, 233));
			haircname.draw(vector2d<int16_t>(591, 252));
			bodyname.draw(vector2d<int16_t>(591, 271));
			topname.draw(vector2d<int16_t>(591, 290));
			botname.draw(vector2d<int16_t>(591, 309));
			shoename.draw(vector2d<int16_t>(591, 328));
			wepname.draw(vector2d<int16_t>(591, 347));
			gendername.draw(vector2d<int16_t>(591, 366));
		}
	}

	void UICharcreation::update()
	{
		UIElement::update();

		newchar.update(Constants::TIMESTEP);
		namechar.update();

		cloudfx += 0.25f;
	}
}