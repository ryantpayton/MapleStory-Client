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
#include "Graphics\Text.h"

namespace IO
{
	using Character::CharStats;
	using Character::Maplestat;
	using Character::Equipstat;
	using Graphics::Text;
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

		UIStatsinfo();

		void draw(float inter) const override;
		void update() override;
		void buttonpressed(uint16_t buttonid) override;

	private:
		UIStatsinfo& operator = (const UIStatsinfo&) = delete;

		static const size_t NUMLABELS = 27;
		static const size_t NUMNORMAL = 12;
		static const size_t NUMDETAIL = 15;
		enum StatLabel
		{
			// Normal
			NAME, JOB, GUILD, FAME, DAMAGE,
			HP, MP, AP, STR, DEX, INT, LUK,
			// Detailed
			ATTACK, CRIT, MINCRIT, MAXCRIT,
			BDM, IGNOREDEF, RESIST, STANCE,
			WDEF, MDEF, ACCURACY, AVOID,
			SPEED, JUMP, HONOR
		};

		void updateap();
		void updatesimple(StatLabel label, Maplestat::Value stat);
		void updatebasevstotal(StatLabel label, Maplestat::Value bstat, Equipstat::Value tstat);
		void updatebuffed(StatLabel label, Equipstat::Value stat);
		void sendappacket(Maplestat::Value stat) const;

		const CharStats& stats;

		vector<Texture> detailtextures;
		map<string, Texture> abilities;
		bool showdetail;

		bool hasap;

		Text statlabels[NUMLABELS];
		Point<int16_t> statoffsets[NUMLABELS];
	};

	class ElementStatsinfo : public Element
	{
		bool isunique() const override
		{
			return true;
		}

		UIElement::Type type() const override
		{
			return UIElement::STATSINFO;
		}

		UIElement* instantiate() const override
		{
			return new UIStatsinfo();
		}
	};
}