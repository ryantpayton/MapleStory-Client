//////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2016 Daniel Allendorf                                        //
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
#include <string>

namespace IO
{
	using std::string;

	class Messages
	{
	public:
		static const size_t LENGTH = 3;
		enum Type
		{
			SCROLL_SUCCESS,
			SCROLL_FAILURE,
			SCROLL_DESTROYED
		};

		Messages()
		{
			messages[SCROLL_SUCCESS] = "The scroll lights up and it's mysterious powers have been transferred to the item.";
			messages[SCROLL_FAILURE] = "The scroll lights up but the item remains as if nothing happened.";
			messages[SCROLL_DESTROYED] = "The item has been destroyed by the overwhelming power of the scroll.";
		}

		string stringfor(Type type)
		{
			return messages[type];
		}

	private:
		string messages[LENGTH];
	};
}