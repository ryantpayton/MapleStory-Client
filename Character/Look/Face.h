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
#include "BodyDrawinfo.h"
#include "Graphics\Texture.h"
#include "Util\Enum.h"
#include "Util\Console.h"

namespace Character
{
	class Expression
	{
	public:
		static const size_t LENGTH = 24;
		enum Value
		{
			DEFAULT, BLINK, HIT, SMILE, TROUBLED, CRY, ANGRY,
			BEWILDERED, STUNNED, BLAZE, BOWING, CHEERS, CHU, DAM,
			DESPAIR, GLITTER, HOT, HUM, LOVE, OOPS, PAIN, SHINE,
			VOMIT, WINK
		};

		static string nameof(Value value)
		{
			static const string expnames[LENGTH] =
			{
				"default", "blink", "hit", "smile", "troubled", "cry", "angry",
				"bewildered", "stunned", "blaze", "bowing", "cheers", "chu", "dam",
				"despair", "glitter", "hot", "hum", "love", "oops", "pain", "shine",
				"vomit", "wink"
			};

			return expnames[value];
		}

		static EnumIterator<Value> getit(Value s = DEFAULT, Value l = WINK)
		{
			return EnumIterator<Value>(s, l);
		}

		static Value byid(size_t id)
		{
			if (id >= LENGTH)
				throw std::runtime_error("unhandled expression.");

			return static_cast<Value>(id);
		}

		static Value bystring(string name)
		{
			for (auto it = getit(); it.hasnext(); it.increment())
			{
				Value value = it.get();
				if (nameof(value) == name)
					return value;
			}

			Console::get().print("Unhandled expression: " + name);

			return DEFAULT;
		}
	};

	using Graphics::Texture;
	using Graphics::DrawArgument;

	class Face
	{
	public:
		Face(int32_t faceid);
		Face();

		void draw(Expression::Value expression, uint8_t frame, const DrawArgument& args) const;
		uint8_t nextframe(Expression::Value expression, uint8_t frame) const;
		int16_t getdelay(Expression::Value expression, uint8_t frame) const;
		string getname() const;

	private:
		unordered_map<uint8_t, Texture> expressions[Expression::LENGTH];
		unordered_map<uint8_t, int16_t> delays[Expression::LENGTH];
		string name;
	};
}

