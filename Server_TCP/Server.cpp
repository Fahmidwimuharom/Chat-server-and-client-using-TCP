#include <iostream>
#include <string> 
#include <WS2tcpip.h> // windows framework to access network sockets // kerangka kerja windows untuk mengakses soket jaringan

#pragma comment (lib, "ws2_32.lib")

using namespace std;

void main()
{
	/*  Initialize winsock
	The thing that calls the windsock 
	function must be initialized using
	windows to ensure that Winsock is supported on the system */

	WSADATA wsDATA; //The WSADATA structure contains information about the Windows Sockets implementation
	WORD ver = MAKEWORD(2, 2);

	int wsok = WSAStartup(ver, &wsDATA); // The WSAStartup function is called to initiate use of WS2_32.dll.
	if (wsok != 0)
	{
		cerr << " Can't Initialize winsock! Quitting " << endl;
		return;
	}

	// Create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0); //AF_INET is the address used for socket
	if (listening == INVALID_SOCKET)
	{
		cerr << " Can't create a socket! Quitting " << endl;
		return;
	}


	// Bind the  IP addrees and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(12000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY; //also can use inet_pton, Define all interfaces, not only localhost

	(bind(listening, (sockaddr*)&hint, sizeof(hint)) != 0);
		

	// Tell winsock the socket is for listening
	if (listen(listening, SOMAXCONN) != 0)
		cout << "ERROR LISTENING" << endl;
	else
		cout << "SERVER LISTENING" << endl;

	// Wait for a connection
	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

	char host[NI_MAXHOST];  //Client's remote name
	char service[NI_MAXHOST]; // Service(i.o. port) the client is connect on

	ZeroMemory(host, NI_MAXHOST); // same as menset (host, 0, NI_MAXHOST)
	ZeroMemory(service, NI_MAXHOST);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		cout << host << " Connect in port " << service << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " Connected on port " << ntohs(client.sin_port) << endl;
	}
	// Close listening socket
	closesocket(listening);

	// while loop : accept and echo massage back to client.
	char buf[4096];
	string userInput;
	while (true)
	{
		ZeroMemory(buf, 4096);

		//wait for client to send date
		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		cout << "CLIENT : ";
		if (bytesReceived == SOCKET_ERROR)
		{
			cerr << " Error in recv(). Quitting!!" << endl;
			break;
		}

		if (bytesReceived == 0)
		{
			cout << "Client disconnected " << endl;
			break;
		}
		cout << string(buf, 0, bytesReceived) << endl;
		//response message back to client
		cout << "> ";
		getline(cin, userInput);

		//Echo Massage back to client
		send(clientSocket, userInput.c_str() , bytesReceived + 1, 0);

	}
	//Close the sock
	closesocket(clientSocket);

	// Cleanup winsock
	WSACleanup();

}