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
#include "Graphics\Texture.h"
#include <map>

namespace Character
{
	using::std::int32_t;
	using::std::string;
	using::std::map;
	using::Graphics::Texture;

	// Class that represents an item loaded from the game's files. Contains all shared data between concrete items.
	class ItemData
	{
	public:
		// Creates an item from the game's Item.nx with the specified id.
		ItemData(int32_t itemid);
		// A default item, which is used when an item is requested by the server, but does not exist in the game files.
		ItemData();
		// Empty destructor.
		virtual ~ItemData();
		// Returns wether the item was loaded correctly. For the default item, this is 'false'.
		bool isloaded() const;
		// Returns the item id.
		int32_t getid() const;
		// Returns the item's name loaded from the String.nx file.
		string getname() const;
		// Returns the item's description loaded from the String.nx file.
		string getdesc() const;
		// Returns one of the item's icons. For each item there is a 'raw' icon and an icon with a drop shadow.
		const Texture& geticon(bool raw) const;

	protected:
		string getcategory() const;

	private:
		string geteqcategory(int32_t) const;

		map<bool, Texture> icons;
		int32_t itemid;
		string name;
		string desc;
		string category;
		bool loaded;
	};
}

