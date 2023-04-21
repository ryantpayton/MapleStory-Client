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
#include "UIChatBar.h"

#include "../UI.h"

#include "../Components/MapleButton.h"

#include "../../Net/Packets/MessagingPackets.h"

#ifdef USE_NX
#include <nlnx/nx.hpp>
#endif

namespace ms
{
	UIChatBar::UIChatBar() : temp_view_x(0), temp_view_y(0), drag_direction(DragDirection::NONE), view_input(false), view_adjusted(false), position_adjusted(false)
	{
		nl::node ingame = nl::nx::UI["StatusBar3.img"]["chat"]["ingame"];
		nl::node input = ingame["input"];

		nl::node view = ingame["view"];
		nl::node max = view["max"];
		nl::node min = view["min"];

		drag = view["drag"];

		max_textures.emplace_back(max["top"]);
		max_textures.emplace_back(max["center"]);
		max_textures.emplace_back(max["bottom"]);

		min_textures.emplace_back(min["top"]);
		min_textures.emplace_back(min["center"]);
		min_textures.emplace_back(min["bottom"]);

		min_x = min_textures[0].width();
		max_x = max_textures[0].width();
		top_y = min_textures[0].height();
		center_y = min_textures[1].height();
		bottom_y = min_textures[2].height();
		btMin_x = Texture(view["btMin"]["normal"]["0"]).width();

		input_textures.emplace_back(input["layer:backgrnd"]);
		input_textures.emplace_back(input["layer:chatEnter"]);

		input_bg_x = input_textures[0].width();
		input_bg_y = input_textures[0].height();
		input_max_x = input_textures[1].width();

		Point<int16_t> input_origin = input_textures[1].get_origin().abs();
		input_origin_x = input_origin.x();
		input_origin_y = input_origin.y();

		min_view_y = Constants::Constants::get().get_viewheight() - input_bg_y;
		user_view_x = Setting<ChatViewX>::get().load();

		if (user_view_x == 0)
			user_view_x = min_x;

		user_view_y = Setting<ChatViewY>::get().load();

		if (user_view_y == 0)
		{
			// 20 pixels for the extra height by default
			user_view_y = top_y + center_y + bottom_y + 20;
		}

		int16_t btMax_x = Texture(view["btMax"]["normal"]["0"]).width();

		// Five pixels left and seven pixels up for padding
		buttons[Buttons::BtMax] = std::make_unique<MapleButton>(view["btMax"], Point<int16_t>(min_x - btMax_x - 5, -7));
		buttons[Buttons::BtMin] = std::make_unique<MapleButton>(view["btMin"]);

		Point<int16_t> input_btns_pos = Point<int16_t>(input_max_x - (input_bg_x - user_view_x) + input_origin_x - 17, 15 + input_origin_y + 1);
		int16_t input_btns_padding = 3;
		input_btns_x = Texture(input["button:chat"]["normal"]["0"]).width() + input_btns_padding;

		buttons[Buttons::BtChat] = std::make_unique<MapleButton>(input["button:chat"], input_btns_pos + Point<int16_t>(input_btns_x * 0, 0));
		buttons[Buttons::BtItemLink] = std::make_unique<MapleButton>(input["button:itemLink"], input_btns_pos + Point<int16_t>(input_btns_x * 1, 0));
		buttons[Buttons::BtChatEmoticon] = std::make_unique<MapleButton>(input["button:chatEmoticon"], input_btns_pos + Point<int16_t>(input_btns_x * 2, 0));
		buttons[Buttons::BtHelp] = std::make_unique<MapleButton>(input["button:help"], input_btns_pos + Point<int16_t>(input_btns_x * 3, 0));
		buttons[Buttons::BtOutChat] = std::make_unique<MapleButton>(input["button:outChat"], input_btns_pos + Point<int16_t>(input_btns_x * 4, 0));

		buttons[Buttons::BtChat]->set_active(false);
		buttons[Buttons::BtItemLink]->set_active(false);
		buttons[Buttons::BtChatEmoticon]->set_active(false);
		buttons[Buttons::BtHelp]->set_active(false);
		buttons[Buttons::BtOutChat]->set_active(false);

		int16_t input_text_limit = 70;
		int16_t input_text_marker_height = 11;

		input_text = Textfield(
			Text::Font::A11M, Text::Alignment::LEFT, Color::Name::WHITE,
			Rectangle<int16_t>(get_input_text_position(), get_input_text_position() + Point<int16_t>(283, INPUT_TEXT_HEIGHT)),
			input_text_limit, input_text_marker_height
		);

		input_text.set_enter_callback(
			[&](std::string message)
			{
				input_text_enter_callback(message);
			}
		);

		input_text.set_key_callback(
			KeyAction::Id::ESCAPE,
			[&]()
			{
				input_text_escape_callback();
			}
		);

		input_text.set_key_callback(
			KeyAction::Id::UP,
			[&]()
			{
				change_message(true);
			}
		);

		input_text.set_key_callback(
			KeyAction::Id::DOWN,
			[&]()
			{
				change_message(false);
			}
		);

		dragarea = drag.get_dimensions();

		toggle_view(Setting<ChatViewMax>::get().load(), false);

#if LOG_LEVEL >= LOG_UI
		dragarea_box = ColorBox(dragarea.x(), dragarea.y(), Color::Name::BLUE, 0.5f);
		input_box = ColorBox(input_bg_x, input_bg_y, Color::Name::RED, 0.5f);
#endif

		show_message("[Welcome] Welcome to MapleStory!!", MessageType::YELLOW);
	}

