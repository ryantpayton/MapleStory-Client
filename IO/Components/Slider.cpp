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
#include "Slider.h"
#include "TwoSpriteButton.h"
#include "nlnx\nx.hpp"

namespace IO
{
	Slider::Slider(int32_t type, Range<int16_t> ver, int16_t xp, int16_t ur, int16_t rm, function<void(bool)> om)
	{
		vertical = ver;
		x = xp;
		onmoved = om;

		start = Point<int16_t>(x, vertical.first());
		end = Point<int16_t>(x, vertical.second());

		node src = nl::nx::ui["Basic.img"]["VScr" + std::to_string(type)];

		node dsrc = src["disabled"];
		dbase = dsrc["base"];
		dnext = dsrc["next"];
		dprev = dsrc["prev"];

		node esrc = src["enabled"];
		base = esrc["base"];

		next = unique_ptr<Button>(new TwoSpriteButton(esrc["next0"], esrc["next1"], end));
		prev = unique_ptr<Button>(new TwoSpriteButton(esrc["prev0"], esrc["prev1"], start));
		thumb = unique_ptr<Button>(new TwoSpriteButton(esrc["thumb0"], esrc["thumb1"]));

		buttonheight = dnext.getdimensions().y();

		setrows(ur, rm);

		enabled = true;
		scrolling = false;
	}

	bool Slider::isenabled() const
	{
		return enabled;
	}

	void Slider::setenabled(bool en)
	{
		enabled = en;
	}

	void Slider::setrows(int16_t ur, int16_t rm)
	{
		rowmax = rm - ur;
		if (rowmax > 0)
		{
			rowheight = (vertical.length() - buttonheight * 2) / rowmax;
		}
		else
		{
			rowheight = 0;
		}
		row = 0;
	}

	void Slider::setvertical(Range<int16_t> ver)
	{
		vertical = ver;
		start = Point<int16_t>(x, vertical.first());
		end = Point<int16_t>(x, vertical.second());
		rowheight = (vertical.length() - buttonheight * 2) / rowmax;
	}

	void Slider::draw(Point<int16_t> position) const
	{
		Point<int16_t> fill = Point<int16_t>(0, vertical.length() + buttonheight);

		using Graphics::DrawArgument;
		if (enabled)
		{
			base.draw(DrawArgument(start + position, fill));
			if (rowheight > 0)
			{
				thumb->draw(getthumbpos() + position);
			}
			prev->draw(position);
			next->draw(position);
		}
		else
		{
			dbase.draw(DrawArgument(start + position, fill));
			dprev.draw(position);
			dnext.draw(position);
		}
	}

	Point<int16_t> Slider::getthumbpos() const
	{
		int16_t y = row < rowmax ?
			vertical.first() + row * rowheight + buttonheight
			: vertical.second() - buttonheight * 2 - 2;
		return Point<int16_t>(x, y);
	}

	Cursor::State Slider::sendcursor(Point<int16_t> cursor, bool pressed)
	{
		Point<int16_t> relative = cursor - start;
		if (scrolling)
		{
			if (pressed)
			{
				int16_t thumby = row * rowheight + buttonheight * 2;
				int16_t delta = relative.y() - thumby;
				if (delta > rowheight / 2 && row < rowmax)
				{
					row++;
					onmoved(false);
				}
				else if (delta < -rowheight / 2 && row > 0)
				{
					row--;
					onmoved(true);
				}
				return Cursor::CLICKING;
			}
			else
			{
				scrolling = false;
			}
		}
		else if (relative.x() < 0 || relative.y() < 0 || relative.x() > 8 || relative.y() > vertical.second())
		{
			thumb->setstate(Button::NORMAL);
			next->setstate(Button::NORMAL);
			prev->setstate(Button::NORMAL);
			return Cursor::IDLE;
		}

		Point<int16_t> thumbpos = getthumbpos();
		if (thumb->bounds(thumbpos).contains(cursor))
		{
			if (pressed)
			{
				scrolling = true;
				thumb->setstate(Button::PRESSED);
				return Cursor::CLICKING;
			}
			else
			{
				thumb->setstate(Button::MOUSEOVER);
				return Cursor::VSCROLL;
			}
		}
		else
		{
			thumb->setstate(Button::NORMAL);
		}

		if (prev->bounds(Point<int16_t>()).contains(cursor))
		{
			if (pressed)
			{
				if (row > 0)
				{
					row--;
					onmoved(true);
				}

				prev->setstate(Button::PRESSED);
				return Cursor::CLICKING;
			}
			else
			{
				prev->setstate(Button::MOUSEOVER);
				return Cursor::CANCLICK;
			}
		}
		else 
		{
			prev->setstate(Button::NORMAL);
		}

		if (next->bounds(Point<int16_t>()).contains(cursor))
		{
			if (pressed)
			{
				if (row < rowmax)
				{
					row++;
					onmoved(false);
				}

				next->setstate(Button::PRESSED);
				return Cursor::CLICKING;
			}
			else
			{
				next->setstate(Button::MOUSEOVER);
				return Cursor::CANCLICK;
			}
		}
		else
		{
			next->setstate(Button::NORMAL);
		}

		if (pressed)
		{
			auto yoffset = static_cast<double>(relative.y() - buttonheight * 2);
			auto cursorrow = static_cast<int16_t>(std::round(yoffset / rowheight));
			if (cursorrow < 0)
				cursorrow = 0;
			else if (cursorrow > rowmax)
				cursorrow = rowmax;
			int16_t delta = row - cursorrow;
			while (delta > 0)
			{
				delta--;
				onmoved(true);
			}
			while (delta < 0)
			{
				delta++;
				onmoved(false);
			}
			row = cursorrow;
		}

		return Cursor::IDLE;
	}
}