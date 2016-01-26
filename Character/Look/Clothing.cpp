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
#include "Clothing.h"
#include "nlnx\nx.hpp"

namespace Character
{
	Clothing::Clothing(int32_t equipid, const BodyDrawinfo& drawinfo) : ItemData(equipid)
	{
		static const CharacterLayer layers[15] = 
		{
			CL_HAT, CL_FACEACC, CL_EYEACC, CL_EARRINGS, CL_TOP, CL_MAIL,
			CL_PANTS, CL_SHOES, CL_GLOVE, CL_SHIELD, CL_CAPE, CL_RING,
			CL_PENDANT, CL_BELT, CL_MEDAL
		};

		static const Slot equipslots[15] =
		{
			CAP, FACEACC, EYEACC, EARRINGS, TOP, TOP,
			PANTS, SHOES, GLOVES, SHIELD, CAPE, RING,
			PENDANT, BELT, MEDAL
		};

		static const string equiptypes[15] = 
		{
			"HAT", "FACE ACCESSORY", "EYE ACCESSORY", "EARRINGS", "TOP", "OVERALL",
			"BOTTOM", "SHOES", "GLOVES", "SHIELD", "CAPE", "RING",
			"PENDANT", "BELT", "MEDAL"
		};

		CharacterLayer chlayer;
		int32_t index = (equipid / 10000) - 100;
		if (index >= 0 && index < 15)
		{
			chlayer = layers[index];
			eqslot = equipslots[index];
			type = equiptypes[index];
		}
		else if (index >= 30 && index <= 49)
		{
			chlayer = CL_WEAPON;
			eqslot = WEAPON;

			static const string weapontypes[20] =
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
			chlayer = CL_BASE;
			eqslot = NONE;
			type = "";
		}

		using nl::node;
		node equipnode = nl::nx::character[getcategory()]["0" + std::to_string(equipid) + ".img"];
		for (node stancenode : equipnode)
		{
			string stance = stancenode.name();
			if (stance == "info")
			{
				cash = stancenode["cash"].get_bool();
				tradeblock = stancenode["tradeBlock"].get_bool();
				price = stancenode["price"];
				slots = stancenode["tuc"];

				reqstats[Maplestat::LEVEL] = stancenode["reqLevel"];
				reqstats[Maplestat::JOB] = stancenode["reqJob"];
				reqstats[Maplestat::STR] = stancenode["reqSTR"];
				reqstats[Maplestat::DEX] = stancenode["reqDEX"];
				reqstats[Maplestat::INT] = stancenode["reqINT"];
				reqstats[Maplestat::LUK] = stancenode["reqLUK"];

				defstats[Equipstat::STR] = stancenode["incSTR"];
				defstats[Equipstat::DEX] = stancenode["incDEX"];
				defstats[Equipstat::INT] = stancenode["incINT"];
				defstats[Equipstat::LUK] = stancenode["incLUK"];
				defstats[Equipstat::WATK] = stancenode["incPAD"];
				defstats[Equipstat::WDEF] = stancenode["incPDD"];
				defstats[Equipstat::MAGIC] = stancenode["incMAD"];
				defstats[Equipstat::MDEF] = stancenode["incMDD"];
				defstats[Equipstat::HP] = stancenode["incMHP"];
				defstats[Equipstat::MP] = stancenode["incMMP"];
				defstats[Equipstat::ACC] = stancenode["incACC"];
				defstats[Equipstat::AVOID] = stancenode["incEVA"];
				defstats[Equipstat::HANDS] = stancenode["incHANDS"];
				defstats[Equipstat::SPEED] = stancenode["incSPEED"];
				defstats[Equipstat::JUMP] = stancenode["incJUMP"];
			}
			else if (stance != "default" && stance != "backDefault")
			{
				uint8_t frame = 0;
				node framenode = stancenode[std::to_string(frame)];
				while (framenode.size() > 0)
				{
					for (node partnode : framenode)
					{
						string part = partnode.name();
						if (partnode.data_type() == node::type::bitmap)
						{
							CharacterLayer z = chlayer;
							string zs = partnode["z"];
							if (z == CL_WEAPON)
							{
								if (zs == "weaponOverHand")
									z = CL_WEAPONOHAND;
								else if (zs == "weaponOverGlove")
									z = CL_WEAPONOGLOVE;
								else if (zs == "backWeaponOverShield")
									z = CL_BACKWEAPON;
							}
							else if (z == CL_SHIELD)
							{
								if (zs == "shieldOverHair")
									z = CL_SHIELDOHAIR;
								else if (zs == "shieldBelowBody")
									z = CL_SHIELDBBODY;
								else if (zs == "backShield")
									z = CL_BACKSHIELD;
							}
							else if (z == CL_HAT)
							{
								if (zs == "capOverHair")
									z = CL_HATOVERHAIR;
							}
							else if (part == "mailArm")
							{
								z = CL_MAILARM;
							}

							stances[stance][z][frame] = Texture(partnode);

							string parent;
							vector2d<int16_t> parentpos;
							for (node mapnode : partnode["map"])
							{
								if (mapnode.data_type() == node::type::vector)
								{
									parent = mapnode.name();
									parentpos = vector2d<int16_t>(mapnode);
								}
							}

							vector2d<int16_t> shift;
							switch (z)
							{
							case CL_FACEACC:
								shift -= parentpos;
								break;
							case CL_TOP:
							case CL_PANTS:
							case CL_CAPE:
							case CL_SHOES:
							case CL_MAIL:
							case CL_MAILARM:
							case CL_GLOVE:
							case CL_BACKWEAPON:
							case CL_BACKSHIELD:
								shift = drawinfo.getbodypos(stance, frame) - parentpos;
								break;
							case CL_HAT:
							case CL_HATOVERHAIR:
							case CL_EARRINGS:
							case CL_EYEACC:
								shift = drawinfo.getfacepos(stance, frame) - parentpos;
								break;
							case CL_SHIELD:
							case CL_SHIELDOHAIR:
							case CL_WEAPON:
							case CL_WEAPONOHAND:
							case CL_WEAPONOGLOVE:
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
							default:
								shift = vector2d<int16_t>();
							}

							stances[stance][z][frame].shift(shift);
						}
					}

					frame++;
					framenode = stancenode[std::to_string(frame)];
				}
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
		eqslot = NONE;
	}

	void Clothing::draw(string stance, CharacterLayer layer, 
		uint8_t frame, const DrawArgument& args) const {

		if (!stances.count(stance))
			return;

		if (!stances.at(stance).count(layer))
			return;

		if (!stances.at(stance).at(layer).count(frame))
			return;

		stances.at(stance).at(layer).at(frame).draw(args);
	}

	bool Clothing::islayer(string stance, CharacterLayer layer) const
	{
		if (!stances.count(stance))
			return false;
		else
			return stances.at(stance).count(layer) > 0;
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
		if (reqstats.count(stat))
			return reqstats.at(stat);
		else
			return 0;
	}

	int16_t Clothing::getdefstat(Equipstat::Value stat) const
	{
		if (defstats.count(stat))
			return defstats.at(stat);
		else
			return 0;
	}

	string Clothing::getstatstr(Equipstat::Value stat) const
	{
		switch (stat)
		{
		case Equipstat::STR:
			return "STR";
		case Equipstat::DEX:
			return "DEX";
		case Equipstat::INT:
			return "INT";
		case Equipstat::LUK:
			return "LUK";
		case Equipstat::WATK:
			return "WEAPON ATT";
		case Equipstat::WDEF:
			return "WEAPON DEFENSE";
		case Equipstat::MAGIC:
			return "MAGIC ATT";
		case Equipstat::MDEF:
			return "MAGIC DEFENSE";
		case Equipstat::ACC:
			return "ACCURACY";
		case Equipstat::AVOID:
			return "AVOID";
		case Equipstat::HP:
			return "MAX HP";
		case Equipstat::MP:
			return "MAX MP";
		case Equipstat::HANDS:
			return "HANDS";
		case Equipstat::SPEED:
			return "SPEED";
		case Equipstat::JUMP:
			return "JUMP";
		default:
			return "";
		}
	}

	string Clothing::gettype() const
	{
		return type;
	}

	Clothing::Slot Clothing::geteqslot() const
	{
		return eqslot;
	}
}
