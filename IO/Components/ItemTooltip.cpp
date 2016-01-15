//////////////////////////////////////////////////////////////////////////////
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
#include "ItemTooltip.h"
#include "Data\DataFactory.h"
#include "nlnx\nx.hpp"
#include "nlnx\node.hpp"

namespace IO
{
	ItemTooltip::ItemTooltip()
	{
		node itemtt = nl::nx::ui["UIToolTip.img"]["Item"];

		top = Texture(itemtt["Frame"]["top"]);
		mid = Texture(itemtt["Frame"]["line"]);
		line = Texture(itemtt["Frame"]["dotline"]);
		bot = Texture(itemtt["Frame"]["bottom"]);
		base = Texture(itemtt["ItemIcon"]["base"]);
		cover = Texture(itemtt["ItemIcon"]["cover"]);
		shade = Texture(itemtt["ItemIcon"]["shade"]);

		itemid = 0;
	}

	ItemTooltip::~ItemTooltip() {}

	void ItemTooltip::setitem(int32_t iid)
	{
		if (itemid == iid)
			return;

		itemid = iid;

		if (itemid == 0)
			return;

		using Character::ItemData;
		const ItemData& idata = Data::getitemdata(itemid);

		itemicon = idata.geticon(false);
		name = Text(Text::A12B, Text::CENTER, Text::WHITE);
		name.settext(idata.getname(), 240);
		desc = Itemtext(idata.getdesc(), 150);

		filllength = 81 + name.getheight();
		int16_t descdelta = desc.getheight() - 80;
		if (descdelta > 0)
			filllength += descdelta;
	}

	void ItemTooltip::draw(vector2d<int16_t> pos) const
	{
		if (itemid == 0)
			return;

		using Graphics::DrawArgument;

		top.draw(pos);
		mid.draw(DrawArgument(pos + vector2d<int16_t>(0, 13), vector2d<int16_t>(0, filllength)));
		bot.draw(pos + vector2d<int16_t>(0, filllength + 13));

		name.draw(pos + vector2d<int16_t>(130, 3));

		pos.shifty(4 + name.getheight());

		base.draw(pos + vector2d<int16_t>(10, 10));
		shade.draw(pos + vector2d<int16_t>(10, 10));
		itemicon.draw(
			DrawArgument(pos + vector2d<int16_t>(20, 82), 2.0f, 2.0f)
			);
		cover.draw(pos + vector2d<int16_t>(10, 10));

		desc.draw(pos + vector2d<int16_t>(100, 6));
	}
}