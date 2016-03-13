//////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2016 Daniel Allendorf                                        //
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
#include "IO\Components\Textfield.h"
#include "Graphics\Texture.h"
#include <functional>

namespace IO
{
	using std::function;
	using Graphics::Texture;
	using Graphics::Text;

	class UINotice : public UIElement
	{
	protected:
		UINotice(string question);

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
		UIYesNo(string question, function<void(bool yes)> yesnohandler);

		void draw(float alpha) const override;

	protected:
		enum Buttons : int16_t
		{
			YES, NO
		};

		void buttonpressed(uint16_t buttonid) override;

	private:
		function<void(bool yes)> yesnohandler;
	};


	class UIEnterNumber : public UINotice
	{
	public:
		UIEnterNumber(string question, function<void(int32_t number)> numhandler, int32_t min, int32_t max, int32_t def);

		void draw(float alpha) const override;
		void update() override;
		Cursor::State sendmouse(bool pressed, Point<int16_t> cursorpos) override;

	protected:
		enum Buttons : int16_t
		{
			OK, CANCEL
		};

		void buttonpressed(uint16_t buttonid) override;

	private:
		void handlestring(string numstr);

		function<void(int32_t number)> numhandler;
		Textfield numfield;
		int32_t min;
		int32_t max;
	};


	class ElementNotice : public Element
	{
	public:
		bool isunique() const override
		{
			return false;
		}

		bool isfocused() const override
		{
			return true;
		}

		UIElement::Type type() const override
		{
			return UIElement::NOTICE;
		}
	};


	class ElementYesNo : public ElementNotice
	{
	public:
		ElementYesNo(string q, function<void(bool)> yh)
			: question(q), yesnohandler(yh) {}

		UIElement* instantiate() const override
		{
			return new UIYesNo(question, yesnohandler);
		}

	private:
		string question;
		function<void(bool yes)> yesnohandler;
	};


	class ElementEnterNumber : public ElementNotice
	{
	public:
		ElementEnterNumber(string q, function<void(int32_t)> nh, int32_t mi, int32_t ma, int32_t de)
			: question(q), numhandler(nh), min(mi), max(ma), def(de) {}

		UIElement* instantiate() const override
		{
			return new UIEnterNumber(question, numhandler, min, max, def);
		}

	private:
		string question;
		function<void(int32_t number)> numhandler;
		int32_t min;
		int32_t max;
		int32_t def;
	};
}