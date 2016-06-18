/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015-2016 Daniel Allendorf                                   //
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

//Define things here.

// JOURNEY_USE_ASIO : Use asio for networking, if not defined use Winsock.
//#define JOURNEY_USE_ASIO

// JOURNEY_USE_XXHASH : Use xxhash for file check (additional dependency)
//#define JOURNEY_USE_XXHASH

// JOURNEY_USE_CRYPTO : Use cryptography for communication with the server.
#define JOURNEY_USE_CRYPTO

// JOURNEY_PRINT_WARNINGS : Print warnings and minor errors to the console.
#define JOURNEY_PRINT_WARNINGS