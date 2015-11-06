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
#include "UILogin.h"
#include "nx.hpp"
#include "audio.hpp"

namespace Journey
{
	Client::Client() : stage(*this)
	{
		error = nxfiles.init() ? CLERR_NOERROR : CLERR_NXFILES;
		error = (error == CLERR_NOERROR) ? session.init() ? CLERR_NOERROR : CLERR_CONNECTION : error;
#ifdef JOURNEY_USE_OPENGL
		error = (error == CLERR_NOERROR) ? window.init(config.getbool("Fullscreen")) ? CLERR_NOERROR : CLERR_WINDOW : error;
#else
		error = (error == CLERR_NOERROR) ? window.init(&ui) ? CLERR_NOERROR : CLERR_WINDOW : error;
#endif
		error = (error == CLERR_NOERROR) ? !audioplayer.geterror() ? CLERR_NOERROR : CLERR_AUDIO : error;

		if (error == CLERR_NOERROR)
		{
			cache.init();
			ui.init();
			stage.init();

			using::IO::ElementLogin;
			ui.add(ElementLogin(window, session, ui, config));

			using::nl::audio;
			audio loginbgm = nl::nx::sound["BgmUI.img"]["Title"].get_audio();
			audioplayer.playbgm((void*)loginbgm.data(), loginbgm.length());
		}
	}

	Client::~Client() {}

	ClientError Client::geterror() const
	{
		return error;
	}

	bool Client::receive()
	{
		return session.receive(*this);
	}

	void Client::draw() const
	{
		window.begin();
		stage.draw();
		ui.draw();
		window.end();
	}

	void Client::update(uint16_t dpf)
	{
#ifdef JOURNEY_USE_OPENGL
		window.update(ui);
#else
		window.update();
#endif
		stage.update(dpf);
		ui.update(dpf);
	}

	Window& Client::getwindow()
	{
		return window;
	}

	Audioplayer& Client::getaudio()
	{
		return audioplayer;
	}

	Datacache& Client::getcache()
	{
		return cache;
	}

	StageInterface& Client::getstage()
	{
		return stage;
	}

	UI& Client::getui()
	{
		return ui;
	}

	Login& Client::getlogin()
	{
		return login;
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
