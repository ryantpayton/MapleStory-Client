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

		static const Equipslot equipslots[15] =
		{
			EQL_CAP, EQL_FACEACC, EQL_EYEACC, EQL_EARRINGS, EQL_TOP, EQL_TOP,
			EQL_PANTS, EQL_SHOES, EQL_GLOVES, EQL_SHIELD, EQL_CAPE, EQL_RING,
			EQL_PENDANT, EQL_BELT, EQL_MEDAL
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
			eqslot = EQL_WEAPON;

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
			eqslot = EQL_NONE;
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

				reqstats[MS_LEVEL] = stancenode["reqLevel"];
				reqstats[MS_JOB] = stancenode["reqJob"];
				reqstats[MS_STR] = stancenode["reqSTR"];
				reqstats[MS_DEX] = stancenode["reqDEX"];
				reqstats[MS_INT] = stancenode["reqINT"];
				reqstats[MS_LUK] = stancenode["reqLUK"];

				defstats[ES_STR] = stancenode["incSTR"];
				defstats[ES_DEX] = stancenode["incDEX"];
				defstats[ES_INT] = stancenode["incINT"];
				defstats[ES_LUK] = stancenode["incLUK"];
				defstats[ES_WATK] = stancenode["incPAD"];
				defstats[ES_WDEF] = stancenode["incPDD"];
				defstats[ES_MAGIC] = stancenode["incMAD"];
				defstats[ES_MDEF] = stancenode["incMDD"];
				defstats[ES_HP] = stancenode["incMHP"];
				defstats[ES_MP] = stancenode["incMMP"];
				defstats[ES_ACC] = stancenode["incACC"];
				defstats[ES_AVOID] = stancenode["incEVA"];
				defstats[ES_HANDS] = stancenode["incHANDS"];
				defstats[ES_SPEED] = stancenode["incSPEED"];
				defstats[ES_JUMP] = stancenode["incJUMP"];
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

							stances[stance][z][frame].setshift(shift);
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
		eqslot = EQL_NONE;
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

	const string& Clothing::gettype() const
	{
		return type;
	}

	Equipslot Clothing::geteqslot() const
	{
		return eqslot;
	}
}
