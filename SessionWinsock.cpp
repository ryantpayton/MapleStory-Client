/////////////////////////////////////////////////////////////////////////////
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
#pragma once;
#include "SessionWinsock.h"
#include "CryptographyMaple.h"

namespace Net
{
	SessionWinsock::SessionWinsock()
	{
		crypto = new CryptographyMaple();
		connected = false;
		active = false;
	}

	SessionWinsock::~SessionWinsock()
	{
		close();
		delete crypto;
	}

	bool SessionWinsock::init(ParentHandler* hnd)
	{
		handler = hnd;
		open(HOST.c_str(), LOGINPORT.c_str());
		return connected;
	}

	void SessionWinsock::open(const char* iaddr, const char* port)
	{
		WSADATA wsa_info;
		sock = INVALID_SOCKET;

		struct addrinfo *addr_info = NULL;
		struct addrinfo *ptr = NULL;
		struct addrinfo hints;

		int result = WSAStartup(MAKEWORD(2, 2), &wsa_info);
		if (result != 0) 
		{
			return;
		}

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		result = getaddrinfo(iaddr, port, &hints, &addr_info);
		if (result != 0) 
		{
			WSACleanup();
			return;
		}

		for (ptr = addr_info; ptr != NULL; ptr = ptr->ai_next) 
		{
			sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
			if (sock == INVALID_SOCKET) 
			{
				WSACleanup();
				return;
			}
			result = connect(sock, ptr->ai_addr, (int)ptr->ai_addrlen);
			if (result == SOCKET_ERROR) 
			{
				closesocket(sock);
				sock = INVALID_SOCKET;
				continue;
			}
			break;
		}

		freeaddrinfo(addr_info);

		if (sock == INVALID_SOCKET) 
		{
			WSACleanup();
			return;
		}

		char recvbuf[32];
		result = recv(sock, recvbuf, 32, 0);

		vector<uint8_t> recviv;
		vector<uint8_t> sendiv;
		uint8_t version;
		uint8_t localisation;
		if (result > 0)
		{
			InPacket handshake = InPacket(recvbuf, result);
			handshake.readshort();
			version = static_cast<uint8_t>(handshake.readshort());
			handshake.readshort();
			handshake.readbyte();
			sendiv = handshake.readvector<uint8_t>(4);
			recviv = handshake.readvector<uint8_t>(4);
			localisation = handshake.readbyte();

			crypto->init(recviv, sendiv, version, localisation);

			u_long smode = 1;
			ioctlsocket(sock, FIONBIO, &smode);

			active = true;
			connected = true;
			return;
		}
		else
		{
			WSACleanup();
			return;
		}
	}

	void SessionWinsock::reconnect(vector<uint8_t> addr, int16_t port)
	{
		close();
		string addrstr;
		for (int8_t i = 0; i < 4; i++)
		{
			addrstr.append(to_string(addr[i]));
			if (i < 3)
			{
				addrstr.push_back('.');
			}
		}
		string portstr = to_string(port);
		open(addrstr.c_str(), portstr.c_str());
	}

	void SessionWinsock::dispatch(OutPacket& tosend)
	{
		lock_guard<mutex> lock(sendlock);
		crypto->encrypt(tosend);
		connected = send(sock, tosend.getbytes(), (int)tosend.length(), 0) != SOCKET_ERROR;
	}

	bool SessionWinsock::receive()
	{
		if (active)
		{
			int result = recv(sock, buffer, 10240, 0);

			if (result > 0)
			{
				lock_guard<mutex> lock(recvlock);
				char* bytes = new char[result];
				memcpy(bytes, buffer, result);
				process(bytes, result);
				delete[] bytes;
			}
		}
		return connected;
	}

	void SessionWinsock::process(char* bytes, int result)
	{
		if (bufferpos == 0)
		{
			p_length = crypto->getlength(bytes) + 4;
			int towrite = p_length - 4;
			if (towrite > result - 4)
			{
				towrite = result - 4;
			}
			crypto->decrypt(bytes + 4, towrite, (result >= p_length));
			toread = InPacket(bytes + 4, towrite);
		}
		else
		{
			int towrite = p_length - bufferpos;
			if (towrite > result)
			{
				towrite = result;
			}
			crypto->decrypt(bytes, towrite, (result + bufferpos >= p_length));
			toread.writearray(bytes, towrite);
		}

		bufferpos += result;
		if (bufferpos >= p_length)
		{
			handler->handle(toread);
			if (bufferpos > p_length)
			{
				bufferpos = 0;
				process(bytes + p_length, result - p_length);
			}
			else
			{
				bufferpos = 0;
			}
		}
	}

	void SessionWinsock::close()
	{
		active = false;
		closesocket(sock);
		WSACleanup();
	}
}