	void UIChatBar::draw(float inter) const
	{
		if (view_max)
		{
			Range<int16_t> vertical = Range<int16_t>(0, 0);

			max_textures[0].draw(position - Point<int16_t>(0, center_y + user_view_y), vertical, Range<int16_t>(0, max_x - user_view_x + 5));
			max_textures[0].draw(position - Point<int16_t>(max_x - user_view_x, center_y + user_view_y), vertical, Range<int16_t>(max_x - 5, 0));

			max_textures[1].draw(DrawArgument(position - Point<int16_t>(0, center_y + user_view_y), Point<int16_t>(0, user_view_y + 1)), vertical, Range<int16_t>(0, max_x - user_view_x + 2));
			max_textures[1].draw(DrawArgument(position - Point<int16_t>(max_x - user_view_x, center_y + user_view_y), Point<int16_t>(0, user_view_y + 1)), vertical, Range<int16_t>(max_x - 2, 0));

			max_textures[2].draw(position, vertical, Range<int16_t>(0, max_x - user_view_x + 5));
			max_textures[2].draw(position - Point<int16_t>(max_x - user_view_x, 0), vertical, Range<int16_t>(max_x - 5, 0));

			drag.draw(position - Point<int16_t>(0, top_y + center_y + user_view_y));

			int16_t message_y = 0;

			for (int i = message_history.size() - 1; i >= 0; i--)
			{
				if (message_y <= center_y + user_view_y)
				{
					message_history[i].text.draw(position - Point<int16_t>(drag.get_origin().x() - 2, 5) - Point<int16_t>(0, message_y));

					message_y += 13;
				}
			}

			if (view_input)
			{
				Point<int16_t> pos = get_input_position();

				input_textures[0].draw(pos, vertical, Range<int16_t>(0, input_bg_x - user_view_x + 5));
				input_textures[0].draw(pos - Point<int16_t>(input_bg_x - user_view_x, 0), vertical, Range<int16_t>(input_bg_x - 5, 0));

				input_textures[1].draw(pos, vertical, Range<int16_t>(0, input_max_x - (input_bg_x - user_view_x) - input_origin_x - 22 + 3));
				input_textures[1].draw(pos - Point<int16_t>(input_max_x - (input_bg_x - user_view_x) - input_origin_x - 22, 0), vertical, Range<int16_t>(input_max_x - 3, 0));

				input_text.draw(Point<int16_t>(1, -2), Point<int16_t>(1, -3));

#if LOG_LEVEL >= LOG_UI
				input_box.draw(pos);
#endif
			}

			if (dragged)
			{
				if (temp_view_y > 0 && temp_view_x == 0)
				{
					Point<int16_t> pos = position;

					if (drag_direction == DragDirection::DOWN)
						pos = temp_position;

					max_textures[0].draw(pos - Point<int16_t>(0, center_y + temp_view_y), vertical, Range<int16_t>(0, max_x - user_view_x + 5));
					max_textures[0].draw(pos - Point<int16_t>(max_x - user_view_x, center_y + temp_view_y), vertical, Range<int16_t>(max_x - 5, 0));

					max_textures[1].draw(DrawArgument(pos - Point<int16_t>(0, center_y + temp_view_y), Point<int16_t>(0, temp_view_y + 1)), vertical, Range<int16_t>(0, max_x - user_view_x + 2));
					max_textures[1].draw(DrawArgument(pos - Point<int16_t>(max_x - user_view_x, center_y + temp_view_y), Point<int16_t>(0, temp_view_y + 1)), vertical, Range<int16_t>(max_x - 2, 0));

					max_textures[2].draw(pos, vertical, Range<int16_t>(0, max_x - user_view_x + 5));
					max_textures[2].draw(pos - Point<int16_t>(max_x - user_view_x, 0), vertical, Range<int16_t>(max_x - 5, 0));
				}
				else if (temp_view_y == 0 && temp_view_x > 0)
				{
					Point<int16_t> pos = position;

					if (drag_direction == DragDirection::LEFT)
						pos = temp_position;

					max_textures[0].draw(pos - Point<int16_t>(0, center_y + user_view_y), vertical, Range<int16_t>(0, max_x - temp_view_x + 5));
					max_textures[0].draw(pos - Point<int16_t>(max_x - temp_view_x, center_y + user_view_y), vertical, Range<int16_t>(max_x - 5, 0));

					max_textures[1].draw(DrawArgument(pos - Point<int16_t>(0, center_y + user_view_y), Point<int16_t>(0, user_view_y + 1)), vertical, Range<int16_t>(0, max_x - temp_view_x + 2));
					max_textures[1].draw(DrawArgument(pos - Point<int16_t>(max_x - temp_view_x, center_y + user_view_y), Point<int16_t>(0, user_view_y + 1)), vertical, Range<int16_t>(max_x - 2, 0));

					max_textures[2].draw(pos, vertical, Range<int16_t>(0, max_x - temp_view_x + 5));
					max_textures[2].draw(pos - Point<int16_t>(max_x - temp_view_x, 0), vertical, Range<int16_t>(max_x - 5, 0));
				}
				else if (temp_view_y > 0 && temp_view_x > 0)
				{
					Point<int16_t> pos = position;

					if (drag_direction == DragDirection::DOWN || drag_direction == DragDirection::LEFT || drag_direction == DragDirection::DOWNLEFT)
						pos = temp_position;

					max_textures[0].draw(pos - Point<int16_t>(0, center_y + temp_view_y), vertical, Range<int16_t>(0, max_x - temp_view_x + 5));
					max_textures[0].draw(pos - Point<int16_t>(max_x - temp_view_x, center_y + temp_view_y), vertical, Range<int16_t>(max_x - 5, 0));

					max_textures[1].draw(DrawArgument(pos - Point<int16_t>(0, center_y + temp_view_y), Point<int16_t>(0, temp_view_y + 1)), vertical, Range<int16_t>(0, max_x - temp_view_x + 2));
					max_textures[1].draw(DrawArgument(pos - Point<int16_t>(max_x - temp_view_x, center_y + temp_view_y), Point<int16_t>(0, temp_view_y + 1)), vertical, Range<int16_t>(max_x - 2, 0));

					max_textures[2].draw(pos, vertical, Range<int16_t>(0, max_x - temp_view_x + 5));
					max_textures[2].draw(pos - Point<int16_t>(max_x - temp_view_x, 0), vertical, Range<int16_t>(max_x - 5, 0));
				}
			}
		}
		else
		{
			min_textures[0].draw(position - Point<int16_t>(0, center_y));
			min_textures[1].draw(position - Point<int16_t>(0, center_y));
			min_textures[2].draw(position);

			drag.draw(position - Point<int16_t>(0, top_y + center_y));

			const size_t size = message_history.size();

			if (size > 0)
				message_history[size - 1].text.draw(position - Point<int16_t>(0, top_y + center_y) + Point<int16_t>(drag.get_origin().abs().x(), drag.height()) + Point<int16_t>(2, 7));
		}

		UIElement::draw(inter);

#if LOG_LEVEL >= LOG_UI
		dimension_box.draw(get_position());
		dragarea_box.draw(get_dragarea_position());

		if (view_max)
		{
			top_box.draw(get_position());
			bottom_box.draw(get_position() + Point<int16_t>(0, dimension.y() - 3));
			left_box.draw(get_position());
			right_box.draw(get_position() + Point<int16_t>(dimension.x() - 3, 0));
		}
#endif
	}

