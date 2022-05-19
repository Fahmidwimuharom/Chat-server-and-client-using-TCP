#include <iostream>
#include <WS2tcpip.h>
#include <string>

#pragma comment (lib, "ws2_32.lib")

using namespace std;

void main()
{
	string ipAdress = "127.0.0.1"; // IP Adress
	int port = 12000; //Port

	/*  Initialize winsock
	The thing that calls the windsock
	function must be initialized using
	windows to ensure that Winsock is supported on the system */

	WSAData data; //The WSADATA structure contains information about the Windows Sockets implementation
	WORD ver = MAKEWORD(2, 2); 
	int wsResult = WSAStartup(ver, &data); //The WSAStartup function is called to initiate use of WS2_32.dll.
	if (wsResult != 0)
	{
		cerr << " Can't start winsock, Err #" << wsResult << endl;
		return;
	}
	// Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0); // AF_INET is the address used for socket
	if (sock == INVALID_SOCKET)
	{
		cerr << " Can't create socket, Err #" << WSAGetLastError << endl;
		return;
	}

	// Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAdress.c_str(), &hint.sin_addr);


	// Connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cerr << " Can't connect to server, Err #" << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return;
	}

	// Do-While loop that functions to send and receive data
	char buf[4096];
	string userInputl;

	do
	{
		//Prompt the user for some text
		cout << " > ";
		getline(cin, userInputl);
		if (userInputl.size() > 0)
		{
			// Sending Text
			int sendResult = send(sock, userInputl.c_str(), userInputl.size() + 1, 0);
			if (sendResult != SOCKET_ERROR)
			{
				//wait for response
				ZeroMemory(buf, 4096);

				int bytesReceived = recv(sock, buf, 4096, 0);
				if (bytesReceived > 0)
				{
					cout << "SERVER > " << string (buf, 0, bytesReceived) << endl;
					
				}
				(bytesReceived > 0);
			}

		}


	} 
	while (userInputl.size() > 0);
	

	// Gracefully close down everything
	closesocket(sock);
	WSACleanup();
}