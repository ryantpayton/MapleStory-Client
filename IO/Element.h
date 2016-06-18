/////////////////////////////////////////////////////////////////////////////
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
#include "UIElement.h"

#include <tuple>
#include <type_traits>

namespace jrc
{
	class IElement
	{
	public:
		virtual ~IElement() {}

		// Return wether the element can only be created once.
		// Such elements will have their visibility toggled when adding them again.
		// Default: false
		virtual bool unique() const = 0;

		// Return wether the element is focused.
		// These elements always stay on top of the screen.
		// Default: false
		virtual bool focused() const = 0;

		// Return the type of this element.
		virtual UIElement::Type type() const = 0;

		// Create the Element instance.
		virtual UIElement::UPtr instantiate() const = 0;
	};


	template <typename T, typename...Args>
	class Element : public IElement
	{
	public:
		Element(Args...a)
		{
			args = std::make_tuple(a...);
		}

		constexpr bool unique() const override
		{
			return T::TOGGLED;
		}

		constexpr bool focused() const override
		{
			return T::FOCUSED;
		}

		constexpr UIElement::Type type() const override
		{
			return T::TYPE;
		}

		UIElement::UPtr instantiate() const override
		{
			return construct(std::index_sequence_for<Args...>());
		}

	private:
		template <size_t...I>
		UIElement::UPtr construct(std::index_sequence<I...>) const
		{ 
			return std::make_unique<T>(std::get<I>(args)...); 
		}

		std::tuple<Args...> args;
	};


	template <typename T>
	class Element<T, typename std::enable_if<std::is_default_constructible<T>::value>::type> : public IElement
	{
	public:
		constexpr bool unique() const override
		{
			return T::TOGGLED;
		}

		constexpr bool focused() const override
		{
			return T::FOCUSED;
		}

		constexpr UIElement::Type type() const override
		{
			return T::TYPE;
		}

		UIElement::UPtr instantiate() const override
		{
			return std::make_unique<T>();
		}
	};
}