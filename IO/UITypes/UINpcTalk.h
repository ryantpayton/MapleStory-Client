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
#include "IO\Components\Npctext.h"
#include "Graphics\Text.h"
#include "Graphics\Texture.h"

namespace IO
{
	using Graphics::Text;
	using Graphics::Texture;

	class UINpcTalk : public UIElement
	{
	public:
		enum Buttons
		{
			OK,
			NEXT,
			PREV,
			END,
			YES,
			NO
		};

		UINpcTalk();

		void draw(float inter) const override;
		void buttonpressed(uint16_t buttonid) override;
		void settext(int32_t npcid, int8_t msgtype, int16_t style, int8_t speaker, string text);

	private:
		Texture top;
		Texture fill;
		Texture bottom;
		Texture nametag;

		Npctext npctext;
		Texture speaker;
		Text name;
		int16_t height;
		int16_t vtile;
		bool slider;

		int8_t type;
	};

	class ElementNpcTalk : public Element
	{
		bool isunique() const override
		{
			return true;
		}

		UIElement::Type type() const override
		{
			return UIElement::NPCTALK;
		}

		UIElement* instantiate() const override
		{
			return new UINpcTalk();
		}
	};
}