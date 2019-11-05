//////////////////////////////////////////////////////////////////////////////////
//	This file is part of the continued Journey MMORPG client					//
//	Copyright (C) 2015-2019  Daniel Allendorf, Ryan Payton						//
//																				//
//	This program is free software: you can redistribute it and/or modify		//
//	it under the terms of the GNU Affero General Public License as published by	//
//	the Free Software Foundation, either version 3 of the License, or			//
//	(at your option) any later version.											//
//																				//
//	This program is distributed in the hope that it will be useful,				//
//	but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				//
//	GNU Affero General Public License for more details.							//
//																				//
//	You should have received a copy of the GNU Affero General Public License	//
//	along with this program.  If not, see <https://www.gnu.org/licenses/>.		//
//////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Maplemap/MapObject.h"

#include "../Graphics/Animation.h"
#include "../Net/Login.h"

#include <cstdint>
#include <memory>

namespace ms
{
	class NpcSpawn
	{
	public:
		NpcSpawn(std::int32_t oid, std::int32_t npcid, Point<int16_t> position, bool mirrored, std::uint16_t fh);

		std::int32_t get_oid() const;
		std::unique_ptr<MapObject> instantiate(const Physics& physics) const;

	private:
		std::int32_t oid;
		std::int32_t id;
		Point<int16_t> position;
		bool flip;
		std::uint16_t fh;
	};

	class MobSpawn
	{
	public:
		MobSpawn(std::int32_t oid, std::int32_t id, std::int8_t mode, std::int8_t stance, std::uint16_t fh, bool newspawn, std::int8_t team, Point<int16_t> position);

		std::int8_t get_mode() const;
		std::int32_t get_oid() const;
		std::unique_ptr<MapObject> instantiate() const;

	private:
		std::int32_t oid;
		std::int32_t id;
		std::int8_t mode;
		std::int8_t stance;
		std::uint16_t fh;
		bool newspawn;
		std::int8_t team;
		Point<int16_t> position;
	};

	class ReactorSpawn
	{
	public:
		ReactorSpawn(std::int32_t oid, std::int32_t rid, std::int8_t state, Point<int16_t> position);

		std::int32_t get_oid() const;
		std::unique_ptr<MapObject> instantiate(const Physics& physics) const;

	private:
		std::int32_t oid;
		std::int32_t rid;
		std::int8_t state;
		Point<int16_t> position;
	};

	class DropSpawn
	{
	public:
		DropSpawn(std::int32_t oid, std::int32_t id, bool meso, std::int32_t owner, Point<int16_t> position, Point<int16_t> destination, std::int8_t droptype, std::int8_t mode, bool playerdrop);

		bool is_meso() const;
		std::int32_t get_itemid() const;
		std::int32_t get_oid() const;
		std::unique_ptr<MapObject> instantiate(const Animation& icon) const;
		std::unique_ptr<MapObject> instantiate(const Texture& icon) const;

	private:
		std::int32_t oid;
		std::int32_t id;
		bool meso;
		std::int32_t owner;
		Point<int16_t> start;
		Point<int16_t> dest;
		std::int8_t droptype;
		std::int8_t mode;
		bool playerdrop;
	};

	class CharSpawn
	{
	public:
		CharSpawn(std::int32_t cid, const LookEntry& look, std::uint8_t level, std::int16_t job, const std::string& name, std::int8_t stance, Point<int16_t> position);

		std::int32_t get_cid() const;
		std::unique_ptr<MapObject> instantiate() const;

	private:
		std::int32_t cid;
		std::uint8_t level;
		std::int16_t job;
		std::string name;
		std::int8_t stance;
		Point<int16_t> position;
		LookEntry look;
	};
}