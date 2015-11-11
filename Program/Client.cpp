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
#include "Client.h"
#include "IO\UITypes\UILogin.h"
#include "Character\Look\CharLook.h"
#include "nlnx\nx.hpp"
#include "nlnx\audio.hpp"

namespace Journey
{
	Client::Client() : stage(*this)
	{
		error = nxfiles.init() ? NONE : NXFILES;
		error = (error == NONE) ? session.init() ? NONE : CONNECTION : error;
#ifdef JOURNEY_USE_OPENGL
		error = (error == NONE) ? window.init(config.getbool("Fullscreen")) ? NONE : WINDOW : error;
#else
		error = (error == NONE) ? window.init(&ui) ? NONE : WINDOW : error;
#endif
		error = (error == NONE) ? !audioplayer.geterror() ? NONE : AUDIO : error;

		if (error == NONE)
		{
			ui.init();
			stage.init();

			using::Character::CharLook;
			CharLook::init();

			using::IO::ElementLogin;
			ui.add(ElementLogin(session, ui, config));

			using::nl::audio;
			audio loginbgm = nl::nx::sound["BgmUI.img"]["Title"].get_audio();
			audioplayer.playbgm((void*)loginbgm.data(), loginbgm.length());
		}
	}

	Client::~Client() {}

	Client::Error Client::geterror() const
	{
		return error;
	}

	bool Client::receive()
	{
		return session.receive(*this);
	}

	void Client::draw(float inter) const
	{
		window.begin();
		stage.draw(inter);
		ui.draw(inter);
		window.end();
	}

	void Client::update(uint16_t dpf)
	{
#ifdef JOURNEY_USE_OPENGL
		window.update(ui);
#else
		window.update();
#endif
		stage.update();
		ui.update();
	}

	Audioplayer& Client::getaudio()
	{
		return audioplayer;
	}

	StageInterface& Client::getstage()
	{
		return stage;
	}

	UI& Client::getui()
	{
		return ui;
	}

	Session& Client::getsession()
	{
		return session;
	}

	Configuration& Client::getconfig()
	{
		return config;
	}

	NxFileManager& Client::getnxfiles()
	{
		return nxfiles;
	}
}
