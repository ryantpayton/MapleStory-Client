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
		static const Layer layers[15] = 
		{
			Layer::HAT, Layer::FACEACC, Layer::EYEACC, Layer::EARRINGS, Layer::TOP, Layer::MAIL,
			Layer::PANTS, Layer::SHOES, Layer::GLOVE, Layer::SHIELD, Layer::CAPE, Layer::RING,
			Layer::PENDANT, Layer::BELT, Layer::MEDAL
		};

		static const Equipslot::Value equipslots[15] =
		{
			Equipslot::CAP, Equipslot::FACEACC, Equipslot::EYEACC, Equipslot::EARRINGS, Equipslot::TOP, Equipslot::TOP,
			Equipslot::PANTS, Equipslot::SHOES, Equipslot::GLOVES, Equipslot::SHIELD, Equipslot::CAPE, Equipslot::RING,
			Equipslot::PENDANT, Equipslot::BELT, Equipslot::MEDAL
		};

		static const string equiptypes[15] = 
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
			chlayer = Layer::CAPE;
			eqslot = Equipslot::NONE;
			type = "";
		}

		using nl::node;
		node equipnode = nl::nx::character[getcategory()]["0" + std::to_string(equipid) + ".img"];

		node infonode = equipnode["info"];
		cash = infonode["cash"].get_bool();
		tradeblock = infonode["tradeBlock"].get_bool();
		price = infonode["price"];
		slots = infonode["tuc"];
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
			string stancename = Stance::nameof(stance);
			node stancenode = equipnode[stancename];
			if (stancenode.size() == 0)
				continue;

			uint8_t frame = 0;
			node framenode = stancenode[std::to_string(frame)];
			while (framenode.size() > 0)
			{
				for (node partnode : framenode)
				{
					string part = partnode.name();
					if (partnode.data_type() == node::type::bitmap)
					{
						Layer z = chlayer;
						string zs = partnode["z"];
						if (part == "mailArm")
							z = Layer::MAILARM;
						else
							z = sublayer(chlayer, zs);

						string parent;
						Point<int16_t> parentpos;
						for (node mapnode : partnode["map"])
						{
							if (mapnode.data_type() == node::type::vector)
							{
								parent = mapnode.name();
								parentpos = Point<int16_t>(mapnode);
							}
						}

						Point<int16_t> shift;
						switch (z)
						{
						case Layer::FACEACC:
							shift -= parentpos;
							break;
						case Layer::TOP:
						case Layer::PANTS:
						case Layer::CAPE:
						case Layer::SHOES:
						case Layer::MAIL:
						case Layer::MAILARM:
						case Layer::GLOVE:
						case Layer::BACKWEAPON:
						case Layer::BACKSHIELD:
							shift = drawinfo.getbodypos(stance, frame) - parentpos;
							break;
						case Layer::HAT:
						case Layer::HATOVERHAIR:
						case Layer::EARRINGS:
						case Layer::EYEACC:
							shift = drawinfo.getfacepos(stance, frame) - parentpos;
							break;
						case Layer::SHIELD:
						case Layer::SHIELDOHAIR:
						case Layer::WEAPON:
						case Layer::WEAPONOHAND:
						case Layer::WEAPONOGLOVE:
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
						stances[stance][z][frame] = partnode;
						stances[stance][z][frame].shift(shift);
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
		if (!stances[stance].count(layer))
			return;

		if (!stances[stance].at(layer).count(frame))
			return;

		stances[stance].at(layer).at(frame).draw(args);
	}

	bool Clothing::islayer(Stance::Value stance, Layer layer) const
	{
		return stances[stance].count(layer) > 0;
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

	string Clothing::gettype() const
	{
		return type;
	}

	Equipslot::Value Clothing::geteqslot() const
	{
		return eqslot;
	}
}