	void UIChatBar::update()
	{
		input_text.update(get_input_text_position(), Point<int16_t>(input_max_x - (input_bg_x - user_view_x) - 22, INPUT_TEXT_HEIGHT));
	}

	Button::State UIChatBar::button_pressed(uint16_t buttonid)
	{
		switch (buttonid)
		{
			case ms::UIChatBar::BtMax:
			{
				toggle_view(true, true);

				return Button::State::NORMAL;
			}
			case ms::UIChatBar::BtMin:
			{
				toggle_view(false, true);

				return Button::State::NORMAL;
			}
			default:
			{
				return Button::State::DISABLED;
			}
		}
	}

	bool UIChatBar::is_in_range(Point<int16_t> cursor_position) const
	{
		if (temp_view_y == 0 && temp_view_x == 0)
		{
			Rectangle<int16_t> bounds = Rectangle<int16_t>(get_position(), get_position() + dimension);
			Rectangle<int16_t> input_bounds = Rectangle<int16_t>(get_input_position(), get_input_position() + Point<int16_t>(user_view_x, input_bg_y));

			return bounds.contains(cursor_position) || input_bounds.contains(cursor_position);
		}
		else
		{
			Rectangle<int16_t> bounds = Rectangle<int16_t>(
				Point<int16_t>(0, 0),
				Point<int16_t>(
					Constants::Constants::get().get_viewwidth(),
					Constants::Constants::get().get_viewheight()
				)
			);

			return bounds.contains(cursor_position);
		}
	}

