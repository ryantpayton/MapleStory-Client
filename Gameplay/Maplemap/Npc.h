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
#include "MapObject.h"
#include "Graphics\Animation.h"
#include "Graphics\Textlabel.h"
#include "Gameplay\Physics\PhysicsObject.h"
#include "Util\Randomizer.h"
#include <map>

namespace Gameplay
{
	using std::string;
	using std::map;
	using std::vector;
	using Util::Randomizer;
	using Graphics::Animation;
	using Graphics::Textlabel;

	// Represents a npc on the current map.
	// Implements the 'Mapobject' interface to be used in a 'Mapobjects' template.
	class Npc : public MapObject
	{
	public:
		// Constructs an npc by combining data from game files with
		// data sent by the server.
		Npc(int32_t npcid, int32_t oid, bool mirrored, uint16_t fhid, 
			bool control, int16_t xpos, int16_t ypos);

		// Draws the current animation and name/function tags.
		void draw(vector2d<int16_t> viewpos, float inter) const override;
		// Updates the current animation and physics.
		int8_t update(const Physics& physics) override;
		// Set the PhysicsObject's position as specified.
		void setposition(int16_t xpos, int16_t ypos) override;
		// Returns object id of this npc.
		int32_t getoid() const override;
		// Returns layer of this npc's foothold.
		int8_t getlayer() const override;
		// Returns the current position as a point from the PhysicsObject's position.
		vector2d<int16_t> getposition() const override;

		// Changes stance and resets animation.
		void setstance(string stance);
		// Change object activity.
		void setactive(bool newvalue);
		// Returns width and height of the current animation.
		vector2d<int16_t> getdimensions() const;

	private:
		map<string, Animation> animations;
		map<string, vector<string>> lines;
		vector<string> states;
		string name;
		string func;
		bool hidename;
		bool scripted;
		bool mouseonly;

		bool control;
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

