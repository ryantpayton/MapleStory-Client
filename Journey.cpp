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
#include "UILogin.h"
#include "nx.hpp"
#include "node.hpp"
#include "audio.hpp"
/*#include <Windows.h>
#include <chrono>
#include <string>*/
//#include "Game.h"
//#include "WindowD2D.h"
//#include "AudioplayerBass.h"
//#include "Datacache.h"
//#include "Configuration.h"

using namespace Net;
using namespace Program;

short DPF = 16;
chrono::steady_clock::time_point start;

int showerror(string error)
{
	MessageBox(NULL, error.c_str(), NULL, MB_OK);
	return 0;
}

void evaluate()
{
	chrono::steady_clock::time_point time = chrono::steady_clock::now();
	double elapsed = (chrono::duration_cast<chrono::duration<double>>(time - start)).count();
	DPF = static_cast<int>(1000 * elapsed);
	if (DPF > 60)
	{
		DPF = 60;
	}
	start = time;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Server server;
	WindowD2D window;
	AudioplayerBass audiopb;
	Datacache cache;
	UI ui;

	nl::nx::load_all();

	cache.init();

	if (window.init(&ui))
	{
		if (server.init(&cache, &ui))
		{
			if (audiopb.init(window.getwindow()))
			{
				audio loginbgm = nx::sound["BgmUI.img"]["Title"].get_audio();
				audiopb.playbgm((void*)loginbgm.data(), loginbgm.length());

				ui.init();
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
				showerror("Error: Could not initialize audio.");
			}
		}
		else
		{
			showerror("Error: Could not connect to server.");
		}
	}
	else
	{
		showerror("Error: Could not initialize windows application.");
	}

	return 0;
}