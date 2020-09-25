//connectless client
#include <stdio.h>
#include <winsock.h>
#pragma comment(lib,"wsock32.lib")

int main()
{
	int len;
	char buf[128];
	char recvbuf[128];
	memset(buf, 0, sizeof(buf));
	memset(recvbuf, 0, sizeof(buf));

	WSAData wsa;
	SOCKET s;
	sockaddr_in local,remote;

	WSAStartup(0x101, &wsa);

	s = socket(AF_INET, SOCK_DGRAM, 0);
	local.sin_family = AF_INET;
	local.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	local.sin_port = htons(0x3412);
	bind(s, (sockaddr*)&local, sizeof(local));

	remote.sin_family = AF_INET;
	remote.sin_addr.S_un.S_addr = htonl(INADDR_LOOPBACK);
	remote.sin_port = htons(0x1234);
	len = sizeof(remote);

	printf("to server:");
	scanf_s("%s", buf,sizeof(buf));
	sendto(s,buf,sizeof(buf), 0, (sockaddr*)&remote, len);
	recvfrom(s, recvbuf, sizeof(recvbuf), 0, (sockaddr*)&remote, &len);
	printf("server:%s", recvbuf);
	closesocket(s);
	WSACleanup();
	return 0;
}
