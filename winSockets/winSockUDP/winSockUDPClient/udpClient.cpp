#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

int main(int argc, char* argv[])
{
	std::string ipAddress = "127.0.0.1";
	int const port = 12345;

	WSADATA wsaData;
	int wsOk = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (wsOk != 0)
	{
		std::cerr << "Can't Initialize winsock! Quitting" << std::endl;
		return -1;
	}

	SOCKET clientSocket;  // сокет для приема и передачи

	// Create a SOCKET for connecting to server
	clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (clientSocket == INVALID_SOCKET)
	{
		std::cerr << "Can't create socket, Err # " << WSAGetLastError() << std::endl;
		WSACleanup();
		return -1;
	}

	sockaddr_in serverAddr;  // это будет адрес сервера и клиентов

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &serverAddr.sin_addr);

	int const bufSize = 4096;
	char buf[bufSize];
	std::string userInput;
	
	do
	{
		std::cout << "> ";
		std::getline(std::cin, userInput);

		if (userInput.size() > 0)
		{
			int const sendResult = sendto(clientSocket, userInput.c_str(), userInput.size(), 0, (sockaddr *)&serverAddr, sizeof(serverAddr));
			if (sendResult != SOCKET_ERROR)
			{
				ZeroMemory(buf, bufSize);
				int const recvResult = recvfrom(clientSocket, buf, bufSize, 0, 0, 0);
				if (recvResult > 0)
					std::cout << "Server: " << std::string(buf, 0, recvResult) << std::endl;
				else
				{
					std::cerr << "recv failed: " << WSAGetLastError() << std::endl;
					closesocket(clientSocket);
					WSACleanup();
					return 1;
				}
			}
		}
	} while (userInput.size() > 0);

	return 0;
}