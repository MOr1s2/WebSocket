// example2.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include "stdio.h"
#include "winsock.h"
#pragma comment (lib,"wsock32.lib")
int main(int argc, char* argv[])
{
	SOCKET s;
	WSAData wsa;
	int retval;
	int i = 0;
	sockaddr_in remote;
	const char* send_buf = "this is example 2";
	char recv_buf[100];

	WSAStartup(0x101, &wsa);

	while (i++ < 5) {
		s = socket(AF_INET, SOCK_STREAM, 0);
		if (s == INVALID_SOCKET) {
			retval = WSAGetLastError();
		}
	}

	remote.sin_family = AF_INET;
	remote.sin_addr.S_un.S_addr = htonl(INADDR_LOOPBACK);
	remote.sin_port = htons(4321);

	retval = connect(s, (sockaddr*)&remote, sizeof(remote));
	if (retval == SOCKET_ERROR)
		retval = WSAGetLastError();
	retval = send(s, send_buf, 17, 0);
	retval = recv(s, recv_buf, 100, 0);
	recv_buf[retval] = 0;
	printf("%s\n", recv_buf);
	WSACleanup();
	return 0;
}
