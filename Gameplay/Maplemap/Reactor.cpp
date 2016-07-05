//////////////////////////////////////////////////////////////////////////////
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
#include "Reactor.h"

#include "../../Util/Misc.h"

#include "nlnx/nx.hpp"

namespace jrc
{
	Reactor::Reactor(int32_t o, int32_t r, int8_t s, Point<int16_t> p)
		: MapObject(o, p), rid(r), state(s) {

		std::string strid = string_format::extend_id(rid, 7);
		nl::node src = nl::nx::reactor[strid + ".img"];

		normal = src["0"];
	}

	void Reactor::draw(double viewx, double viewy, float alpha) const
	{
		Point<int16_t> absp = phobj.get_absolute(viewx, viewy, alpha);
		Point<int16_t> shift = { 0, normal.get_dimensions().y() / 2 };
		normal.draw(absp - shift, alpha);
	}

	void Reactor::destroy(int8_t, Point<int16_t>)
	{
		deactivate();
	}
}