	Cursor::State UIChatBar::send_cursor(bool clicked, Point<int16_t> cursor_position)
	{
		if (view_input && temp_view_y == 0 && temp_view_x == 0)
			if (Cursor::State new_state = input_text.send_cursor(cursor_position, clicked))
				return new_state;

		if (clicked)
		{
			if (dragged)
			{
				if (temp_view_y == 0 && temp_view_x == 0)
				{
					Point<int16_t> new_pos = cursor_position - cursoroffset;
					int16_t new_pos_x = new_pos.x();
					int16_t new_pos_y = new_pos.y();

					if (new_pos_x < 0)
						new_pos.set_x(0);

					int16_t min_y = MIN_HEIGHT - 2 + drag.get_origin().y() * -1;

					if (view_input)
						min_y += user_view_y;

					if (new_pos_y < min_y)
						new_pos.set_y(min_y);

					int16_t max_x = Constants::Constants::get().get_viewwidth() - user_view_x;

					if (new_pos_x > max_x)
						new_pos.set_x(max_x);

					int16_t max_y = min_view_y;

					if (view_input)
						max_y -= input_bg_y;

					if (new_pos_y > max_y)
						new_pos.set_y(max_y);

					position = new_pos;

					return Cursor::State::CHATBARMOVE;
				}
				else
				{
					if (temp_view_x == 0)
					{
						if (drag_direction == DragDirection::DOWN)
						{
							// TODO: The top gets shifted by a pixel
							Point<int16_t> pos_y = cursor_position - position + Point<int16_t>(0, user_view_y) - Point<int16_t>(0, 13);
							Point<int16_t> pos = cursor_position - cursoroffset;

							if (pos_y.y() <= MIN_HEIGHT)
							{
								temp_view_y = MIN_HEIGHT;
								temp_position = position - Point<int16_t>(0, user_view_y) + Point<int16_t>(0, 13);
							}
							else if (pos_y.y() >= MAX_HEIGHT)
							{
								temp_view_y = MAX_HEIGHT;
								temp_position = position - Point<int16_t>(0, user_view_y) + Point<int16_t>(0, MAX_HEIGHT);
							}
							else
							{
								temp_view_y = pos_y.y();
								temp_position = Point<int16_t>(position.x(), pos.y());
							}
						}
						else
						{
							Point<int16_t> pos = position - cursor_position - Point<int16_t>(0, 13);

							if (pos.y() <= MIN_HEIGHT)
								temp_view_y = MIN_HEIGHT;
							else if (pos.y() >= MAX_HEIGHT)
								temp_view_y = MAX_HEIGHT;
							else
								temp_view_y = pos.y();
						}

#if LOG_LEVEL >= LOG_UI
						dimension = Point<int16_t>(user_view_x, top_y + center_y + bottom_y) + Point<int16_t>(0, temp_view_y);

						dimension_box = ColorBox(dimension.x(), dimension.y(), Color::Name::RED, 0.5f);
						left_box = ColorBox(3, dimension.y(), Color::Name::YELLOW, 0.5f);
						right_box = ColorBox(3, dimension.y(), Color::Name::YELLOW, 0.5f);
#endif

						return Cursor::State::CHATBARVDRAG;
					}
					else if (temp_view_y == 0)
					{
						if (drag_direction == DragDirection::LEFT)
						{
							// TODO: The right gets shifted by a pixel
							Point<int16_t> pos_x = position - cursor_position + Point<int16_t>(user_view_x, 0);
							Point<int16_t> pos = cursor_position - cursoroffset;

							if (pos_x.x() <= min_x)
							{
								temp_view_x = min_x;
								temp_position = position + Point<int16_t>(user_view_x, 0) - Point<int16_t>(min_x, 0);
							}
							else if (pos_x.x() >= max_x)
							{
								temp_view_x = max_x;
								temp_position = position - Point<int16_t>(max_x, 0) + Point<int16_t>(user_view_x, 0);
							}
							else
							{
								temp_view_x = pos_x.x();
								temp_position = Point<int16_t>(pos.x(), position.y());
							}
						}
						else
						{
							Point<int16_t> pos = cursor_position - position;

							if (pos.x() <= min_x)
								temp_view_x = min_x;
							else if (pos.x() >= max_x)
								temp_view_x = max_x;
							else
								temp_view_x = pos.x();
						}

#if LOG_LEVEL >= LOG_UI
						dimension = Point<int16_t>(temp_view_x, top_y + center_y + bottom_y) + Point<int16_t>(0, user_view_y);

						dimension_box = ColorBox(dimension.x(), dimension.y(), Color::Name::RED, 0.5f);
						top_box = ColorBox(dimension.x(), 3, Color::Name::GREEN, 0.5f);
						bottom_box = ColorBox(dimension.x(), 3, Color::Name::GREEN, 0.5f);
#endif

						return Cursor::State::CHATBARHDRAG;
					}
					else
					{
						if (drag_direction == DragDirection::DOWNLEFT)
						{
							int16_t temp_position_x, temp_position_y;

							Point<int16_t> pos = cursor_position - cursoroffset;

							// TODO: The right gets shifted by a pixel
							Point<int16_t> pos_x = position - cursor_position + Point<int16_t>(user_view_x, 0);

							if (pos_x.x() <= min_x)
							{
								temp_view_x = min_x;
								temp_position_x = position.x() + user_view_x - min_x;
							}
							else if (pos_x.x() >= max_x)
							{
								temp_view_x = max_x;
								temp_position_x = position.x() - max_x + user_view_x;
							}
							else
							{
								temp_view_x = pos_x.x();
								temp_position_x = pos.x();
							}

							// TODO: The top gets shifted by a pixel
							Point<int16_t> pos_y = cursor_position - position + Point<int16_t>(0, user_view_y) - Point<int16_t>(0, 13);

							if (pos_y.y() <= MIN_HEIGHT)
							{
								temp_view_y = MIN_HEIGHT;
								temp_position_y = position.y() - user_view_y + 13;
							}
							else if (pos_y.y() >= MAX_HEIGHT)
							{
								temp_view_y = MAX_HEIGHT;
								temp_position_y = position.y() - user_view_y + MAX_HEIGHT;
							}
							else
							{
								temp_view_y = pos_y.y();
								temp_position_y = pos.y();
							}

							temp_position = Point<int16_t>(temp_position_x, temp_position_y);
						}
						else
						{
							if (drag_direction == DragDirection::LEFT)
							{
								// TODO: The right gets shifted by a pixel
								Point<int16_t> pos_x = position - cursor_position + Point<int16_t>(user_view_x, 0);
								Point<int16_t> pos = cursor_position - cursoroffset;

								if (pos_x.x() <= min_x)
								{
									temp_view_x = min_x;
									temp_position = position + Point<int16_t>(user_view_x, 0) - Point<int16_t>(min_x, 0);
								}
								else if (pos_x.x() >= max_x)
								{
									temp_view_x = max_x;
									temp_position = position - Point<int16_t>(max_x, 0) + Point<int16_t>(user_view_x, 0);
								}
								else
								{
									temp_view_x = pos_x.x();
									temp_position = Point<int16_t>(pos.x(), position.y());
								}
							}
							else
							{
								Point<int16_t> pos_x = cursor_position - position;

								if (pos_x.x() <= min_x)
									temp_view_x = min_x;
								else if (pos_x.x() >= max_x)
									temp_view_x = max_x;
								else
									temp_view_x = pos_x.x();
							}

							if (drag_direction == DragDirection::DOWN)
							{
								// TODO: The top gets shifted by a pixel
								Point<int16_t> pos_y = cursor_position - position + Point<int16_t>(0, user_view_y) - Point<int16_t>(0, 13);
								Point<int16_t> pos = cursor_position - cursoroffset;

								if (pos_y.y() <= MIN_HEIGHT)
								{
									temp_view_y = MIN_HEIGHT;
									temp_position = position - Point<int16_t>(0, user_view_y) + Point<int16_t>(0, 13);
								}
								else if (pos_y.y() >= MAX_HEIGHT)
								{
									temp_view_y = MAX_HEIGHT;
									temp_position = position - Point<int16_t>(0, user_view_y) + Point<int16_t>(0, MAX_HEIGHT);
								}
								else
								{
									temp_view_y = pos_y.y();
									temp_position = Point<int16_t>(position.x(), pos.y());
								}
							}
							else
							{
								Point<int16_t> pos_y = position - cursor_position - Point<int16_t>(0, 13);

								if (pos_y.y() <= MIN_HEIGHT)
									temp_view_y = MIN_HEIGHT;
								else if (pos_y.y() >= MAX_HEIGHT)
									temp_view_y = MAX_HEIGHT;
								else
									temp_view_y = pos_y.y();
							}
						}

#if LOG_LEVEL >= LOG_UI
						dimension = Point<int16_t>(temp_view_x, top_y + center_y + bottom_y) + Point<int16_t>(0, temp_view_y);

						dimension_box = ColorBox(dimension.x(), dimension.y(), Color::Name::RED, 0.5f);
						top_box = ColorBox(dimension.x(), 3, Color::Name::GREEN, 0.5f);
						bottom_box = ColorBox(dimension.x(), 3, Color::Name::GREEN, 0.5f);
						left_box = ColorBox(3, dimension.y(), Color::Name::YELLOW, 0.5f);
						right_box = ColorBox(3, dimension.y(), Color::Name::YELLOW, 0.5f);
#endif

						if (drag_direction == DragDirection::DOWN || drag_direction == DragDirection::LEFT)
							return Cursor::State::CHATBARBRTLDRAG;
						else
							return Cursor::State::CHATBARBLTRDRAG;
					}
				}
			}
			else if (indragrange(cursor_position))
			{
				cursoroffset = cursor_position - position;
				dragged = true;

				return Cursor::State::CHATBARMOVE;
			}
			else if (view_max)
			{
				if (intoprange(cursor_position) && !inleftrange(cursor_position) && !inrightrange(cursor_position))
				{
					dragged = true;

					temp_view_y = user_view_y;

					return Cursor::State::CHATBARVDRAG;
				}
				else if (inrightrange(cursor_position) && !intoprange(cursor_position) && !inbottomrange(cursor_position))
				{
					dragged = true;

					temp_view_x = user_view_x;

					return Cursor::State::CHATBARHDRAG;
				}
				else if (intoprightrange(cursor_position))
				{
					dragged = true;

					temp_view_x = user_view_x;
					temp_view_y = user_view_y;

					return Cursor::State::CHATBARBLTRDRAG;
				}
				else if (inbottomrange(cursor_position) && !inleftrange(cursor_position) && !inrightrange(cursor_position))
				{
					cursoroffset = cursor_position - position;
					dragged = true;

					temp_view_y = user_view_y;
					temp_position = position;
					drag_direction = DragDirection::DOWN;

					return Cursor::State::CHATBARVDRAG;
				}
				else if (inbottomrightrange(cursor_position))
				{
					cursoroffset = cursor_position - position;
					dragged = true;

					temp_view_x = user_view_x;
					temp_view_y = user_view_y;
					temp_position = position;
					drag_direction = DragDirection::DOWN;

					return Cursor::State::CHATBARBRTLDRAG;
				}
				else if (inleftrange(cursor_position) && !intoprange(cursor_position) && !inbottomrange(cursor_position))
				{
					cursoroffset = cursor_position - position;
					dragged = true;

					temp_view_x = user_view_x;
					temp_position = position;
					drag_direction = DragDirection::LEFT;

					return Cursor::State::CHATBARHDRAG;
				}
				else if (intopleftrange(cursor_position))
				{
					cursoroffset = cursor_position - position;
					dragged = true;

					temp_view_x = user_view_x;
					temp_view_y = user_view_y;
					temp_position = position;
					drag_direction = DragDirection::LEFT;

					return Cursor::State::CHATBARBRTLDRAG;
				}
				else if (inbottomleftrange(cursor_position))
				{
					cursoroffset = cursor_position - position;
					dragged = true;

					temp_view_x = user_view_x;
					temp_view_y = user_view_y;
					temp_position = position;
					drag_direction = DragDirection::DOWNLEFT;

					return Cursor::State::CHATBARBLTRDRAG;
				}
			}
		}
		else
		{
			if (dragged)
			{
				if (temp_view_y == 0 && temp_view_x == 0)
				{
					dragged = false;

					Setting<PosCHAT>::get().save(position);

					return Cursor::State::CHATBARMOVE;
				}
				else
				{
					if (temp_view_x == 0)
					{
						user_view_y = temp_view_y;
						temp_view_y = 0;
						dragged = false;

						update_view(false);

						Setting<ChatViewY>::get().save(user_view_y);

						if (drag_direction == DragDirection::DOWN)
						{
							drag_direction = DragDirection::NONE;

							if (temp_position.y() > min_view_y - input_bg_y)
							{
								if (view_input)
									temp_position.set_y(min_view_y - input_bg_y);
								else
									temp_position.set_y(min_view_y);
							}

							position = temp_position;

							Setting<PosCHAT>::get().save(position);
						}

						return Cursor::State::CHATBARVDRAG;
					}
					else if (temp_view_y == 0)
					{
						user_view_x = temp_view_x;
						temp_view_x = 0;
						dragged = false;

						update_view(false);

						Setting<ChatViewX>::get().save(user_view_x);

						if (drag_direction == DragDirection::LEFT)
						{
							drag_direction = DragDirection::NONE;

							position = temp_position;

							Setting<PosCHAT>::get().save(position);
						}

						return Cursor::State::CHATBARHDRAG;
					}
					else
					{
						user_view_x = temp_view_x;
						temp_view_x = 0;

						user_view_y = temp_view_y;
						temp_view_y = 0;

						dragged = false;

						update_view(false);

						Setting<ChatViewX>::get().save(user_view_x);
						Setting<ChatViewY>::get().save(user_view_y);

						if (drag_direction == DragDirection::DOWN || drag_direction == DragDirection::LEFT || drag_direction == DragDirection::DOWNLEFT)
						{
							drag_direction = DragDirection::NONE;

							if (temp_position.y() > min_view_y - input_bg_y)
							{
								if (view_input)
									temp_position.set_y(min_view_y - input_bg_y);
								else
									temp_position.set_y(min_view_y);
							}

							position = temp_position;

							Setting<PosCHAT>::get().save(position);

							if (drag_direction != DragDirection::DOWNLEFT)
								return Cursor::State::CHATBARBRTLDRAG;
						}

						return Cursor::State::CHATBARBLTRDRAG;
					}
				}
			}
			else if (indragrange(cursor_position))
			{
				return Cursor::State::CHATBARMOVE;
			}
			else if (view_max)
			{
				if (intopleftrange(cursor_position) || inbottomrightrange(cursor_position))
					return Cursor::State::CHATBARBRTLDRAG;
				else if (intoprightrange(cursor_position) || inbottomleftrange(cursor_position))
					return Cursor::State::CHATBARBLTRDRAG;
				else if (intoprange(cursor_position) || inbottomrange(cursor_position))
					return Cursor::State::CHATBARVDRAG;
				else if (inleftrange(cursor_position) || inrightrange(cursor_position))
					return Cursor::State::CHATBARHDRAG;
			}
		}

		return UIElement::send_cursor(clicked, cursor_position);
	}

