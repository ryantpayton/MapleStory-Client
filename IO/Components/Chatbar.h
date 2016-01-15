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
#include "IO\UIElement.h"
#include "Textfield.h"
#include "Button.h"
#include "IO\Cursor.h"
#include "Graphics\Texture.h"
#include "Graphics\Text.h"
#include <vector>
#include <memory>

namespace IO
{
	using std::vector;
	using std::unique_ptr;
	using Graphics::Text;
	using Graphics::Texture;

	class Chatbar : public UIElement
	{
	public:
		enum Buttons : uint16_t
		{
			BT_OPENCHAT,
			BT_CLOSECHAT,
			BT_SCROLLUP,
			BT_SCROLLDOWN,
			BT_CHATTARGETS
		};

		enum ChatTarget
		{
			CHT_ALL,
			CHT_BUDDY,
			CHT_GUILD,
			CHT_ALLIANCE,
			CHT_PARTY,
			CHT_SQUAD
		};

		Chatbar(vector2d<int16_t> position);
		~Chatbar();

		void draw(float inter) const override;
		void update() override;
		void buttonpressed(uint16_t buttonid) override;
		rectangle2d<int16_t> bounds() const override;
		Cursor::Mousestate sendmouse(bool pressed, vector2d<int16_t> position) override;

		void sendline(string line, int8_t type);

	private:
		Textfield chatfield;
		map<bool, Texture> chatspace;
		map<ChatTarget, Texture> chattargets;
		Texture chatenter;
		Texture chatcover;
		Text closedtext;

		bool chatopen;
		ChatTarget chattarget;
		vector<string> lines;
		vector<string> lastentered;
		size_t lastpos;
	};
}