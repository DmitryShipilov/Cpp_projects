#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment (lib, "ws2_32.lib")

int main(int argc, char* argv[])
{
	// init winsock
	WSADATA wsaData;
	int wsOk = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (wsOk != 0)
	{
		std::cerr << "Can't Initialize winsock! Quitting" << std::endl;
		return -1;
	}

	// Create a socket
	SOCKET listening = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (listening == INVALID_SOCKET)
	{
		std::cerr << "Can't create a socket! Quitting" << std::endl;
		return -1;
	}
	// Setup the UDP listening socket

	sockaddr_in hintServer; 

	hintServer.sin_family = AF_INET;
	hintServer.sin_addr.s_addr = INADDR_ANY;
	hintServer.sin_port = htons(12345);

	int bindResult = bind(listening, (sockaddr *)&hintServer, sizeof(hintServer));
	if (bindResult == SOCKET_ERROR)
	{
		std::cerr << "bind failed: " << WSAGetLastError() << std::endl;
		closesocket(listening);
		WSACleanup();
		return 1;
	}

	// SOCKADDR_IN for client
	sockaddr_in client;
	int clientSize = sizeof(client);

	// buffer for send and recieve
	int const bufSize = 4096;
	char buf[bufSize];

	char host[NI_MAXHOST];		// Client's remote name
	char service[NI_MAXSERV];	// Service (i.e. port) the client is connect on


	while (true) 
	{
		ZeroMemory(host, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST);
		ZeroMemory(service, NI_MAXSERV);

		if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
		{
			std::cout << host << " connected on port " << service << std::endl;
		}
		else
		{
			inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
			std::cout << host << " connected on port " <<
				ntohs(client.sin_port) << std::endl;
		}


		// Accept a client socket
		int const bytesReceived = recvfrom(listening, buf, bufSize, 0, (sockaddr *)&client, &clientSize);
		if (bytesReceived > 0) 
		{
			std::cout << "Received query: " << std::string(buf, 0, bytesReceived) << std::endl;
			
			// отправляем результат на клиенту
			sendto(listening, buf, bytesReceived, 0, (sockaddr *)&client, sizeof(client));
			std::cout << "Sent answer " << std::string(buf, 0, bytesReceived) << std::endl;
		}
		else 
		{
			std::cout << " recv failed: " << WSAGetLastError() << std::endl;
			closesocket(listening);
			WSACleanup();
			return 1;
		}
	}

	return 0;
}