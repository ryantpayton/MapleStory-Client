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
#include "Clothing.h"

#include <nlnx\nx.hpp>

namespace jrc
{
	Clothing::Clothing(int32_t equipid, const BodyDrawinfo& drawinfo) 
		: ItemData(equipid) {

		static const Layer layers[15] = 
		{
			Layer::CAP, Layer::FACEACC, Layer::EYEACC, Layer::EARRINGS, Layer::TOP, Layer::MAIL,
			Layer::PANTS, Layer::SHOES, Layer::GLOVE, Layer::SHIELD, Layer::CAPE, Layer::RING,
			Layer::PENDANT, Layer::BELT, Layer::MEDAL
		};

		static const Equipslot::Value equipslots[15] =
		{
			Equipslot::CAP, Equipslot::FACEACC, Equipslot::EYEACC, Equipslot::EARRINGS, Equipslot::TOP, Equipslot::TOP,
			Equipslot::PANTS, Equipslot::SHOES, Equipslot::GLOVES, Equipslot::SHIELD, Equipslot::CAPE, Equipslot::RING,
			Equipslot::PENDANT, Equipslot::BELT, Equipslot::MEDAL
		};

		static const std::string equiptypes[15] = 
		{
			"HAT", "FACE ACCESSORY", "EYE ACCESSORY", "EARRINGS", "TOP", "OVERALL",
			"BOTTOM", "SHOES", "GLOVES", "SHIELD", "CAPE", "RING",
			"PENDANT", "BELT", "MEDAL"
		};

		Layer chlayer;
		int32_t index = (equipid / 10000) - 100;
		if (index >= 0 && index < 15)
		{
			chlayer = layers[index];
			eqslot = equipslots[index];
			type = equiptypes[index];
		}
		else if (index >= 30 && index <= 49)
		{
			chlayer = Layer::WEAPON;
			eqslot = Equipslot::WEAPON;

			static const std::string weapontypes[20] =
			{
				"ONE-HANDED SWORD", "ONE-HANDED AXE", "ONE-HANDED MACE", "DAGGER",
				"", "", "", "WAND", "STAFF", "", "TWO-HANDED SWORD", "TWO-HANDED AXE",
				"TWO-HANDED MACE", "SPEAR", "POLEARM", "BOW", "CROSSBOW", "CLAW",
				"KNUCKLE", "GUN"
			};

			int32_t weaponindex = index - 30;
			if (weaponindex >= 0 && weaponindex < 20)
				type = weapontypes[weaponindex];
			else
				type = "CASH";
		}
		else
		{
			chlayer = Layer::CAPE;
			eqslot = Equipslot::NONE;
			type = "";
		}

		nl::node equipnode = nl::nx::character[getcategory()]["0" + std::to_string(equipid) + ".img"];

		nl::node infonode = equipnode["info"];
		cash = infonode["cash"].get_bool();
		tradeblock = infonode["tradeBlock"].get_bool();
		price = infonode["price"];
		slots = infonode["tuc"];
		vslot = infonode["vslot"];
		reqstats[Maplestat::LEVEL] = infonode["reqLevel"];
		reqstats[Maplestat::JOB] = infonode["reqJob"];
		reqstats[Maplestat::STR] = infonode["reqSTR"];
		reqstats[Maplestat::DEX] = infonode["reqDEX"];
		reqstats[Maplestat::INT] = infonode["reqINT"];
		reqstats[Maplestat::LUK] = infonode["reqLUK"];
		defstats[Equipstat::STR] = infonode["incSTR"];
		defstats[Equipstat::DEX] = infonode["incDEX"];
		defstats[Equipstat::INT] = infonode["incINT"];
		defstats[Equipstat::LUK] = infonode["incLUK"];
		defstats[Equipstat::WATK] = infonode["incPAD"];
		defstats[Equipstat::WDEF] = infonode["incPDD"];
		defstats[Equipstat::MAGIC] = infonode["incMAD"];
		defstats[Equipstat::MDEF] = infonode["incMDD"];
		defstats[Equipstat::HP] = infonode["incMHP"];
		defstats[Equipstat::MP] = infonode["incMMP"];
		defstats[Equipstat::ACC] = infonode["incACC"];
		defstats[Equipstat::AVOID] = infonode["incEVA"];
		defstats[Equipstat::HANDS] = infonode["incHANDS"];
		defstats[Equipstat::SPEED] = infonode["incSPEED"];
		defstats[Equipstat::JUMP] = infonode["incJUMP"];

		for (auto it = Stance::getit(); it.hasnext(); it.increment())
		{
			Stance::Value stance = it.get();
			std::string stancename = Stance::nameof(stance);
			nl::node stancenode = equipnode[stancename];
			if (stancenode.size() == 0)
				continue;

			uint8_t frame = 0;
			nl::node framenode = stancenode[std::to_string(frame)];
			while (framenode.size() > 0)
			{
				for (auto partnode : framenode)
				{
					std::string part = partnode.name();
					if (partnode.data_type() == nl::node::type::bitmap)
					{
						Layer z = chlayer;
						std::string zs = partnode["z"];
						if (part == "mailArm")
							z = Layer::MAILARM;
						else
							z = sublayer(chlayer, zs);

						std::string parent;
						Point<int16_t> parentpos;
						for (auto mapnode : partnode["map"])
						{
							if (mapnode.data_type() == nl::node::type::vector)
							{
								parent = mapnode.name();
								parentpos = Point<int16_t>(mapnode);
							}
						}

						Point<int16_t> shift;
						switch (eqslot)
						{
						case Equipslot::FACEACC:
							shift -= parentpos;
							break;
						case Equipslot::SHOES:
						case Equipslot::GLOVES:
						case Equipslot::TOP:
						case Equipslot::PANTS:
						case Equipslot::CAPE:
							shift = drawinfo.getbodypos(stance, frame) - parentpos;
							break;
						case Equipslot::CAP:
						case Equipslot::EARRINGS:
						case Equipslot::EYEACC:
							shift = drawinfo.getfacepos(stance, frame) - parentpos;
							break;
						case Equipslot::SHIELD:
						case Equipslot::WEAPON:
							if (parent == "hand")
							{
								shift += drawinfo.getarmpos(stance, frame);
							}
							else if (parent == "handMove")
							{
								shift += drawinfo.gethandpos(stance, frame);
							}
							shift += drawinfo.getbodypos(stance, frame) - parentpos;
							break;
						}

						Texture texture = partnode;
						texture.shift(shift);
						stances[stance][z][frame].add(texture);
					}
				}

				frame++;
				framenode = stancenode[std::to_string(frame)];
			}
		}

		static const int32_t transparentequips[1] =
		{
			1002186
		};

		transparent = false;
		for (size_t i = 0; i < 1; i++)
		{
			if (equipid == transparentequips[i])
				transparent = true;
		}
	}

