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

#include "../Constants.h"

#include "../../Data/ItemData.h"

#include "nlnx/nx.hpp"
#include "nlnx/node.hpp"

namespace jrc
{
	// TODO: Add blue dot next to name
	ItemTooltip::ItemTooltip()
	{
		nl::node Item = nl::nx::ui["UIToolTip.img"]["Item"];
		nl::node Frame = Item["Frame2"];
		nl::node ItemIcon = Item["ItemIcon"];

		frame = Frame;
		cover = Frame["cover"];
		base = ItemIcon["base"];
		itemcover = ItemIcon["cover"];
		type[true] = ItemIcon["new"];
		type[false] = ItemIcon["old"];

		itemid = 0;
	}

	bool ItemTooltip::set_item(int32_t iid)
	{
		if (itemid == iid)
			return false;

		itemid = iid;

		if (itemid == 0)
			return false;

		const ItemData& idata = ItemData::get(itemid);

		itemicon = idata.get_icon(false);

		name = Text(Text::Font::A12B, Text::Alignment::LEFT, Text::Color::WHITE, idata.get_name(), 240);
		desc = Text(Text::Font::A12M, Text::Alignment::LEFT, Text::Color::WHITE, idata.get_desc(), 185);

		fillwidth = 264;
		fillheight = 83 + name.height();

		int16_t descdelta = desc.height() - 80;

		if (descdelta > 0)
			fillheight += descdelta;

		return true;
	}

	void ItemTooltip::draw(Point<int16_t> pos) const
	{
		if (itemid == 0)
			return;

		int16_t max_width = Constants::Constants::get().get_viewwidth();
		int16_t max_height = Constants::Constants::get().get_viewheight();
		int16_t cur_width = pos.x() + fillwidth + 32;
		int16_t cur_height = pos.y() + fillheight + 40;

		int16_t adj_x = cur_width - max_width;
		int16_t adj_y = cur_height - max_height;

		if (adj_x > 0)
			pos.shift_x(adj_x * -1);

		if (adj_y > 0)
			pos.shift_y(adj_y * -1);

		frame.draw(pos + Point<int16_t>(150, 118), fillwidth, fillheight);
		cover.draw(pos + Point<int16_t>(4, 4));
		name.draw(pos + Point<int16_t>(22, 8));

		pos.shift(14, 18 + name.height());

		base.draw(pos);
		type[true].draw(pos);
		itemicon.draw(DrawArgument(pos + Point<int16_t>(8, 72), 2.0f, 2.0f));
		itemcover.draw(pos);
		desc.draw(pos + Point<int16_t>(90, -6));
	}
}