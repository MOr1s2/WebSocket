//connectless server
#include <stdio.h>
#include <winsock.h>
#pragma comment(lib,"wsock32.lib")

int main()
{
	int len;
	const char buf[128]="ACK";
	char recvbuf[128];
	//memset(buf, 0, sizeof(buf));
	memset(recvbuf, 0, sizeof(buf));
	
	WSAData wsa;
	SOCKET s;
	sockaddr_in local,remote;

	WSAStartup(0x101, &wsa);

	s = socket(AF_INET,SOCK_DGRAM,0);
	local.sin_family = AF_INET;
	local.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	local.sin_port = htons(0x1234);
	bind(s,(SOCKADDR*)&local,sizeof(local));

	remote.sin_family = AF_INET;
	remote.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	remote.sin_port = htons(0x3412);
	len = sizeof(remote);

	while (1) {
		recvfrom(s, recvbuf, sizeof(recvbuf), 0,(sockaddr*)&remote,&len);
		printf("client:%s\n", recvbuf);
		printf("to client:%s\n",buf);
		//scanf_s("%s", buf,sizeof(buf));
		sendto(s,buf,sizeof(buf),0,(sockaddr*)&remote,len);

	}
	closesocket(s);
	WSACleanup();
	return 0;
}
