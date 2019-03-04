#pragma once
#include <iostream>
class Server
{
public:
	int count = 1;
	std::string test = "test";
	Server();
	~Server();
private:
	int delim = 15;
	void d_print(); //debug printing
};

