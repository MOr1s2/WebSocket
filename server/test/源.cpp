// server2.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include "stdio.h"
#include "winsock.h"
#pragma comment (lib,"wsock32.lib")

int main(int argc, char* argv[])
{
	SOCKET s, newsock;
	struct sockaddr_in ser_addr, remote_addr;
	int len;
	char buf[128];
	const char* ack_buf = "this is server 2";
	WSAData wsa;

	WSAStartup(0x101, &wsa);

	s = socket(AF_INET, SOCK_STREAM, 0);
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	ser_addr.sin_port = htons(4321);

	bind(s, (sockaddr*)&ser_addr, sizeof(ser_addr));

	listen(s, 0);
	len = sizeof(remote_addr);
	newsock = accept(s, (sockaddr*)&remote_addr, &len);
	printf("\nRemote port= %d, ip addr = %s\n", ntohs(remote_addr.sin_port), inet_ntoa(remote_addr.sin_addr));

	len = recv(newsock, buf, sizeof(buf), 0);
	buf[len] = 0;
	printf("%s\n", buf);

	send(newsock, ack_buf, 16, 0);

	recv(newsock, buf, sizeof(buf), 0);
	printf("%s\n", buf);
	closesocket(newsock);

	closesocket(s);
	WSACleanup();
	return 0;

}