	void UIChatBar::send_key(int32_t keycode, bool pressed, bool escape)
	{
		if (pressed)
		{
			if (keycode == KeyAction::Id::RETURN)
			{
				if (!view_input)
				{
					toggle_input(true);
				}
				else
				{
					input_text.change_text("");
					input_text.set_state(Textfield::State::FOCUSED);
				}
			}
			else
			{
				int32_t index = UI::get().get_keyboard().get_mapping_index(keycode);

				input_text.change_text("");
				input_text.set_state(Textfield::State::FOCUSED);
				input_text.send_key(KeyType::Id::TEXT, index, pressed);
			}
		}
	}

	UIElement::Type UIChatBar::get_type() const
	{
		return TYPE;
	}

	bool UIChatBar::has_input() const
	{
		return view_input;
	}

	void UIChatBar::toggle_view()
	{
		toggle_view(!view_max, true);
	}

	void UIChatBar::show_message(const char* message, MessageType type)
	{
		Color::Name color = Color::Name::RED;

		if (type == MessageType::YELLOW)
			color = Color::Name::YELLOW;
		else if (type == MessageType::WHITE)
			color = Color::Name::WHITE;
		else
			LOG(LOG_DEBUG, "[UIChatBar::show_message]: " << type << " not supported.");

		message_history.push_back(Message(MessageGroup::ALL, type, Text(Text::Font::A11M, Text::Alignment::LEFT, color, message)));
	}

