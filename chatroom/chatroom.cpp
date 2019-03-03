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
	server.recvConnection(result_handler, sock_handler, feedback, feedback_body);



}
