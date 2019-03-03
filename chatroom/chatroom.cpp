#include "pch.h"
#include <iostream>
#include <string>
#include "Server.h"


int sock_handler, client_socket;
std::string HOST = "127.0.0.1";
std::string PORT = "8000";
std::stringstream feedback; // сюда будет записываться ответ клиенту
std::stringstream feedback_body; // тело ответа

int main()
{
	Server server = Server();

	// Preparation steps
	int result_handler = server.wsaInit();
	
	if (result_handler != 0) {
		std::cerr << "WSAStartup failed: " << result_handler << "\n";
		return result_handler;
	}
	server.wsaGetAddr(result_handler, HOST, PORT);

	// Upon successful preparation of WSA, socket opening and starting is executed
	server.start(sock_handler);
	server.bindAddr(result_handler, sock_handler);
	
	if (listen(sock_handler, SOMAXCONN) == SOCKET_ERROR) {
		std::cerr << "listen failed with error: " << WSAGetLastError() << "\n";
		closesocket(sock_handler);
		WSACleanup();
		return 1;
	} 
	client_socket = server.acceptConnection(sock_handler);

	char buf[1024];
	int result = server.recvConnection(result_handler, sock_handler, buf);
	std::cout << "Result handler = " << result_handler << std::endl;
	if (result != 0) {
		buf[result_handler] = '\0';
		feedback_body << "<title>Test C++ HTTP Server</title>\n"
			<< "<h1>Test page</h1>\n"
			<< "<p>This is body of the test page...</p>\n"
			<< "<h2>Request headers</h2>\n"
			<< "<pre>" << buf << "</pre>\n"
			<< "<em><small>Test C++ Http Server</small></em>\n";


		feedback << "HTTP/1.1 200 OK\r\n"
			<< "Version: HTTP/1.1\r\n"
			<< "Content-Type: text/html; charset=utf-8\r\n"
			<< "Content-Length: " << feedback_body.str().length()
			<< "\r\n\r\n"
			<< feedback_body.str();

		result = send(sock_handler, feedback.str().c_str(),
			feedback.str().length(), 0);
		if (result == SOCKET_ERROR) {
			std::cerr << "send failed: " << WSAGetLastError() << "\n";
		}
	}
	closesocket(sock_handler);
}
