#include "pch.h"
#include "Server.h"


Server::Server()
{
	std::cout << "Created new server obj" << std::endl;
}


Server::~Server()
{
}

void Server::d_print()
{
	std::cout << "d_print " << delim << std::endl;
}
