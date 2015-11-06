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
#include "SessionServer.h"
#include "AudioplayerBass.h"
#include "NxFileManager.h"
#include "UI.h"
#include "Stage.h"
#include "ClientInterface.h"

#include "Journey.h"
#ifdef JOURNEY_USE_OPENGL
#include "WindowGLFW.h"
#else
#include "WindowD2D.h"
#endif

namespace Journey
{
	// Error codes to be checked after initialisation.
	enum ClientError
	{
		CLERR_NOERROR,
		CLERR_NXFILES,
		CLERR_CONNECTION,
		CLERR_WINDOW,
		CLERR_AUDIO
	};

#ifdef JOURNEY_USE_OPENGL
	using::IO::WindowGLFW;
#else
	using::IO::WindowD2D;
#endif

	using::Net::SessionServer;
	using::Program::AudioplayerBass;
	using::Util::Configuration;
	using::Gameplay::Stage;

	// Topmost class in the hierarchy that represents the program itself. Contains all game- and networking-objects.
	class Client : public ClientInterface
	{
	public:
		Client();
		~Client();
		// Returns the last error code.
		ClientError geterror() const;
		// Checks for incoming packets and returns if the connection is still alive.
		bool receive();
		// Draws the window and all game objects.
		void draw() const;
		// Processes inputs and updates the window and all game objects.
		// Parameters: short(delay-per-frame)
		void update(uint16_t);
		// Methods for obtaining references to game objects.
		Window& getwindow();
		Audioplayer& getaudio();
		Datacache& getcache();
		StageInterface& getstage();
		UI& getui();
		Login& getlogin();
		Session& getsession();
		Configuration& getconfig();
		NxFileManager& getnxfiles();
	private:

#ifdef JOURNEY_USE_OPENGL
		WindowGLFW window;
#else
		WindowD2D window;
#endif

		AudioplayerBass audioplayer;
		NxFileManager nxfiles;
		Datacache cache;
		Stage stage;
		UI ui;
		Login login;
		SessionServer session;
		Configuration config;
		ClientError error;
	};
}

