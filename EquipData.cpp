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
#include "EquipData.h"
#include "nx.hpp"

namespace Data
{
	EquipData::EquipData(int equipid, BodyDrawinfo* drawinfo) : ItemData(equipid)
	{
		CharacterLayer chlayer;
		int prefix = equipid / 10000;
		switch (prefix)
		{
		case 100:
			chlayer = CL_HAT;
			eqslot = EQL_CAP;
			type = "HAT";
			break;
		case 101:
			chlayer = CL_FACEACC;
			eqslot = EQL_FACEACC;
			type = "FACE ACCESSORY";
			break;
		case 102:
			chlayer = CL_EYEACC;
			eqslot = EQL_EYEACC;
			type = "EYE ACCESSORY";
			break;
		case 103:
			chlayer = CL_EARRINGS;
			eqslot = EQL_EARRINGS;
			type = "EARRINGS";
			break;
		case 104:
			chlayer = CL_TOP;
			eqslot = EQL_TOP;
			type = "TOP";
			break;
		case 105:
			chlayer = CL_MAIL;
			eqslot = EQL_TOP;
			type = "OVERALL";
			break;
		case 106:
			chlayer = CL_PANTS;
			eqslot = EQL_PANTS;
			type = "BOTTOM";
			break;
		case 107:
			chlayer = CL_SHOES;
			eqslot = EQL_SHOES;
			type = "SHOES";
			break;
		case 108:
			chlayer = CL_GLOVE;
			eqslot = EQL_GLOVES;
			type = "GLOVES";
			break;
		case 109:
			chlayer = CL_SHIELD;
			eqslot = EQL_SHIELD;
			type = "SHIELD";
			break;
		case 110:
			chlayer = CL_CAPE;
			eqslot = EQL_CAPE;
			type = "CAPE";
			break;
		case 111:
			chlayer = CL_RING;
			eqslot = EQL_RING;
			type = "RING";
			break;
		case 112:
			chlayer = CL_PENDANT;
			eqslot = EQL_PENDANT;
			type = "PENDANT";
			break;
		case 113:
			chlayer = CL_BELT;
			eqslot = EQL_BELT;
			type = "BELT";
			break;
		case 114:
			chlayer = CL_MEDAL;
			eqslot = EQL_MEDAL;
			type = "MEDAL";
			break;
		default:
			if (prefix >= 130 && prefix <= 170)
			{
				chlayer = CL_WEAPON;
				eqslot = EQL_WEAPON;

				switch (prefix)
				{
				case 130:
					type = "ONE-HANDED SWORD";
					break;
				case 131:
					type = "ONE-HANDED AXE";
					break;
				case 132:
					type = "ONE-HANDED MACE";
					break;
				case 133:
					type = "DAGGER";
					break;
				case 137:
					type = "WAND";
					break;
				case 138:
					type = "STAFF";
					break;
				case 140:
					type = "TWO-HANDED SWORD";
					break;
				case 141:
					type = "TWO-HANDED AXE";
					break;
				case 142:
					type = "TWO-HANDED MACE";
					break;
				case 143:
					type = "SPEAR";
					break;
				case 144:
					type = "POLEARM";
					break;
				case 145:
					type = "BOW";
					break;
				case 146:
					type = "CROSSBOW";
					break;
				case 147:
					type = "CLAW";
					break;
				case 148:
					type = "KNUCKLE";
					break;
				case 149:
					type = "GUN";
					break;
				}
			}
		}

		node equipnode = nx::character[category]["0" + to_string(equipid) + ".img"];
		for (node stancenode = equipnode.begin(); stancenode != equipnode.end(); ++stancenode)
		{
			string stance = stancenode.name();
			if (stance == "info")
			{
				cash = stancenode["cash"].get_bool();
				tradeblock = stancenode["tradeBlock"].get_bool();
				price = stancenode["price"];
				slots = static_cast<uint8_t>(stancenode["tuc"]);

				reqstats[MS_LEVEL] = stancenode["reqLevel"];
				reqstats[MS_JOB] = stancenode["reqJob"];
				reqstats[MS_STR] = stancenode["reqSTR"];
				reqstats[MS_DEX] = stancenode["reqDEX"];
				reqstats[MS_INT] = stancenode["reqINT"];
				reqstats[MS_LUK] = stancenode["reqLUK"];

				defstats[ES_STR] = static_cast<short>(stancenode["incSTR"]);
				defstats[ES_DEX] = static_cast<short>(stancenode["incDEX"]);
				defstats[ES_INT] = static_cast<short>(stancenode["incINT"]);
				defstats[ES_LUK] = static_cast<short>(stancenode["incLUK"]);
				defstats[ES_WATK] = static_cast<short>(stancenode["incPAD"]);
				defstats[ES_WDEF] = static_cast<short>(stancenode["incPDD"]);
				defstats[ES_MAGIC] = static_cast<short>(stancenode["incMAD"]);
				defstats[ES_MDEF] = static_cast<short>(stancenode["incMDD"]);
				defstats[ES_HP] = static_cast<short>(stancenode["incMHP"]);
				defstats[ES_MP] = static_cast<short>(stancenode["incMMP"]);
				defstats[ES_ACC] = static_cast<short>(stancenode["incACC"]);
				defstats[ES_AVOID] = static_cast<short>(stancenode["incEVA"]);
				defstats[ES_HANDS] = static_cast<short>(stancenode["incHANDS"]);
				defstats[ES_SPEED] = static_cast<short>(stancenode["incSPEED"]);
				defstats[ES_JUMP] = static_cast<short>(stancenode["incJUMP"]);
			}
			else if (stance != "default" && stance != "backDefault")
			{
				uint8_t frame = 0;
				node framenode = stancenode[to_string(frame)];
				while (framenode.size() > 0)
				{
					for (node partnode = framenode.begin(); partnode != framenode.end(); ++partnode)
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
							else if (part == "mailArm")
							{
								z = CL_MAILARM;
							}

							stances[stance][z].add(frame, new Texture(partnode));

							string parent;
							vector2d<int> parentpos;
							node map = partnode["map"];
							for (node mapnode = map.begin(); mapnode != map.end(); mapnode++)
							{
								if (mapnode.data_type() == node::type::vector)
								{
									parent = mapnode.name();
									parentpos = vector2d<int>(mapnode.x(), mapnode.y());
								}
							}

							vector2d<int> shift = vector2d<int>();
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
								shift = drawinfo->getbodypos(stance, frame) - parentpos;
								break;
							case CL_HAT:
							case CL_EARRINGS:
							case CL_EYEACC:
								shift = drawinfo->getfacepos(stance, frame) - parentpos;
								break;
							case CL_SHIELD:
							case CL_SHIELDOHAIR:
							case CL_WEAPON:
							case CL_WEAPONOHAND:
							case CL_WEAPONOGLOVE:
								if (parent == "hand")
								{
									shift += drawinfo->getarmpos(stance, frame);
								}
								else if (parent == "handMove")
								{
									shift += drawinfo->gethandpos(stance, frame);
								}
								shift += drawinfo->getbodypos(stance, frame) - parentpos;
								break;
							}

							stances[stance][z].get(frame)->setshift(shift);
						}
					}

					frame++;
					framenode = stancenode[to_string(frame)];
				}
			}
		}

		transparent = (getid() == 1002186);
	}

	EquipData::EquipData() : ItemData(0)
	{
		eqslot = EQL_NONE;
		transparent = true;
	}
}
