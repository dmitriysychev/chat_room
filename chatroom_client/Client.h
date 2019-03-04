#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

using std::cerr;

class Client
{
public:
	Client();
	void start(std::string HOST, std::string PORT);
	~Client();

private:

	WSADATA wsaData;
	int result;
	struct addrinfo* addr = NULL;
	struct addrinfo hints;
};

