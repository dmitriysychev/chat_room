#include "pch.h"
#include "Client.h"


Client::Client()
{
	try {
		result = WSAStartup(MAKEWORD(2, 2), &wsaData);

		if (result != 0) {
			throw 1;
		}

		ZeroMemory(&hints, sizeof(hints));

		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;
	}
	catch (int ex) {
		std::cout << "exeption : " << ex << std::endl;
	}
	
}

void Client::start(std::string HOST, std::string PORT)
{
	try {
		result = getaddrinfo(HOST.c_str(), PORT.c_str(), &hints, &addr);

		if (result != 0) {
			cerr << "getaddrinfo failed: " << result << "\n";
			WSACleanup(); // выгрузка библиотеки Ws2_32.dll
			throw 2;
		}

		int listen_socket = socket(addr->ai_family, addr->ai_socktype,
			addr->ai_protocol);

		if (listen_socket == INVALID_SOCKET) {
			cerr << "Error at socket: " << WSAGetLastError() << "\n";
			freeaddrinfo(addr);
			WSACleanup();
			throw 5;
		}

		result = bind(listen_socket, addr->ai_addr, (int)addr->ai_addrlen);

		if (result == SOCKET_ERROR) {
			cerr << "bind failed with error: " << WSAGetLastError() << "\n";
			freeaddrinfo(addr);
			closesocket(listen_socket);
			WSACleanup();
			throw 6;
		}


	}
	catch (int ex) {
		std::cout << "exeption : " << ex << std::endl;
	}
	result = getaddrinfo(HOST.c_str(), PORT.c_str(), &hints, &addr);
}


Client::~Client()
{
}
