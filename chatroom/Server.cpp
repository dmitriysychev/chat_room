#include "pch.h"
#include "Server.h"

// Structure that stores information about IP-address of listening socket
struct addrinfo* addr = NULL;

// Template for address structure initialization
struct addrinfo hints;

const int BUFFER_SIZE = 1024;

Server::Server()
{
	std::cout << "Created new server obj" << std::endl;
}


Server::~Server()
{
}

int Server::wsaInit() {
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	
	return result;
}

int Server::wsaGetAddr(int& result, std::string HOST, std::string PORT) {
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP; 
	hints.ai_flags = AI_PASSIVE;

	result = getaddrinfo(HOST.c_str(), PORT.c_str(), &hints, &addr);
	if (result != 0) {
		std::cerr << "getaddrinfo failed: " << result << "\n";
		WSACleanup(); // выгрузка библиотеки Ws2_32.dll
		return 1;
	}
	return result;
}

int Server::start(int& socket_listener){
	socket_listener = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_listener == INVALID_SOCKET) {
		std::cerr << "Error at socket: " << WSAGetLastError() << "\n";
		freeaddrinfo(addr);
		WSACleanup();
		return 1;
	}
}

int Server::bindAddr(int& result, int& listener) {
	result = bind(listener, addr->ai_addr, (int)addr->ai_addrlen);
	if (result == SOCKET_ERROR) {
		std::cerr << "bind failed with error: " << WSAGetLastError() << "\n";
		freeaddrinfo(addr);
		closesocket(listener);
		WSACleanup();
		return 1;
	}
	return result;
}

int Server::acceptConnection(int& socket_listener) {
	int clientSocket = accept(socket_listener, NULL, NULL);
	if (clientSocket == INVALID_SOCKET) {
		std::cerr << "accept failed: " << WSAGetLastError() << "\n";
		closesocket(socket_listener);
		WSACleanup();
		return 1;
	}
	return clientSocket;
}

int Server::recvConnection(int& result, int& socket, char* buf) {

	buf[BUFFER_SIZE];
	result = recv(socket, buf, BUFFER_SIZE, 0);
	std::cout << "Result in recvConnection(): " << socket << std::endl;
	if (result == SOCKET_ERROR) {
		std::cerr << "recv failed: " << result << "\n";
		closesocket(socket);
		return 0;
	}
	else if (result == 0) {
		// Connection was closed by the client
		std::cerr << "connection closed...\n";
		return 0;
	}
	else if (result > 0) {
		return result;
	}

}




