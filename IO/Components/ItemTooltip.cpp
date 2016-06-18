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
#include "ItemTooltip.h"

#include "..\..\Data\DataFactory.h"

#include "nlnx\nx.hpp"
#include "nlnx\node.hpp"

namespace jrc
{
	ItemTooltip::ItemTooltip()
	{
		nl::node itemtt = nl::nx::ui["UIToolTip.img"]["Item"];

		top = itemtt["Frame"]["top"];
		mid = itemtt["Frame"]["line"];
		line = itemtt["Frame"]["dotline"];
		bot = itemtt["Frame"]["bottom"];
		base = itemtt["ItemIcon"]["base"];
		cover = itemtt["ItemIcon"]["cover"];
		shade = itemtt["ItemIcon"]["shade"];

		itemid = 0;
	}

	void ItemTooltip::set_item(int32_t iid)
	{
		if (itemid == iid)
			return;

		itemid = iid;

		if (itemid == 0)
			return;

		const ItemData& idata = DataFactory::get().get_itemdata(itemid);

		itemicon = idata.geticon(false);
		name = { Text::A12B, Text::CENTER, Text::WHITE, idata.get_name(), 240 };
		desc = { Text::A12M, Text::LEFT, Text::WHITE, idata.getdesc(), 150 };

		filllength = 81 + name.height();
		int16_t descdelta = desc.height() - 80;
		if (descdelta > 0)
			filllength += descdelta;
	}

	void ItemTooltip::draw(Point<int16_t> pos) const
	{
		if (itemid == 0)
			return;

		top.draw(pos);
		mid.draw({ pos + Point<int16_t>(0, 13), Point<int16_t>(0, filllength) });
		bot.draw(pos + Point<int16_t>(0, filllength + 13));

		name.draw(pos + Point<int16_t>(130, 3));

		pos.shift_y(4 + name.height());

		base.draw(pos + Point<int16_t>(10, 10));
		shade.draw(pos + Point<int16_t>(10, 10));
		itemicon.draw({ pos + Point<int16_t>(20, 82), 2.0f, 2.0f });
		cover.draw(pos + Point<int16_t>(10, 10));

		desc.draw(pos + Point<int16_t>(100, 6));
	}
}