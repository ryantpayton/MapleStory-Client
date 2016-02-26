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
#include "Graphics\Texture.h"
#include "Constants.h"
#include <unordered_map>

namespace IO
{
	using std::unordered_map;
	using Graphics::Texture;

	class BuffIcon
	{
	public:
		BuffIcon(int32_t buff, int32_t dur);
		BuffIcon();
		~BuffIcon();

		void draw(vector2d<int16_t> position) const;

		bool update();

	private:
		int32_t buffid;
		int32_t duration;
		float opacity;
	};

	class UIBuffList : public UIElement
	{
	public:
		UIBuffList();

		void draw(float inter) const override;
		void update() override;
		Cursor::State sendmouse(bool pressed, vector2d<int16_t> position) override;

		void addbuff(int32_t buffid, int32_t duration);

	private:
		unordered_map<int32_t, BuffIcon> icons;
	};

	class ElementBuffList : public Element
	{
		UIElement::Type type() const override
		{
			return UIElement::BUFFLIST;
		}

		UIElement* instantiate() const override
		{
			return new UIBuffList();
		}
	};
}