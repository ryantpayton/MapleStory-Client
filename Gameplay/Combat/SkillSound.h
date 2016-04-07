//////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2016 Daniel Allendorf                                        //
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
#include "Audio\Audio.h"

namespace Gameplay
{
	// Interface for skill sound.
	class SkillSound
	{
	public:
		virtual ~SkillSound() {}

		virtual void playuse() const = 0;
		virtual void playhit() const = 0;
	};


	// No sound.
	class NoSkillSound : public SkillSound
	{
	public:
		void playuse() const override {}
		void playhit() const override {}
	};


	// Plays one use and one hit sound.
	class SingleSkillSound : public SkillSound
	{
	public:
		using string = std::string;

		SingleSkillSound(string strid);

		void playuse() const override;
		void playhit() const override;

	private:
		using Sound = Audio::Sound;

		Sound usesound;
		Sound hitsound;
	};
}