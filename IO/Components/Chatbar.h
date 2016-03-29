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
#include "Button.h"
#include "Textfield.h"
#include "Slider.h"
#include "IO\UIElement.h"
#include "IO\Cursor.h"
#include "Graphics\Texture.h"
#include "Graphics\Geometry.h"
#include <vector>

namespace IO
{
	using std::unordered_map;
	using std::vector;
	using Graphics::Geometry;
	using Graphics::Rectangle;
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

		static const size_t NUM_TARGETS = 6;
		enum ChatTarget
		{
			CHT_ALL,
			CHT_BUDDY,
			CHT_GUILD,
			CHT_ALLIANCE,
			CHT_PARTY,
			CHT_SQUAD
		};

		enum LineType
		{
			UNK0,
			WHITE,
			RED,
			BLUE,
			YELLOW
		};

		Chatbar(Point<int16_t> position);
		~Chatbar();

		void draw(float inter) const override;
		void update() override;
		void buttonpressed(uint16_t buttonid) override;
		bool isinrange(Point<int16_t> cursorpos) const override;
		Cursor::State sendmouse(bool pressed, Point<int16_t> cursorpos) override;

		void sendline(string line, LineType type);

	private:
		int16_t getchattop() const;

		Textfield chatfield;
		Texture chatspace[2];
		Texture chattargets[NUM_TARGETS];
		Texture chatenter;
		Texture chatcover;
		Texture tapbar;
		Texture tapbartop;

		bool chatopen;
		ChatTarget chattarget;

		vector<string> lastentered;
		size_t lastpos;

		unordered_map<int16_t, Text> rowtexts;

		static const int16_t CHATYOFFSET = 65;
		static const int16_t CHATROWHEIGHT = 16;
		Rectangle chatbox;
		int16_t chatrows;
		int16_t rowpos;
		int16_t rowmax;
		unique_ptr<Slider> slider;
		bool dragchattop;
	};
}