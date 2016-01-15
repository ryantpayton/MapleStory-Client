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
#include "Graphics\Text.h"
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
	using Graphics::Text;

	// Represents a npc on the current map.
	// Implements the 'Mapobject' interface to be used in a 'Mapobjects' template.
	class Npc : public MapObject
	{
	public:
		// Constructs an npc by combining data from game files with
		// data sent by the server.
		Npc(int32_t npcid, int32_t oid, bool mirrored, uint16_t fhid, 
			bool control, vector2d<int16_t> position);

		// Draws the current animation and name/function tags.
		void draw(const Camera& camera, float inter) const override;
		// Updates the current animation and physics.
		int8_t update(const Physics& physics) override;

		// Changes stance and resets animation.
		void setstance(string stance);
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

		int32_t npcid;
		bool flip;
		string stance;
		bool control;

		Randomizer random;
		Text namelabel;
		Text funclabel;
	};
}

