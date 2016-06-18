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
#pragma once
#include "..\..\Graphics\Texture.h"
#include "..\..\Util\BoolPair.h"

namespace jrc
{
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
		bool is_loaded() const;
		// Returns the item id.
		int32_t get_id() const;
		// Returns the item price.
		int32_t getprice() const;
		// Returns the item's name loaded from the String.nx file.
		std::string get_name() const;
		// Returns the item's description loaded from the String.nx file.
		std::string getdesc() const;
		// Returns one of the item's icons. For each item there is a 'raw' icon and an icon with a drop shadow.
		const Texture& geticon(bool raw) const;

	protected:
		std::string getcategory() const;

	private:
		std::string geteqcategory(int32_t) const;

		BoolPair<Texture> icons;
		int32_t itemid;
		int32_t price;
		std::string name;
		std::string desc;
		std::string category;
		bool loaded;
	};
}

