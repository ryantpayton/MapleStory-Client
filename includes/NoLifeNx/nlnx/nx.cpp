//////////////////////////////////////////////////////////////////////////////
// NoLifeNx - Part of the NoLifeStory project                               //
// Copyright © 2013 Peter Atashian                                          //
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
#include "nx.hpp"
#include "file.hpp"
#include "node.hpp"

#include <fstream>
#include <vector>
#include <memory>
#include <stdexcept>

namespace nl
{
	namespace nx
	{
		std::vector<std::unique_ptr<file>> files;

		bool exists(std::string name)
		{
			return std::ifstream(name).is_open();
		}

		node add_file(std::string name)
		{
			if (!exists(name))
				return {};

			files.emplace_back(new file(name));

			return *files.back();
		}

		node Base, Character, Effect, Etc, EtcLatest, Item, Map, Map001, MapLatest, Mob, Morph, Npc, Quest, Reactor, Skill, Sound, Sound002, String, TamingMob, UI;

		void load_all()
		{
			if (exists("Base.nx"))
			{
				Base = add_file("Base.nx");
				Character = add_file("Character.nx");
				Effect = add_file("Effect.nx");
				Etc = add_file("Etc.nx");
				EtcLatest = add_file("EtcLatest.nx");
				Item = add_file("Item.nx");
				Map = add_file("Map.nx");
				Map001 = add_file("Map001.nx");
				MapLatest = add_file("MapLatest.nx");
				Mob = add_file("Mob.nx");
				Morph = add_file("Morph.nx");
				Npc = add_file("Npc.nx");
				Quest = add_file("Quest.nx");
				Reactor = add_file("Reactor.nx");
				Skill = add_file("Skill.nx");
				Sound = add_file("Sound.nx");
				Sound002 = add_file("Sound002.nx");
				String = add_file("String.nx");
				TamingMob = add_file("TamingMob.nx");
				UI = add_file("UI.nx");
			}
			else if (exists("Data.nx"))
			{
				Base = add_file("Data.nx");
				Character = Base["Character"];
				Effect = Base["Effect"];
				Etc = Base["Etc"];
				EtcLatest = Base["EtcLatest"];
				Item = Base["Item"];
				Map = Base["Map"];
				Map001 = Base["Map001"];
				MapLatest = Base["MapLatest"];
				Mob = Base["Mob"];
				Morph = Base["Morph"];
				Npc = Base["Npc"];
				Quest = Base["Quest"];
				Reactor = Base["Reactor"];
				Skill = Base["Skill"];
				Sound = Base["Sound"];
				Sound002 = Base["Sound002"];
				String = Base["String"];
				TamingMob = Base["TamingMob"];
				UI = Base["UI"];
			}
			else
			{
				throw std::runtime_error("Failed to locate nx files.");
			}
		}
	}
}