	bool UIChatBar::indragrange(Point<int16_t> cursor_position) const
	{
		Rectangle<int16_t> bounds = Rectangle<int16_t>(get_dragarea_position(), get_dragarea_position() + dragarea);

		return bounds.contains(cursor_position);
	}

	bool UIChatBar::intoprange(Point<int16_t> cursor_position) const
	{
		Rectangle<int16_t> bounds = Rectangle<int16_t>(get_position(), get_position() + Point<int16_t>(dimension.x(), 3));

		return bounds.contains(cursor_position);
	}

	bool UIChatBar::inbottomrange(Point<int16_t> cursor_position) const
	{
		Point<int16_t> position = get_position() + Point<int16_t>(0, dimension.y() - 3);
		Rectangle<int16_t> bounds = Rectangle<int16_t>(position, position + Point<int16_t>(dimension.x(), 3));

		return bounds.contains(cursor_position);
	}

	bool UIChatBar::inleftrange(Point<int16_t> cursor_position) const
	{
		Rectangle<int16_t> bounds = Rectangle<int16_t>(get_position(), get_position() + Point<int16_t>(3, dimension.y()));

		return bounds.contains(cursor_position);
	}

	bool UIChatBar::inrightrange(Point<int16_t> cursor_position) const
	{
		Point<int16_t> position = get_position() + Point<int16_t>(dimension.x() - 3, 0);
		Rectangle<int16_t> bounds = Rectangle<int16_t>(position, position + Point<int16_t>(3, dimension.y()));

		return bounds.contains(cursor_position);
	}

