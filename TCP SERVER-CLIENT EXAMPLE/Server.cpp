#define _CRT_SECURE_NO_WARNINGS
/*
	Bind socket to port 8888 on localhost
*/
#include "httpd.h"

#include<io.h>
#include<stdio.h>
#include<winsock2.h>
#include <string.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

int main1(int argc, char* argv[])
{
	char STATUS_LINE[100] = "HTTP/1.0 200 OK\n\n";

	char RESPONSE_STR[100] = "<H1 style='color:red'>Hello World</H1>";



	WSADATA wsa;
	SOCKET s, new_socket;
	struct sockaddr_in server, client;
	int message_len = 0, c;
	char message[200] = { 0 };

	char recvbuf[1024];
	int recvbuflen = 1024;

	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}

	printf("Initialised.\n");

	//Create a socket
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}

	printf("Socket created.\n");

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(27015);

	//Bind
	if (bind(s, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
	}

	puts("Bind done");

	//Listen to incoming connections
	listen(s, 3);

	//Accept and incoming connection
	puts("Waiting for incoming connections...");

	c = sizeof(struct sockaddr_in);
	//while (1) {
		new_socket = accept(s, (struct sockaddr*)&client, &c);
		if (new_socket == INVALID_SOCKET)
		{
			printf("accept failed with error code : %d", WSAGetLastError());
		}

		printf("Connection accepted\n");

		
		/*
		memset(message, 0, sizeof(message));
		
		strcpy(message, "HTTP/1.1 200 OK\r\nContent-Length: ");
		sprintf(message, "%d\r\n", strlen(RESPONSE_STR));*/

		strcpy(message, STATUS_LINE);
		strcat(message, RESPONSE_STR);

		send(new_socket, message, strlen(message), 0);

		memset(message, 0, sizeof(message));
		message_len = 0;
		closesocket(new_socket);
	//}// end while

	closesocket(s);
	WSACleanup();
	return 0;
}
