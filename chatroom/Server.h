#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>

#define _WIN32_WINNT 0x501

#pragma comment(lib, "Ws2_32.lib")

class Server
{
public:
	Server();
	~Server();

	int start(int& socket_listener);
	int wsaInit();
	int wsaGetAddr(int& result, std::string HOST, std::string PORT);
	int bindAddr(int& result, int& listener);
	int acceptConnection(int& socket);

	int recvConnection(int & result, int& socket, char* buf);

private:
	WSADATA wsaData;
};

