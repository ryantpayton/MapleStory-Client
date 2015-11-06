//////////////////////////////////////////////////////////////////////////////
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
#include "Journey.h"
#include "Client.h"
#include "StopWatch.h"
#include <iostream>

using::std::string;
void showerror(string error)
{
	std::cout << error << std::endl;
}

int main()
{
	using::Journey::Client;
	using::Journey::ClientError;
	Client client;

	ClientError error = client.geterror();
	if (error == Journey::CLERR_NOERROR)
	{
		uint16_t dpf = 4;
		uint16_t remain = 0;

		using::Util::StopWatch;
		StopWatch swatch;
		while (client.receive())
		{
			remain += swatch.evaluate();
			while (remain > dpf - 1)
			{
				client.update(dpf);
				remain -= dpf;
			}
			client.draw();
		}
	}
	else
	{
		switch (error)
		{
		case Journey::CLERR_NXFILES:
			showerror("Error: Could not find valid game files.");
			break;
		case Journey::CLERR_CONNECTION:
			showerror("Error: Could not connect to server.");
			break;
		case Journey::CLERR_WINDOW:
			showerror("Error: Could not initialize graphics.");
			break;
		case Journey::CLERR_AUDIO:
			showerror("Error: Could not initialize audio.");
			break;
		}

		while (true) {}
	}

	return 0;
}