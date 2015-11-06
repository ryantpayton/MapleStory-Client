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
#include "AnimationData.h"
#include "Ptrmap.h"

namespace Data
{
	using::std::uint8_t;
	using::std::int32_t;
	using::std::string;
	using::std::vector;
	using::std::map;
	using::Graphics::AnimationData;
	using::Graphics::Texture;
	using::Graphics::FlipArgument;
	using::Util::vector2d;
	using::Util::Ptrmap;

	// Data associated with an npc id which is loaded from the game files.
	// Contains textures, lines to speak and some flags.
	class Npcdata
	{
	public:
		// Parameters: int32_t(npcdata id)
		Npcdata(int32_t);
		// Empty destructor.
		~Npcdata();
		// Returns wether to display this npc's name.
		bool showname() const;
		// Returns if this npc needs a script to be talked to.
		bool isscripted() const;
		// Returns if this npc can only be talked to using the mouse.
		bool ismouseonly() const;
		// Returns the number of stances/states this npc has.
		size_t getnumstates() const;
		// Returns name.
		string getname() const;
		// Returns function, aka 'merchant' or 'job instructor'.
		string getfunc() const;
		// Returns a state by position. Will return empty string for incorrect pos.
		// Placeholder for a better way to decide which animation to display.
		// Parameter: size_t(pos in the vector of states)
		string getstate(size_t) const;
		// Returns width and height of the texture.
		// Parameter: string(stance or npc state), uint8_t(animation frame)
		vector2d<int32_t> getdimensions(string, uint8_t) const;
		// Returns a pointer to a texture.
		// Parameters: string(stance or npc state), uint8_t(animation frame) 
		const Texture* gettexture(string, uint8_t) const;
		// Returns a reference to an animation. Returns default data if the paramter is invalid.
		// Parameters: string(stance or npc state)
		const AnimationData& getanimation(string) const;
	private:
		Ptrmap<string, AnimationData> animations;
		AnimationData defaultanim;
		map<string, vector<string>> lines;
		vector<string> states;
		string name;
		string func;
		bool hidename;
		bool scripted;
		bool mouseonly;
	};
}