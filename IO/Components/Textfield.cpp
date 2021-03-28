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
#include "Textfield.h"

#include "../UI.h"

#include <sstream>
#include <iostream>

namespace ms
{
	Textfield::Textfield()
	{
		text = "";
	}

	Textfield::Textfield(Text::Font font, Text::Alignment alignment, Color::Name text_color, Rectangle<int16_t> b, size_t limit, int16_t marker_height) : bounds(b), limit(limit)
	{
		textlabel = Text(font, alignment, text_color, "", 0, false);
		marker = ColorLine(marker_height, text_color, 1.0f, true);
		boundsoutline = ColorBox(bounds.width(), bounds.height(), Color::Name::RED, 0.5f);

		text = "";
		markerpos = 0;
		crypt = 0;
		state = State::NORMAL;
	}

	void Textfield::draw(Point<int16_t> position) const
	{
		draw(position, Point<int16_t>(0, 0));
	}

	void Textfield::draw(Point<int16_t> position, Point<int16_t> marker_adjust) const
	{
		Point<int16_t> absp = bounds.get_left_top();

#ifdef DEBUG
		boundsoutline.draw(absp);
#endif

		if (state == State::DISABLED)
			return;

		absp.shift(position);

		if (text.size() > 0)
			textlabel.draw(absp);

		if (state == State::FOCUSED && showmarker)
		{
			Point<int16_t> mpos = absp + Point<int16_t>(textlabel.advance(markerpos) - 1, 8) + marker_adjust;

			if (crypt > 0)
				mpos.shift(1, -3);

			marker.draw(mpos);
		}
	}

	void Textfield::update()
	{
		if (state == State::DISABLED)
			return;

		elapsed += Constants::TIMESTEP;

		if (elapsed > 256)
		{
			showmarker = !showmarker;
			elapsed = 0;
		}
	}

	void Textfield::update(Point<int16_t> position, Point<int16_t> dimensions)
	{
		if (state == State::DISABLED)
			return;

		bounds = Rectangle<int16_t>(position, position + dimensions);

		boundsoutline.setwidth(bounds.width());
		boundsoutline.setheight(bounds.height());

		elapsed += Constants::TIMESTEP;

		if (elapsed > 256)
		{
			showmarker = !showmarker;
			elapsed = 0;
		}
	}

	void Textfield::set_state(State st)
	{
		if (state != st)
		{
			state = st;

			if (state != State::DISABLED)
			{
				elapsed = 0;
				showmarker = true;
			}
			else
			{
				UI::get().remove_textfield();
			}

			if (state == State::FOCUSED)
				UI::get().focus_textfield(this);
		}
	}

	void Textfield::set_enter_callback(std::function<void(std::string)> onr)
	{
		onreturn = onr;
	}

	void Textfield::set_key_callback(KeyAction::Id key, std::function<void(void)> action)
	{
		callbacks[key] = action;
	}

	void Textfield::set_text_callback(std::function<void(void)> action)
	{
		ontext = action;
	}

	void Textfield::send_key(KeyType::Id type, int32_t action, bool pressed)
	{
		if (pressed)
		{
			if (type == KeyType::Id::ACTION)
			{
				switch (action)
				{
					case KeyAction::Id::LEFT:
					{
						if (markerpos > 0)
							markerpos--;

						break;
					}
					case KeyAction::Id::RIGHT:
					{
						if (markerpos < text.size())
							markerpos++;

						break;
					}
					case KeyAction::Id::BACK:
					{
						if (text.size() > 0 && markerpos > 0)
						{
							text.erase(markerpos - 1, 1);

							markerpos--;

							modifytext(text);
						}

						break;
					}
					case KeyAction::Id::RETURN:
					{
						if (onreturn)
							onreturn(text);

						break;
					}
					case KeyAction::Id::SPACE:
					{
						add_string(" ");
						break;
					}
					case KeyAction::Id::HOME:
					{
						markerpos = 0;
						break;
					}
					case KeyAction::Id::END:
					{
						markerpos = text.size();
						break;
					}
					case KeyAction::Id::DELETE:
					{
						if (text.size() > 0 && markerpos < text.size())
						{
							text.erase(markerpos, 1);

							modifytext(text);
						}

						break;
					}
					default:
					{
						if (callbacks.count(action))
							callbacks.at(action)();

						break;
					}
				}
			}
			else if (type == KeyType::Id::TEXT)
			{
				if (ontext)
				{
					if (isdigit(action) || isalpha(action))
					{
						ontext();
						return;
					}
				}

				std::stringstream ss;
				char c = static_cast<int8_t>(action);

				ss << c;

				add_string(ss.str());
			}
		}
	}

	void Textfield::add_string(const std::string& str)
	{
		for (char c : str)
		{
			if (belowlimit())
			{
				text.insert(markerpos, 1, c);

				markerpos++;

				modifytext(text);
			}
		}
	}

	void Textfield::modifytext(const std::string& t)
	{
		if (crypt > 0)
		{
			std::string crypted;
			crypted.insert(0, t.size(), crypt);

			textlabel.change_text(crypted);
		}
		else
		{
			textlabel.change_text(t);
		}

		text = t;
	}

	Cursor::State Textfield::send_cursor(Point<int16_t> cursorpos, bool clicked)
	{
		if (state == State::DISABLED)
			return Cursor::State::IDLE;

		if (bounds.contains(cursorpos))
		{
			if (clicked)
			{
				if (state == State::NORMAL)
					set_state(State::FOCUSED);

				return Cursor::State::CLICKING;
			}
			else
			{
				return Cursor::State::CANCLICK;
			}
		}
		else
		{
#ifdef DEBUG
			Point<int16_t> lt = bounds.get_left_top();
			int16_t lt_x = lt.x();
			int16_t lt_y = lt.y();

			Point<int16_t> rb = bounds.get_right_bottom();
			int16_t rb_x = rb.x();
			int16_t rb_y = rb.y();

			int16_t cur_x = cursorpos.x();
			int16_t cur_y = cursorpos.y();

			std::cout
				<< "(" << lt_x << ", " << lt_y << ") != (" << rb_x << ", " << rb_y << ") && "
				<< cur_x << " >= (" << lt_x << ") && "
				<< cur_x << " <= (" << rb_x << ") && "
				<< cur_y << " >= (" << lt_y << ") && "
				<< cur_y << " <= (" << rb_y << ")"
				<< std::endl
				<< std::endl;
#endif

			if (clicked && state == State::FOCUSED)
				set_state(State::NORMAL);

			return Cursor::State::IDLE;
		}
	}

	void Textfield::change_text(const std::string& t)
	{
		modifytext(t);

		markerpos = text.size();
	}

	void Textfield::set_cryptchar(int8_t character)
	{
		crypt = character;
	}

	bool Textfield::belowlimit() const
	{
		if (limit > 0)
		{
			return text.size() < limit;
		}
		else
		{
			uint16_t advance = textlabel.advance(text.size());

			return (advance + 50) < bounds.get_horizontal().length();
		}
	}

	const std::string& Textfield::get_text() const
	{
		return text;
	}

	bool Textfield::can_copy_paste() const
	{
		if (ontext)
		{
			ontext();

			return false;
		}
		else
		{
			return true;
		}
	}

	bool Textfield::empty() const
	{
		return text.empty();
	}

	Textfield::State Textfield::get_state() const
	{
		return state;
	}
}