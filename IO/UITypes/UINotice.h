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
#pragma once
#include "../UIDragElement.h"

#include "../Components/Textfield.h"

namespace jrc
{
	class UINotice : public UIDragElement<PosNOTICE>
	{
	public:
		static constexpr Type TYPE = UIElement::Type::NOTICE;
		static constexpr bool FOCUSED = true;
		static constexpr bool TOGGLED = false;

		enum NoticeType
		{
			YESNO,
			ENTERNUMBER,
			OK,
			OKSMALL
		};

		void send_key(int32_t keycode, bool pressed, bool escape);

	protected:
		UINotice(std::string message, NoticeType type);

		void draw(bool textfield) const;

		int16_t box2offset() const;

	private:
		Texture top;
		Texture center;
		Texture centerbox;
		Texture box;
		Texture box2;
		Texture bottom;
		Texture bottombox;
		Text question;
		int16_t height;
		NoticeType type;
	};

	class UIYesNo : public UINotice
	{
	public:
		UIYesNo(std::string message, std::function<void(bool yes)> yesnohandler);

		void draw(float alpha) const override;

		void send_key(int32_t keycode, bool pressed, bool escape) override;

	protected:
		Button::State button_pressed(uint16_t buttonid) override;

	private:
		enum Buttons : int16_t
		{
			YES, NO
		};

		std::function<void(bool yes)> yesnohandler;
	};

	class UIEnterNumber : public UINotice
	{
	public:
		UIEnterNumber(std::string message, std::function<void(int32_t number)> numhandler, int32_t max, int32_t quantity);

		void draw(float alpha) const override;
		void update() override;

		Cursor::State send_cursor(bool pressed, Point<int16_t> cursorpos) override;
		void send_key(int32_t keycode, bool pressed, bool escape) override;

	protected:
		Button::State button_pressed(uint16_t buttonid) override;

	private:
		void handlestring(std::string numstr);

		enum Buttons : int16_t
		{
			OK, CANCEL
		};

		std::function<void(int32_t number)> numhandler;
		Textfield numfield;
		int32_t max;
	};

	class UIOk : public UINotice
	{
	public:
		UIOk(std::string message, std::function<void()> okhandler, NoticeType type);

		void draw(float alpha) const override;

		void send_key(int32_t keycode, bool pressed, bool escape) override;

	protected:
		Button::State button_pressed(uint16_t buttonid) override;

	private:
		enum Buttons : int16_t
		{
			OK
		};

		std::function<void()> okhandler;
	};
}