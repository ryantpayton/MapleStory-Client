//////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015 SYJourney                                               //
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
#include "SessionWinsock.h"
#include "Packethandler.h"
#include "WindowD2D.h"
#include "AudioplayerBass.h"
#include "Datacache.h"
#include "Stage.h"
#include "UI.h"
#include "StopWatch.h"
#include "UILogin.h"
#include "nx.hpp"
#include "node.hpp"
#include "audio.hpp"

using namespace Net;
using namespace Program;

void showerror(string error)
{
	MessageBox(NULL, error.c_str(), NULL, MB_OK);
}

int init(SessionWinsock& session, ParentHandler& handler, WindowD2D& window, AudioplayerBass& audiopb, Datacache& cache, UI& ui)
{
	if (!session.init(&handler)) { return 1; }
	if (!window.init(&ui)) { return 2; }
	if (!audiopb.init(window.getwindow())) { return 3; }
	return 0;
}

#ifdef WIN32
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#else
int main()
#endif
{
	nl::nx::load_all();

	SessionWinsock session;
	WindowD2D window;
	AudioplayerBass audiopb;
	Datacache cache;
	Stage stage;
	UI ui;
	Login login;
	Packethandler handler(cache, stage, ui, login, session);

	int error = init(session, handler, window, audiopb, cache, ui);
	if (error == 0)
	{
		audio loginbgm = nx::sound["BgmUI.img"]["Title"].get_audio();
		audiopb.playbgm((void*)loginbgm.data(), loginbgm.length());

		cache.init();
		ui.init();
		stage.init();
		ui.add(ElementLogin(session, ui));

		StopWatch swatch;
		short dpf = 16;
		while (session.receive())
		{
			window.begin();
			stage.draw();
			ui.draw();
			window.end();

			window.update();
			stage.update(dpf);
			ui.update(dpf);

			dpf = swatch.evaluate();
		}
	}
	else
	{
		switch (error)
		{
		case 1:
			showerror("Error: Could not connect to server.");
			break;
		case 2:
			showerror("Error: Could not initialize window.");
			break;
		case 3:
			showerror("Error: Could not initialize audio.");
			break;
		}
	}

	return 0;
}