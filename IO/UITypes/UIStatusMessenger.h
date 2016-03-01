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
#include "IO\Element.h"
#include "Graphics\Text.h"
#include <vector>

namespace IO
{
	using std::string;
	using std::vector;
	using Graphics::Text;

	struct StatusInfo
	{
		Text text;
		Text shadow;
		float alpha = 1.0f;
		float lastalpha = 1.0f;

		StatusInfo(string str, Text::Color color)
		{
			text = Text(Text::A12M, Text::RIGHT, color);
			text.settext(str);
			shadow = Text(Text::A12M, Text::RIGHT, Text::BLACK);
			shadow.settext(str);
		}

		void draw(Point<int16_t> position, float inter) const
		{
			float interalpha = (1.0f - inter) * lastalpha + inter * alpha;
			shadow.draw(position + Point<int16_t>(1, 1), interalpha);
			text.draw(position, interalpha);
		}

		bool update()
		{
			lastalpha = alpha;
			alpha -= 0.00125f;
			return alpha < 0.00125f;
		}
	};

	class UIStatusMessenger : public UIElement
	{
	public:
		UIStatusMessenger();

		void draw(float inter) const override;
		void update() override;
		void showstatus(Text::Color color, string message);

	private:
		vector<StatusInfo> statusinfos;
	};

	class ElementStatusMessenger : public Element
	{
		UIElement::Type type() const override
		{
			return UIElement::STATUSMESSENGER;
		}

		UIElement* instantiate() const override
		{
			return new UIStatusMessenger();
		}
	};
}