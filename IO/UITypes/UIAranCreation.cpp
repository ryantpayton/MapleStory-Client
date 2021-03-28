//////////////////////////////////////////////////////////////////////////////////
//	This file is part of the continued Journey MMORPG client					//
//	Copyright (C) 2015-2019  Daniel Allendorf, Ryan Payton						//
//																				//
//	This program is free software: you can redistribute it and/or modify		//
//	it under the terms of the GNU Affero General Public License as published by	//
//	the Free Software Foundation, either version 3 of the License, or			//
//	(at your option) any later version.											//
//																				//
//	This program is distributed in the hope that it will be useful,				//
//	but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				//
//	GNU Affero General Public License for more details.							//
//																				//
//	You should have received a copy of the GNU Affero General Public License	//
//	along with this program.  If not, see <https://www.gnu.org/licenses/>.		//
//////////////////////////////////////////////////////////////////////////////////
#include "UIAranCreation.h"

#ifdef USE_NX
#include <nlnx/nx.hpp>
#endif

namespace ms
{
	UIAranCreation::UIAranCreation() : UICommonCreation("2000")
	{
		nl::node back = nl::nx::map001["Back"]["login.img"]["back"];

		sky = back["2"];
		cloud = back["27"];

		sprites_background.emplace_back(back["33"], Point<int16_t>(256, 299));
		sprites_background.emplace_back(back["34"], Point<int16_t>(587, 157));

		nl::node CustomizeChar = nl::nx::ui["Login.img"]["CustomizeChar"]["2000"];

		sprites_lookboard.emplace_back(CustomizeChar["charSet"], Point<int16_t>(473, 103));

		cloudfx = 200.0f;
	}

	void UIAranCreation::draw(float inter) const
	{
		for (size_t i = 0; i < 2; i++)
			for (size_t k = 0; k < 800; k += sky.width())
				sky.draw(Point<int16_t>(k, (400 * i) - 100));

		int16_t cloudx = static_cast<int16_t>(cloudfx) % 800;
		cloud.draw(Point<int16_t>(cloudx - cloud.width(), 310));
		cloud.draw(Point<int16_t>(cloudx, 310));
		cloud.draw(Point<int16_t>(cloudx + cloud.width(), 310));

		UICommonCreation::draw(inter);
	}

	void UIAranCreation::update()
	{
		cloudfx += 0.25f;

		UICommonCreation::update();
	}
}