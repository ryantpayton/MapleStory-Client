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
#include "IO\Element.h"
#include "IO\UIDragElement.h"
#include "Character\CharStats.h"
#include "Graphics\Textlabel.h"

namespace IO
{
	using Character::CharStats;
	using Character::Maplestat;
	using Graphics::Textlabel;
	using Graphics::Texture;

	class UIStatsinfo : public UIDragElement
	{
	public:
		enum Buttons
		{
			BT_HP,
			BT_MP,
			BT_STR,
			BT_DEX,
			BT_INT,
			BT_LUK,
			BT_DETAILOPEN,
			BT_DETAILCLOSE
		};

		UIStatsinfo(const CharStats& stats);

		void draw(float inter) const override;
		void buttonpressed(uint16_t buttonid) override;

		void updateap();

	private:
		UIStatsinfo& operator = (const UIStatsinfo&) = delete;

		void sendappacket(Maplestat stat);

		const CharStats& stats;

		vector<Texture> detailtextures;
		map<string, Texture> abilities;
		Textlabel statlabel;
		bool showdetail;
	};

	class ElementStatsinfo : public Element
	{
	public:
		ElementStatsinfo(const CharStats& st) : stats(st) {}

		bool isunique() const override
		{
			return true;
		}

		UIType type() const override
		{
			return STATSINFO;
		}

		UIElement* instantiate() const override
		{
			return new UIStatsinfo(stats);
		}

	private:
		ElementStatsinfo& operator = (const ElementStatsinfo&) = delete;

		const CharStats& stats;
	};
}