	Clothing::Clothing()
	{
		transparent = true;
		type = "";
		eqslot = Equipslot::NONE;
	}

	void Clothing::draw(Stance::Value stance, Layer layer, uint8_t frame, const DrawArgument& args) const
	{
		auto frameit = stances[stance][layer].find(frame);
		if (frameit == stances[stance][layer].end())
			return;

		frameit->second.draw(args);
	}

	bool Clothing::contains_layer(Stance::Value stance, Layer layer) const
	{
		return !stances[stance][layer].empty();
	}

	bool Clothing::istransparent() const
	{
		return transparent;
	}

	bool Clothing::isweapon() const
	{
		return eqslot == WEAPON;
	}

	int16_t Clothing::getreqstat(Maplestat::Value stat) const
	{
		return reqstats[stat];
	}

	int16_t Clothing::getdefstat(Equipstat::Value stat) const
	{
		return defstats[stat];
	}

	std::string Clothing::gettype() const
	{
		return type;
	}

	std::string Clothing::getvslot() const
	{
		return vslot;
	}

	Equipslot::Value Clothing::geteqslot() const
	{
		return eqslot;
	}


	EnumIterator<Clothing::Layer> Clothing::layerit()
	{
		return{ CAPE, WEAPONOGLOVE };
	}

	Clothing::Layer Clothing::sublayer(Layer base, const std::string& name)
	{
		switch (base)
		{
		case WEAPON:
			if (name == "weaponOverHand")
				return Layer::WEAPONOHAND;
			else if (name == "weaponOverGlove")
				return Layer::WEAPONOGLOVE;
			else if (name == "weaponOverBody")
				return Layer::WEAPONOBODY;
			else if (name == "weaponBelowArm")
				return Layer::WEAPONBARM;
			else if (name == "weaponBelowBody")
				return Layer::WEAPONBBODY;
			else if (name == "backWeaponOverShield")
				return Layer::BACKWEAPON;
			break;
		case SHIELD:
			if (name == "shieldOverHair")
				return Layer::SHIELDOHAIR;
			else if (name == "shieldBelowBody")
				return Layer::SHIELDBBODY;
			else if (name == "backShield")
				return Layer::BACKSHIELD;
			break;
		case GLOVE:
			if (name == "gloveWrist")
				return Layer::WRIST;
			else if (name == "gloveOverHair")
				return Layer::GLOVEOHAIR;
			else if (name == "gloveOverBody")
				return Layer::GLOVEOBODY;
			else if (name == "gloveWristOverHair")
				return Layer::WRISTOHAIR;
			else if (name == "gloveWristOverBody")
				return Layer::WRISTOBODY;
		case CAP:
			if (name == "capOverHair")
				return Layer::CAPOHAIR;
			else if (name == "capBelowBody")
				return Layer::CAPBBODY;
			break;
		}
		return base;
	}
}
