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

void Server::recvConnection(int& result, int& socket, std::stringstream& response, 
	std::stringstream& response_body) {

	char buf[BUFFER_SIZE];
	result = recv(socket, buf, BUFFER_SIZE, 0);
	bool working = true;
	while (working) {
		if (result == SOCKET_ERROR) {
			// ошибка получения данных
			std::cerr << "recv failed: " << result << "\n";
			closesocket(socket);
			working = false;
		}
		else if (result == 0) {
			// Connection was closed by the client
			std::cerr << "connection closed...\n";
		}
		else if (result > 0) {
			buf[result] = '\0';
			// Данные успешно получены
		// формируем тело ответа (HTML)
			response_body << "<title>Test C++ HTTP Server</title>\n"
				<< "<h1>Test page</h1>\n"
				<< "<p>This is body of the test page...</p>\n"
				<< "<h2>Request headers</h2>\n"
				<< "<pre>" << buf << "</pre>\n"
				<< "<em><small>Test C++ Http Server</small></em>\n";

			// Формируем весь ответ вместе с заголовками
			response << "HTTP/1.1 200 OK\r\n"
				<< "Version: HTTP/1.1\r\n"
				<< "Content-Type: text/html; charset=utf-8\r\n"
				<< "Content-Length: " << response_body.str().length()
				<< "\r\n\r\n"
				<< response_body.str();

			// Отправляем ответ клиенту с помощью функции send
			result = send(socket, response.str().c_str(),
				response.str().length(), 0);

			if (result == SOCKET_ERROR) {
				// произошла ошибка при отправле данных
				std::cerr << "send failed: " << WSAGetLastError() << "\n";
			}
			// Закрываем соединение к клиентом
			closesocket(socket);
		}
	}

}




