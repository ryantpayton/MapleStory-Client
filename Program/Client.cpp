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
#include "Client.h"
#include "Configuration.h"
#include "Util\NxFileMethods.h"
#include "IO\UI.h"
#include "Gameplay\Stage.h"
#include "Net\Session.h"
#include "Audio\Audioplayer.h"

#include "IO\UITypes\UILogin.h"
#include "Character\Look\CharLook.h"
#include "nlnx\nx.hpp"
#include "nlnx\audio.hpp"

namespace Program
{
	Client::Client() {}

	Client::~Client() 
	{
		Audioplayer::close();
		Configuration::save();
	}

	Client::Error Client::init()
	{
		if (!Util::NxFileMethods::init())
			return NXFILES;

		if (!Net::Session::init())
			return CONNECTION;

		if (!window.init())
			return WINDOW;

		if (!Audioplayer::init())
			return AUDIO;

		Configuration::load();

		IO::UI::init();
		Gameplay::Stage::init();
		Character::CharLook::init();

		IO::UI::add(IO::ElementLogin());

		Audioplayer::setbgmvolume(Configuration::getbyte("BGMVolume"));
		Audioplayer::setsfxvolume(Configuration::getbyte("SFXVolume"));
		Audioplayer::playbgm("BgmUI.img/Title");

		return NONE;
	}

	bool Client::receive() const
	{
		return Net::Session::receive();
	}

	void Client::draw(float inter) const
	{
		window.begin();
		Gameplay::Stage::draw(inter);
		IO::UI::draw(inter);
		window.end();
	}

	void Client::update()
	{
		window.update();
		Gameplay::Stage::update();
		IO::UI::update();
	}
}
