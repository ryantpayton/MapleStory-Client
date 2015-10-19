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
#include "Equipcache.h"
#include "WeaponData.h"

namespace Data
{
	void Equipcache::init()
	{
		drawinfo.init();
	}

	void Equipcache::loadlook(CharLook& toload, const LookEntry& look)
	{
		toload.setbody(getbody(look.getskin()));
		toload.sethair(gethair(look.gethair()));
		toload.setface(getface(look.getface()));
		for (Equipslot e = EQL_CAP; e <= EQL_WEAPON; e = static_cast<Equipslot>(e + 1))
		{
			int equipid = look.getequip(e);
			if (equipid > 0)
			{
				toload.addequip(getequip(equipid));
			}
		}
		toload.init(&drawinfo);
	}

	BodyData* Equipcache::getbody(char skin)
	{
		if (!bodytypes.contains(skin))
		{
			bodytypes.add(skin, new BodyData(skin, drawinfo));
		}
		return bodytypes.get(skin);
	}

	HairData* Equipcache::gethair(int hairid)
	{
		if (!hairstyles.contains(hairid))
		{
			hairstyles.add(hairid, new HairData(hairid, drawinfo));
		}
		return hairstyles.get(hairid);
	}

	FaceData* Equipcache::getface(int faceid)
	{
		if (!faces.contains(faceid))
		{
			faces.add(faceid, new FaceData(faceid));
		}
		return faces.get(faceid);
	}

	const EquipData& Equipcache::getequip(int equipid)
	{
		if (!equips.contains(equipid))
		{
			int prefix = equipid / 10000;
			if (prefix > 129 && prefix < 200)
			{
				equips.add(equipid, new WeaponData(equipid, drawinfo));
			}
			else
			{
				equips.add(equipid, new EquipData(equipid, drawinfo));
			}
		}
		return *equips.get(equipid);
	}
}
