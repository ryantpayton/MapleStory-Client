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
#include "SkillSound.h"

#include "nlnx\nx.hpp"

namespace jrc
{
	SingleSkillSound::SingleSkillSound(std::string strid)
	{
		nl::node soundsrc = nl::nx::sound["Skill.img"][strid];

		usesound = soundsrc["Use"];
		hitsound = soundsrc["Hit"];
	}

	void SingleSkillSound::play_use() const
	{
		usesound.play();
	}

	void SingleSkillSound::play_hit() const
	{
		hitsound.play();
	}
}