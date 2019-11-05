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
#pragma once

#include "DrawArgument.h"

#include <map>
#include <vector>

namespace ms
{
	class Text
	{
	public:
		enum Font
		{
			A11M,
			A11B,
			A12M,
			A12B,
			A13M,
			A13B,
			A15B,
			A18M,
			NUM_FONTS
		};

		enum Alignment
		{
			LEFT,
			CENTER,
			RIGHT
		};

		enum Background
		{
			NONE,
			NAMETAG
		};

		class Layout
		{
		public:
			struct Word
			{
				std::size_t first;
				std::size_t last;
				Font font;
				Color::Name color;
			};

			struct Line
			{
				std::vector<Word> words;
				Point<int16_t> position;
			};

			Layout(const std::vector<Line>& lines, const std::vector<int16_t>& advances, std::int16_t width, std::int16_t height, std::int16_t endx, std::int16_t endy);
			Layout();

			std::int16_t width() const;
			std::int16_t height() const;
			std::int16_t advance(std::size_t index) const;
			Point<int16_t> get_dimensions() const;
			Point<int16_t> get_endoffset() const;

			using iterator = std::vector<Line>::const_iterator;
			iterator begin() const;
			iterator end() const;

		private:
			std::vector<Line> lines;
			std::vector<int16_t> advances;
			Point<int16_t> dimensions;
			Point<int16_t> endoffset;
		};

		Text(Font font, Alignment alignment, Color::Name color, Background background, const std::string& text = "", std::uint16_t maxwidth = 0, bool formatted = true, std::int16_t line_adj = 0);
		Text(Font font, Alignment alignment, Color::Name color, const std::string& text = "", std::uint16_t maxwidth = 0, bool formatted = true, std::int16_t line_adj = 0);
		Text();

		void draw(const DrawArgument& args) const;

		void change_text(const std::string& text);
		void change_color(Color::Name color);
		void set_background(Background background);

		bool empty() const;
		std::size_t length() const;
		std::int16_t width() const;
		std::int16_t height() const;
		std::uint16_t advance(std::size_t pos) const;
		Point<int16_t> dimensions() const;
		Point<int16_t> endoffset() const;
		const std::string& get_text() const;

	private:
		void reset_layout();

		Font font;
		Alignment alignment;
		Color::Name color;
		Background background;
		Layout layout;
		std::uint16_t maxwidth;
		bool formatted;
		std::string text;
		std::int16_t line_adj;
	};
}