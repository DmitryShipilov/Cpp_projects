///************
// CLIENT     *
//*************
#include <iostream>
#include <WS2tcpip.h>
#include <string>

#pragma comment (lib, "ws2_32.lib")

int main()
{
	std::string ipAddress = "127.0.0.1";	// IP address of the server
	int const port = 54000;						// port of the server

	// Initialize WinSock
	WSADATA data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		std::cerr << "Can't start winSock, Error # " << wsResult << std::endl;
		return -1;
	}

	// Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		std::cerr << "Can't create socket, Err # " << WSAGetLastError() << std::endl;
		WSACleanup();
		return -1;
	}

	// Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	// Connect to server 
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		std::cerr << "Can't connect to server, Err # " << WSAGetLastError() << std::endl;
		closesocket(sock);
		WSACleanup();
		return -1;
	}

	// Do while loop: send and receive data
	char buf[4096];
	std::string userInput;

	do
	{
		// Prompt the user to send text
		std::cout << "> ";
		std::getline(std::cin, userInput);

		if (userInput.size() > 0)
		{
			// Send the text
			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
			if (sendResult != SOCKET_ERROR)
			{
				// Wait for response
				ZeroMemory(buf, 4096);
				int byteReceived = recv(sock, buf, 4096, 0);
				if (byteReceived > 0)
				{
					// Echo response to console
					std::cout << "SERVER " << std::string(buf, 0, byteReceived) << std::endl;
				}
			}
		}
	} while (userInput.size() > 0);

	// Colse down everything
	closesocket(sock);
	WSACleanup();

    return 0;
}