	bool UIChatBar::intopleftrange(Point<int16_t> cursor_position) const
	{
		return intoprange(cursor_position) && inleftrange(cursor_position);
	}

	bool UIChatBar::inbottomrightrange(Point<int16_t> cursor_position) const
	{
		return inbottomrange(cursor_position) && inrightrange(cursor_position);
	}

	bool UIChatBar::intoprightrange(Point<int16_t> cursor_position) const
	{
		return intoprange(cursor_position) && inrightrange(cursor_position);
	}

	bool UIChatBar::inbottomleftrange(Point<int16_t> cursor_position) const
	{
		return inbottomrange(cursor_position) && inleftrange(cursor_position);
	}

	Point<int16_t> UIChatBar::get_position() const
	{
		if (temp_view_y == 0 && temp_view_x == 0)
		{
			if (view_max)
				return position - Point<int16_t>(0, top_y + center_y + user_view_y);
			else
				return position - Point<int16_t>(0, top_y + center_y);
		}
		else
		{
			if (temp_view_y > 0 && temp_view_x == 0)
			{
				if (drag_direction == DragDirection::DOWN)
					return temp_position - Point<int16_t>(0, top_y + center_y + temp_view_y);
				else
					return position - Point<int16_t>(0, top_y + center_y + temp_view_y);
			}
			else if (temp_view_y == 0 && temp_view_x > 0)
			{
				if (drag_direction == DragDirection::LEFT)
					return temp_position - Point<int16_t>(0, top_y + center_y + user_view_y);
				else
					return position - Point<int16_t>(0, top_y + center_y + user_view_y);
			}
			else
			{
				if (drag_direction == DragDirection::DOWN || drag_direction == DragDirection::LEFT || drag_direction == DragDirection::DOWNLEFT)
					return temp_position - Point<int16_t>(0, top_y + center_y + temp_view_y);
				else
					return position - Point<int16_t>(0, top_y + center_y + temp_view_y);
			}
		}
	}

	Point<int16_t> UIChatBar::get_dragarea_position() const
	{
		Point<int16_t> drag_origin = drag.get_origin();

		if (view_max)
			return position - Point<int16_t>(drag_origin.x(), top_y + center_y + user_view_y + drag_origin.y());
		else
			return position - Point<int16_t>(drag_origin.x(), top_y + center_y + drag_origin.y());
	}

	Point<int16_t> UIChatBar::get_input_position() const
	{
		return position + Point<int16_t>(0, 15);
	}

	Point<int16_t> UIChatBar::get_input_text_position()
	{
		Point<int16_t> adjust = Point<int16_t>(57, 20);

		return position + adjust;
	}

