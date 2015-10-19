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
#include "Texture.h"
#include "Ptrmap.h"

using namespace Graphics;

namespace Data
{
	class ItemData
	{
	public:
		ItemData(int);
		ItemData();
		virtual ~ItemData(){}
		bool isloaded() const;
		int getid() { return itemid; }
		string getname() { return name; }
		string getdesc() { return desc; }
		Texture* geticon(bool r) { return icons.get(r); }
	protected:
		void geteqcategory(int);
		string category;
	private:
		Ptrmap<bool, Texture> icons;
		int itemid;
		string name;
		string desc;
		bool loaded;
	};
}

