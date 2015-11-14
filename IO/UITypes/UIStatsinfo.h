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
#include "IO\UIInterface.h"
#include "Net\SessionInterface.h"
#include "Character\Charstats.h"
#include "Graphics\Textlabel.h"

namespace IO
{
	using std::string;
	using Character::Charstats;
	using Graphics::Textlabel;
	using Graphics::Texture;
	using Net::SessionInterface;

	class UIStatsinfo : public UIElement
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

		UIStatsinfo(const Charstats& stats, SessionInterface& session, UIInterface& ui);

		void draw(float inter) const override;
		void buttonpressed(uint16_t buttonid) override;

	private:
		const Charstats& stats;
		SessionInterface& session;
		UIInterface& ui;

		vector<Texture> detailtextures;
		map<string, Texture> abilities;
		Textlabel statlabel;
		bool showdetail;
	};

	class ElementStatsinfo : public Element
	{
	public:
		ElementStatsinfo(const Charstats& st, SessionInterface& ses, UIInterface& u) 
			: stats(st) , session(ses), ui(u) {}

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
			return new UIStatsinfo(stats, session, ui);
		}

	private:
		const Charstats& stats;
		SessionInterface& session;
		UIInterface& ui;
	};
}