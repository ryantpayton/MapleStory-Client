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
#include "Mapobject.h"
#include "Graphics\Animation.h"
#include "Graphics\Textlabel.h"
#include "Gameplay\Physics\PhysicsObject.h"
#include "Util\Randomizer.h"
#include <map>

namespace Gameplay
{
	using::std::string;
	using::std::map;
	using::Util::Randomizer;
	using::Graphics::Animation;
	using::Graphics::Textlabel;

	// Represents a npc on the current map.
	// Implements the 'Mapobject' interface to be used in a 'Mapobjects' template.
	class Npc : public Mapobject
	{
	public:
		// Parameters: int(npcdata id), int(objectid), bool(is mirrored), short(foothold id), int32_t(x), int32_t(y)
		Npc(int32_t, int32_t, bool, uint16_t, int32_t, int32_t);
		// Draws the current animation and name/function tags.
		void draw(vector2d<int32_t>, float) const;
		// Updates the current animation and physics.
		// Parameters: Physics&(engine to use)
		int8_t update(const Physics&);
		// Changes stance and resets animation.
		// Parameters: string(stance eg. npc state)
		void setstance(string);
		// Parameters: bool(is object active)
		void setactive(bool);
		// Set the PhysicsObject's position as specified.
		void setposition(int32_t, int32_t);
		// Returns object id of this npc.
		int32_t getoid() const;
		// Returns layer of this npc's foothold.
		int8_t getlayer() const;
		// Returns the current position as a point from the PhysicsObject's position.
		vector2d<int32_t> getposition() const;
		// Returns width and height of the current texture.
		vector2d<int32_t> getdimensions() const;
	private:
		map<string, Animation> animations;
		map<string, vector<string>> lines;
		vector<string> states;
		string name;
		string func;
		bool hidename;
		bool scripted;
		bool mouseonly;
		PhysicsObject phobj;
		int32_t oid;
		int32_t npcid;
		bool flip;
		string stance;
		bool active;
		Randomizer random;
		Textlabel namelabel;
		Textlabel funclabel;
	};
}

