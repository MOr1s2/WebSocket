// coclient.cpp : Defines the entry point for the console application.

#include "winsock.h"
#include "stdio.h"
#pragma comment (lib,"wsock32.lib")
int main(int argc, char* argv[])
{
	SOCKET s;
	struct sockaddr_in server;
	WSAData wsa;
	int retval;
	char buf[128];

	WSAStartup(0x101, &wsa);

	s = socket(AF_INET, SOCK_STREAM, 0);

	server.sin_family = AF_INET;
	server.sin_addr.S_un.S_addr = htonl(INADDR_LOOPBACK);
	server.sin_port = htons(0x1234);
	retval = connect(s, (sockaddr*)&server, sizeof(server));
	if (retval != 0)
		return 0;
	printf("Connect to server!\n");
	while (1) {
		memset(buf, 0, sizeof(buf));
		printf("Enter the string to send : ");
		scanf("%s", buf);
		if (!strcmp(buf, "exit")) {
			break;
		}
		send(s, buf, strlen(buf), 0);
		retval = recv(s, buf, 128, 0);
		if (retval == -1) {
			break;
		}
		//buf[retval] = 0;
		printf("->%s\n", buf);
	}
	closesocket(s);
	WSACleanup();
	return 0;
}