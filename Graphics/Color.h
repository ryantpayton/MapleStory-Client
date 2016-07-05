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
#include <array>
#include <cstdint>

namespace jrc
{
	// Simple color class which stores r-g-b-a components.
	class Color
	{
	public:
		static constexpr size_t LENGTH = 4;
		using underlying_t = std::array<float, LENGTH>;

		// Codes of predefined colors.
		enum Code : uint32_t
		{
			NONE = 0x00000000,
			WHITE = 0xFFFFFFFF,
			BLACK = 0x000000FF,
			RED = 0xFF0000FF,
			GREEN = 0x00FF00FF,
			BLUE = 0x0000FFFF,
			YELLOW = 0xFFFF00FF,
			TURQUOISE = 0x00FFFFFF,
			PURPLE = 0xFF00FFFF
		};

		// Create a color by an array of real numbers [0.0f, 1.0f]
		constexpr Color(underlying_t comps)
			: rgba(comps) {}
		// Create a color by real numbers [0.0f, 1.0f]
		constexpr Color(float red, float green, float blue, float alpha)
			: Color(underlying_t{ red, green, blue, alpha }) {}
		// Create a color by an array of natural numbers [0, 255]
		constexpr Color(const std::array<uint8_t, Color::LENGTH> comps)
			: Color(comps[0], comps[1], comps[2], comps[3]) {}
		// Create a color by natural numbers [0, 255]
		constexpr Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
			: Color(
				static_cast<float>(red) / 255,
				static_cast<float>(green) / 255,
				static_cast<float>(blue) / 255,
				static_cast<float>(alpha) / 255
			) {}
		// Create a color by code.
		constexpr Color(uint32_t code)
			: Color(
				static_cast<uint8_t>(code >> 24),
				static_cast<uint8_t>(code >> 16),
				static_cast<uint8_t>(code >> 8),
				static_cast<uint8_t>(code)
			) {}
		// Create a color by named code.
		constexpr Color(Code code)
			: Color((uint32_t)code) {}
		constexpr Color()
			: Color(NONE) {}

		// Check wether the color is completely invisble.
		constexpr bool invisible() const
		{
			return rgba[3] <= 0.0f;
		}

		// Return the red component.
		constexpr float r() const
		{
			return rgba[0];
		}

		// Return the green component.
		constexpr float g() const
		{
			return rgba[1];
		}

		// Return the blue component.
		constexpr float b() const
		{
			return rgba[2];
		}

		// Return the alpha (opacity) component.
		constexpr float a() const
		{
			return rgba[3];
		}

		// Return all components.
		const float* data() const;

		// Return a begin iterator.
		underlying_t::const_iterator begin() const;

		// Return an end iterator.
		underlying_t::const_iterator end() const;

		// Blend the second color into the first.
		Color blend(const Color& other, float alpha) const;

		// Combine two colors.
		constexpr Color operator*(const Color& o) const
		{
			return{
				r() * o.r(),
				g() * o.g(),
				b() * o.b(),
				a() * o.a()
			};
		}

		// Combine two colors.
		constexpr Color operator/(const Color& o) const
		{
			return{
				r() / o.r(),
				g() / o.g(),
				b() / o.b(),
				a() / o.a()
			};
		}

	private:
		underlying_t rgba;
	};
}