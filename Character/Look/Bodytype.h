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
#include "Character\Look\BodyDrawinfo.h"
#include "Graphics\Texture.h"

namespace Character
{
	using::Graphics::Texture;
	using::Graphics::DrawArgument;

	class Bodytype
	{
	public:
		Bodytype(int8_t, const BodyDrawinfo&);
		Bodytype();
		~Bodytype();
		void draw(string, CharacterLayer, uint8_t, const DrawArgument&) const;
		const string& getname() const;
	private:
		map<string, map<CharacterLayer, map<uint8_t, Texture>>> stances;
		string name;
	};
}

