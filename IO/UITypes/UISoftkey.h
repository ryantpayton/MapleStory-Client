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
#include "Graphics\Text.h"
#include "Util\Randomizer.h"

namespace IO
{
	using Graphics::Text;

	// Keyboard which is used via the mouse. The game uses this for pic/pin input.
	class UISoftkey : public UIElement
	{
	public:
		enum Buttons
		{
			BT_0,
			BT_1,
			BT_2,
			BT_3,
			BT_4,
			BT_5,
			BT_6,
			BT_7,
			BT_8,
			BT_9,
			BT_NEXT,
			BT_BACK,
			BT_CANCEL,
			BT_OK
		};

		enum SkType
		{
			REGISTER,
			CHARSELECT,
			CHARDEL,
			MERCHANT
		};

		UISoftkey(SkType);

		void draw(float) const override;
		void buttonpressed(uint16_t) override;

	private:
		void shufflekeys();
		Point<int16_t> keypos(uint8_t) const;

		SkType type;

		Text entry;
		Randomizer random;
	};

	class ElementSoftkey : public Element
	{
	public:
		ElementSoftkey(UISoftkey::SkType t) 
		{
			sktype = t;
		}

		bool isfocused() const override
		{
			return true;
		}

		UIElement::Type type() const override
		{
			return UIElement::SOFTKEYBOARD;
		}

		UISoftkey* instantiate() const override
		{
			return new UISoftkey(sktype);
		}
	private:
		UISoftkey::SkType sktype;
	};
}

