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
#include "Server.h"
#include "WindowD2D.h"
#include "AudioplayerBass.h"
#include "Datacache.h"
#include "UI.h"
#include "UILogin.h"
#include "nx.hpp"
#include "node.hpp"
#include "audio.hpp"

using namespace Net;
using namespace Program;

short DPF = 16;
chrono::steady_clock::time_point start;

void evaluate()
{
	chrono::steady_clock::time_point time = chrono::steady_clock::now();
	double elapsed = (chrono::duration_cast<chrono::duration<double>>(time - start)).count();
	DPF = static_cast<int>(1000 * elapsed);
	start = time;
}

int init(WindowD2D& window, Server& server, AudioplayerBass& audiopb, Datacache& cache, UI& ui)
{
	if (!window.init(&ui)) { return 1; }
	if (!server.init(&cache, &ui)) { return 2; }
	if (!audiopb.init(window.getwindow())) { return 3; }
	nl::nx::load_all();
	cache.init();
	ui.init();
	return 0;
}

#ifdef WIN32
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#else
int main()
#endif
{
	WindowD2D window;
	Server server;
	AudioplayerBass audiopb;
	Datacache cache;
	UI ui;

	int error = init(window, server, audiopb, cache, ui);

	if (error == 0)
	{
		audio loginbgm = nx::sound["BgmUI.img"]["Title"].get_audio();
		audiopb.playbgm((void*)loginbgm.data(), loginbgm.length());

		ui.add(&ElementLogin(server.getsession(), &ui));

		start = chrono::steady_clock::now();
		while (server.run())
		{
			window.begin();
			ui.draw();
			window.end();
			window.update();
			ui.update(DPF);
			evaluate();
		}
	}
	else
	{
		switch (error)
		{
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		}
		PostQuitMessage(0);
	}

	return 0;
}