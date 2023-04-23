//////////////////////////////////////////////////////////////////////////////////
//	This file is part of the continued Journey MMORPG client					//
//	Copyright (C) 2015-2019  Daniel Allendorf, Ryan Payton						//
//																				//
//	This program is free software: you can redistribute it and/or modify		//
//	it under the terms of the GNU Affero General Public License as published by	//
//	the Free Software Foundation, either version 3 of the License, or			//
//	(at your option) any later version.											//
//																				//
//	This program is distributed in the hope that it will be useful,				//
//	but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				//
//	GNU Affero General Public License for more details.							//
//																				//
//	You should have received a copy of the GNU Affero General Public License	//
//	along with this program.  If not, see <https://www.gnu.org/licenses/>.		//
//////////////////////////////////////////////////////////////////////////////////
#pragma once

// If defined use Asio for networking, otherwise use Winsock.
//#define USE_ASIO

// Use cryptography for communication with the server
#define USE_CRYPTO

// If defined use NX, otherwise use WZ.
#define USE_NX

// Debug options
#define LOG_ERROR	1
#define LOG_WARN	2
#define LOG_INFO	3
#define LOG_DEBUG	4
#define LOG_NETWORK	5
#define LOG_UI		6
#define LOG_TRACE	7

// Log Level
#ifdef _DEBUG
	#define LOG_LEVEL LOG_DEBUG
#else
	#define LOG_LEVEL LOG_WARN
#endif

// Log Text
#define LOG_TEXT(level) (\
	level == LOG_ERROR		? "ERROR"	:\
	level == LOG_WARN		? "WARN"	:\
	level == LOG_INFO		? "INFO"	:\
	level == LOG_DEBUG		? "DEBUG"	:\
	level == LOG_NETWORK	? "NETWORK"	:\
	level == LOG_UI			? "UI"		:\
	level == LOG_TRACE		? "TRACE"	: "UNDEFINED")

// Log Commands
#ifdef _DEBUG
	#define LOG(level, message) level <= LOG_LEVEL ? std::cout << "[" << LOG_TEXT(level) << "]: " << message << std::endl : std::cout
#else
	#define LOG(level, message) void(0)
#endif