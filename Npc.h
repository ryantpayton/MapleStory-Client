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
#include "PhysicsObject.h"
#include "Npcdata.h"
#include "AnimationController.h"
#include "Randomizer.h"
#include "Textlabel.h"

namespace Gameplay
{
	using::std::string;
	using::Util::Randomizer;
	using::Data::Npcdata;
	using::Graphics::AnimationController;
	using::IO::Textlabel;

	// Represents a npc on the current map and only contains values specific
	// to this npc. Data common among npcs is accessed via the reference to 'Npcdata'.
	// Implements the 'Mapobject' interface to be used in a 'Mapobjects' template.
	class Npc : public Mapobject
	{
	public:
		// Parameters: Npcdata&(reference to shared data), int(npcdata id),
		// int(objectid), bool(is mirrored), short(foothold id), vector2d(position)
		Npc(const Npcdata&, int32_t, int32_t, bool, uint16_t, vector2d<int32_t>);
		// Draws the current animation and name/function tags.
		// Parameters: vector2d(position of camera)
		void draw(vector2d<int32_t>) const;
		// Updates the current animation and physics.
		// Parameters: Physics&(engine to use), short(delay-per-frame)
		int8_t update(const Physics&, uint16_t);
		// Changes stance and resets animation.
		// Parameters: string(stance eg. npc state)
		void setstance(string);
		// Parameters: bool(is object active)
		void setactive(bool);
		// Set the PhysicsObject's position as specified.
		void setposition(vector2d<int32_t>);
		// Returns object id of this npc.
		int32_t getoid() const;
		// Returns layer of this npc's foothold.
		int8_t getlayer() const;
		// Returns the current position as a point from the PhysicsObject's position.
		vector2d<int32_t> getposition() const;
		// Returns width and height of the current texture.
		vector2d<int32_t> getdimensions() const;
	private:
		const Npcdata& ndata;
		AnimationController animcont;
		PhysicsObject phobj;
		int32_t oid;
		int32_t npcid;
		bool flip;
		string stance;
		bool active;
		Randomizer random;
		Textlabel name;
		Textlabel func;
	};
}