	void UIChatBar::toggle_input(bool enabled)
	{
		view_input = enabled;

		if (view_input && !view_max)
		{
			view_adjusted = true;

			toggle_view(true, true);
		}
		else
		{
			if (view_adjusted)
			{
				view_adjusted = false;

				toggle_view(false, true);
			}
			else
			{
				update_view(true);
			}
		}

		buttons[Buttons::BtChat]->set_active(view_input);
		buttons[Buttons::BtItemLink]->set_active(view_input);
		buttons[Buttons::BtChatEmoticon]->set_active(view_input);
		buttons[Buttons::BtHelp]->set_active(view_input);
		buttons[Buttons::BtOutChat]->set_active(view_input);
	}

	void UIChatBar::toggle_view(bool max, bool pressed)
	{
		view_max = max;

		if (!view_max)
		{
			view_input = false;
			view_adjusted = false;

			buttons[Buttons::BtChat]->set_active(view_input);
			buttons[Buttons::BtItemLink]->set_active(view_input);
			buttons[Buttons::BtChatEmoticon]->set_active(view_input);
			buttons[Buttons::BtHelp]->set_active(view_input);
			buttons[Buttons::BtOutChat]->set_active(view_input);
		}

		Setting<ChatViewMax>::get().save(view_max);

		buttons[Buttons::BtMax]->set_active(!view_max);
		buttons[Buttons::BtMin]->set_active(view_max);

		update_view(pressed);
	}

	void UIChatBar::update_view(bool pressed)
	{
		if (view_input)
			input_text.set_state(Textfield::State::FOCUSED);
		else
			input_text.set_state(Textfield::State::DISABLED);

		if (pressed)
		{
			if (position_adjusted)
			{
				position_adjusted = false;

				if (position.y() >= min_view_y - input_bg_y)
					position.shift_y(input_bg_y);
			}
			else
			{
				if (view_input && position.y() >= min_view_y)
				{
					position_adjusted = true;
					position.shift_y(-input_bg_y);
				}
			}
		}

		int16_t y = position.y() - center_y - user_view_y;

		if (y < 0)
		{
			position.shift_y(-y);
			position.shift_y(15);
		}

		Point<int16_t> btMin_padding = Point<int16_t>(-4, 3);

		if (view_max)
		{
			dimension = Point<int16_t>(user_view_x, top_y + center_y + bottom_y) + Point<int16_t>(0, user_view_y);

			buttons[Buttons::BtMin]->set_position(Point<int16_t>(user_view_x - btMin_x, -top_y - center_y - user_view_y) + btMin_padding);

			Point<int16_t> input_btns_pos = Point<int16_t>(input_max_x - (input_bg_x - user_view_x) + input_origin_x - 17, 15 + input_origin_y + 1);

			buttons[Buttons::BtChat]->set_position(input_btns_pos + Point<int16_t>(input_btns_x * 0, 0));
			buttons[Buttons::BtItemLink]->set_position(input_btns_pos + Point<int16_t>(input_btns_x * 1, 0));
			buttons[Buttons::BtChatEmoticon]->set_position(input_btns_pos + Point<int16_t>(input_btns_x * 2, 0));
			buttons[Buttons::BtHelp]->set_position(input_btns_pos + Point<int16_t>(input_btns_x * 3, 0));
			buttons[Buttons::BtOutChat]->set_position(input_btns_pos + Point<int16_t>(input_btns_x * 4, 0));
		}
		else
		{
			dimension = Point<int16_t>(min_x, top_y + center_y + bottom_y);

			buttons[Buttons::BtMin]->set_position(Point<int16_t>(min_x - btMin_x, -top_y - center_y - user_view_y) + btMin_padding);
		}

#if LOG_LEVEL >= LOG_UI
		dimension_box = ColorBox(dimension.x(), dimension.y(), Color::Name::RED, 0.5f);
		top_box = ColorBox(dimension.x(), 3, Color::Name::GREEN, 0.5f);
		bottom_box = ColorBox(dimension.x(), 3, Color::Name::GREEN, 0.5f);
		left_box = ColorBox(3, dimension.y(), Color::Name::YELLOW, 0.5f);
		right_box = ColorBox(3, dimension.y(), Color::Name::YELLOW, 0.5f);
		input_box = ColorBox(user_view_x, input_bg_y, Color::Name::RED, 0.5f);
#endif
	}

	void UIChatBar::input_text_enter_callback(std::string message)
	{
		if (message == "")
		{
			input_text_escape_callback();
		}
		else
		{
			user_message_history.push_back(message);
			user_message_history_index = user_message_history.size();

			GeneralChatPacket(message, true).dispatch();

			input_text.change_text("");
		}
	}

	void UIChatBar::input_text_escape_callback()
	{
		toggle_input(false);
	}

	void UIChatBar::change_message(bool up)
	{
		size_t size = user_message_history.size();
		size_t message_history_min = 1;
		size_t message_history_max = size;
		size_t max = 7;

		if (size > max)
			message_history_min = size - max;

		if (user_message_history.size() > 0)
		{
			if (up && user_message_history_index > message_history_min)
				user_message_history_index--;

			if (!up && user_message_history_index < message_history_max)
				user_message_history_index++;

			input_text.change_text(user_message_history[user_message_history_index - 1]);
		}
	}
}