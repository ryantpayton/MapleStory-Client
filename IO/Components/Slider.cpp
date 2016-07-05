/////////////////////////////////////////////////////////////////////////////
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
#include "Slider.h"

#include "nlnx/nx.hpp"

namespace jrc
{
	Slider::Slider(int32_t type, Range<int16_t> ver, int16_t xp, 
		int16_t ur, int16_t rm, std::function<void(bool)> om) {

		vertical = ver;
		x = xp;
		onmoved = om;

		start = { x, vertical.first() };
		end = { x, vertical.second() };

		nl::node src = nl::nx::ui["Basic.img"]["VScr" + std::to_string(type)];

		nl::node dsrc = src["disabled"];

		dbase = dsrc["base"];
		dnext = dsrc["next"];
		dprev = dsrc["prev"];

		nl::node esrc = src["enabled"];

		base = esrc["base"];

		prev = { esrc["prev0"], esrc["prev1"], start };
		next = { esrc["next0"], esrc["next1"], end };
		thumb = { esrc["thumb0"], esrc["thumb1"] };

		buttonheight = dnext.get_dimensions().y();

		setrows(ur, rm);

		enabled = true;
		scrolling = false;
	}

	Slider::Slider()
		: Slider(0, {}, 0, 0, 0, {}) {}

	bool Slider::isenabled() const
	{
		return enabled;
	}

	void Slider::setenabled(bool en)
	{
		enabled = en;
	}

	void Slider::setrows(int16_t nr, int16_t ur, int16_t rm)
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
		row = nr;
	}

	void Slider::setrows(int16_t ur, int16_t rm)
	{
		setrows(0, ur, rm);
	}

	void Slider::setvertical(Range<int16_t> ver)
	{
		vertical = ver;
		start = { x, vertical.first() };
		end = { x, vertical.second() };
		prev.set_position(start);
		next.set_position(end);
		if (rowmax > 0)
		{
			rowheight = (vertical.length() - buttonheight * 2) / rowmax;
		}
		else
		{
			rowheight = 0;
		}
	}

	void Slider::draw(Point<int16_t> position) const
	{
		Point<int16_t> fill(0, vertical.length() + buttonheight);

		if (enabled)
		{
			base.draw({ position + start, fill });
			if (rowheight > 0)
			{
				thumb.draw({ position + getthumbpos() });
			}
			prev.draw({ position });
			next.draw({ position });
		}
		else
		{
			dbase.draw({ position + start, fill });
			dprev.draw({ position });
			dnext.draw({ position });
		}
	}

	bool Slider::remove_cursor(bool clicked)
	{
		if (scrolling)
		{
			return scrolling = clicked;
		}
		else
		{
			thumb.set_state(Button::NORMAL);
			next.set_state(Button::NORMAL);
			prev.set_state(Button::NORMAL);
			return false;
		}
	}

	Point<int16_t> Slider::getthumbpos() const
	{
		int16_t y = row < rowmax ?
			vertical.first() + row * rowheight + buttonheight
			: vertical.second() - buttonheight * 2 - 2;
		return{ x, y };
	}

	Cursor::State Slider::send_cursor(Point<int16_t> cursor, bool pressed)
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
			thumb.set_state(Button::NORMAL);
			next.set_state(Button::NORMAL);
			prev.set_state(Button::NORMAL);
			return Cursor::IDLE;
		}

		Point<int16_t> thumbpos = getthumbpos();
		if (thumb.bounds(thumbpos).contains(cursor))
		{
			if (pressed)
			{
				scrolling = true;
				thumb.set_state(Button::PRESSED);
				return Cursor::CLICKING;
			}
			else
			{
				thumb.set_state(Button::MOUSEOVER);
				return Cursor::VSCROLL;
			}
		}
		else
		{
			thumb.set_state(Button::NORMAL);
		}

		if (prev.bounds(Point<int16_t>()).contains(cursor))
		{
			if (pressed)
			{
				if (row > 0)
				{
					row--;
					onmoved(true);
				}

				prev.set_state(Button::PRESSED);
				return Cursor::CLICKING;
			}
			else
			{
				prev.set_state(Button::MOUSEOVER);
				return Cursor::CANCLICK;
			}
		}
		else 
		{
			prev.set_state(Button::NORMAL);
		}

		if (next.bounds(Point<int16_t>()).contains(cursor))
		{
			if (pressed)
			{
				if (row < rowmax)
				{
					row++;
					onmoved(false);
				}

				next.set_state(Button::PRESSED);
				return Cursor::CLICKING;
			}
			else
			{
				next.set_state(Button::MOUSEOVER);
				return Cursor::CANCLICK;
			}
		}
		else
		{
			next.set_state(Button::NORMAL);
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