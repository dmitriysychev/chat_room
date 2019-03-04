//#include "pch.h"
//#include <iostream>
//#include <string>
//#include "Client.h"
//
//int main()
//{
//	std::string HOST = "127.0.0.1", PORT = "8000";
//	Client client = Client();
//	client.start(HOST, PORT);
//}
#include "pch.h"
#include <winsock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <iostream>

int ResolveHostName(const char* pszHostName, sockaddr_in* pAddr)
{

	int ret;
	HRESULT hr = S_OK;
	addrinfo* pResultList = NULL;
	addrinfo hints = {};
	int result = -1;

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	ret = getaddrinfo(pszHostName, NULL, &hints, &pResultList);

	result = (ret == 0) ? 1 : -1;
	if (result != -1)
	{
		// just pick the first one found
		*pAddr = *(sockaddr_in*)(pResultList->ai_addr);
		result = 0;
	}

	if (pResultList != NULL)
	{
		::freeaddrinfo(pResultList);
	}

	return result;
}



int main()
{
	SOCKET sock = -1;
	WSADATA data = {};
	sockaddr_in addrRemote = {};
	int result;

	WSAStartup(MAKEWORD(2, 2), &data);


	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock != -1)
	{
		if (ResolveHostName("127.0.0.1", &addrRemote) != -1)
		{
			addrRemote.sin_port = htons(8000);
			result = connect(sock, (sockaddr*)&addrRemote, sizeof(addrRemote));

			if (result != -1)
			{
				const char* msg = "GET / HTTP/1.1\r\nHost: 127.0.0.1:8000\r\nConnection: close\r\n\r\n";
				int msg_len = strlen(msg);
				result = send(sock, msg, msg_len, 0);
				if (result != -1)
				{
					char szBuffer[10000 + 1];
					result = recv(sock, szBuffer, 10000, 0);
					if (result > 0)
					{
						// safely null terminate
						szBuffer[result] = '\0';
						printf("%s\n", szBuffer);
					}
				}
			}
		}
	}

	if (sock != -1)
	{
		closesocket(sock);
		sock = -1;
	}

	return 0;
}