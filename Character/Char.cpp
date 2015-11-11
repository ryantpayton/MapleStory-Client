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
#include "Char.h"

namespace Character
{
	// Number of stances that are currently implemented.
	const size_t NUM_STANCES = 11;

	// Names of the character stances used in the game's files.
	const string stances[NUM_STANCES] =
	{
		"", "walk", "stand", "jump", "alert", "prone", "fly", "ladder", "rope",
		"dead", "sit"
	};

	void Char::draw(vector2d<int32_t> absp, float inter) const
	{
		look.draw(absp, inter);
		namelabel.draw(absp);
	}

	void Char::setflip(bool f)
	{
		flip = f;
		look.setflip(f);
	}

	void Char::setstance(Stance st)
	{
		if (stance != st)
		{
			stance = st;
			look.setstance(stances[st / 2]);
		}
	}

	void Char::setposition(int32_t x, int32_t y)
	{
		phobj.fx = static_cast<float>(x);
		phobj.fy = static_cast<float>(y);
	}

	bool Char::getflip() const
	{
		return flip;
	}

	int8_t Char::getlayer() const
	{
		return phobj.fhlayer;
	}

	int32_t Char::getoid() const
	{
		return cid;
	}

	vector2d<int32_t> Char::getposition() const
	{
		return vector2d<int32_t>(static_cast<int32_t>(phobj.fx), static_cast<int32_t>(phobj.fy));
	}

	Char::Stance Char::getstance() const
	{
		return stance;
	}

	CharLook& Char::getlook()
	{
		return look;
	}

	PhysicsObject& Char::getphobj()
	{
		return phobj;
	}
}