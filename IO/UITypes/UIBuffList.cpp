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
#include "UIBuffList.h"
#include "Data\DataFactory.h"

namespace IO
{
	BuffIcon::BuffIcon(int32_t buff, int32_t dur)
	{
		buffid = buff;
		duration = dur;
		opacity = 1.0f;
	}

	BuffIcon::BuffIcon() {}

	BuffIcon::~BuffIcon() {}

	void BuffIcon::draw(Point<int16_t> position) const
	{
		using Character::Skill;
		using Data::DataFactory;

		bool skill = buffid >= 0;
		const Texture& texture = skill ? DataFactory::get().getskill(buffid).geticon(Skill::NORMAL)
			: DataFactory::get().getitemdata(-buffid).geticon(true);

		using Graphics::DrawArgument;
		texture.draw(DrawArgument(position, opacity));
	}

	bool BuffIcon::update()
	{
		duration -= Constants::TIMESTEP;
		if (duration < 160)
		{
			opacity -= 0.05f;
			if (opacity < 0.05f)
				return true;
		}
		return false;
	}

	UIBuffList::UIBuffList() 
	{
		position = Point<int16_t>(750, 40);
		active = true;
	}

	void UIBuffList::draw(float) const
	{
		Point<int16_t> icpos = position;
		for (auto& icon : icons)
		{
			icon.second.draw(icpos);
			icpos.shiftx(-32);
		}
	}

	void UIBuffList::update()
	{
		vector<int32_t> expiredbuffs;
		for (auto& icon : icons)
		{
			bool expired = icon.second.update();
			if (expired)
				expiredbuffs.push_back(icon.first);
		}

		for (auto& exp : expiredbuffs)
		{
			icons.erase(exp);
		}
	}

	Cursor::State UIBuffList::sendmouse(bool pressed, Point<int16_t> position)
	{
		return UIElement::sendmouse(pressed, position);
	}

	void UIBuffList::addbuff(int32_t buffid, int32_t duration)
	{
		icons[buffid] = BuffIcon(buffid, duration);
	}
}