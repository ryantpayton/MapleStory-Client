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
#include "BodyDrawinfo.h"

#include "..\..\Console.h"
#include "..\..\Graphics\Texture.h"
#include "..\..\Util\Enum.h"
#include "..\..\Util\EnumMap.h"

namespace jrc
{
	class Expression
	{
	public:
		enum Value
		{
			DEFAULT, BLINK, HIT, SMILE, TROUBLED, CRY, ANGRY,
			BEWILDERED, STUNNED, BLAZE, BOWING, CHEERS, CHU, DAM,
			DESPAIR, GLITTER, HOT, HUM, LOVE, OOPS, PAIN, SHINE,
			VOMIT, WINK,
			LENGTH
		};

		static constexpr char* names[LENGTH] = 
		{
			"default", "blink", "hit", "smile", "troubled", "cry", "angry",
			"bewildered", "stunned", "blaze", "bowing", "cheers", "chu", "dam",
			"despair", "glitter", "hot", "hum", "love", "oops", "pain", "shine",
			"vomit", "wink"
		};

		static EnumIterator<Value> getit(Value s = DEFAULT, Value l = WINK)
		{
			return EnumIterator<Value>(s, l);
		}

		static Value byaction(size_t action)
		{
			action -= 98;
			if (action < LENGTH)
				return static_cast<Value>(action);

			Console::get().print("Unhandled expression id: " + action);
			return DEFAULT;
		}

		static Value byid(size_t id)
		{
			if (id < LENGTH)
				return static_cast<Value>(id);

			Console::get().print("Unhandled expression id: " + id);
			return DEFAULT;
		}

		static Value bystring(std::string name)
		{
			for (auto it = getit(); it.hasnext(); it.increment())
			{
				Value value = it.get();
				if (names[value] == name)
					return value;
			}

			Console::get().print("Unhandled expression: " + name);

			return DEFAULT;
		}
	};


	class Face
	{
	public:
		Face(int32_t faceid);
		Face();

		void draw(Expression::Value expression, uint8_t frame, const DrawArgument& args) const;
		uint8_t nextframe(Expression::Value expression, uint8_t frame) const;
		int16_t get_delay(Expression::Value expression, uint8_t frame) const;
		std::string get_name() const;

	private:
		struct Frame
		{
			Texture texture;
			uint16_t delay;

			Frame(nl::node src)
			{
				texture = src["face"];

				Point<int16_t> shift = src["face"]["map"]["brow"];
				texture.shift(-shift);

				delay = src["delay"];
				if (delay == 0)
					delay = 2500;
			}
		};

		EnumMap<Expression::Value, std::unordered_map<uint8_t, Frame>> expressions;
		std::string name;
	};
}

