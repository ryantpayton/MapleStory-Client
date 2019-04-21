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
#include "../UIElement.h"
#include "../Components/Textfield.h"

#include "../../Graphics/Texture.h"

#include <functional>

namespace jrc
{
	class UINotice : public UIElement
	{
	public:
		static constexpr Type TYPE = NOTICE;
		static constexpr bool FOCUSED = true;
		static constexpr bool TOGGLED = false;

	protected:
		UINotice(std::string question);

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
	};


	class UIYesNo : public UINotice
	{
	public:
		UIYesNo(std::string question, std::function<void(bool yes)> yesnohandler);

		void draw(float alpha) const override;

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
		UIEnterNumber(std::string question, std::function<void(int32_t number)> numhandler, int32_t min, int32_t max, int32_t def);

		void draw(float alpha) const override;
		void update() override;

		Cursor::State send_cursor(bool pressed, Point<int16_t> cursorpos) override;

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
		int32_t min;
		int32_t max;
	};
}