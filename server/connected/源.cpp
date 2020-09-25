///server_connected
#include "stdio.h"
#include <winsock.h>
#pragma comment(lib,"wsock32.lib")

int main() {

	int len, r;
	const char buf[128]="I am a server";
	//char buf[128];
	char recvbuf[128];
	//memset(buf, 0, sizeof(buf));
	memset(recvbuf, 0, sizeof(recvbuf));
	SOCKET s, news;
	WSAData wsa;
	sockaddr_in local;
	sockaddr_in remote;

	WSAStartup(0x101, &wsa);

	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == SOCKET_ERROR) {
		r = WSAGetLastError();
		printf("socker error:%d", r);
	}

	local.sin_family = AF_INET;
	local.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	//local.sin_addr.S_un.S_addr = inet_addr("113.54.209.189");
	local.sin_port = htons(0x1234);

	if (bind(s, (sockaddr*)&local, sizeof(local)) != 0) {
		r = WSAGetLastError();
		printf("socket error:%d", r);
	}

	if (listen(s, 5) == 0) {
		while (1) {
			len = sizeof(remote);
			news = accept(s, (sockaddr*)&remote, &len);
			if (news == INVALID_SOCKET) {
				r = WSAGetLastError();
				printf("accept error:%d", r);
			}
			else {
				recv(news, recvbuf, sizeof(recvbuf), 0);
				printf("client:%s\n", recvbuf);
				printf("to client:%s\n",buf);
				//scanf_s("%s", &buf,128);
				if (send(news, buf, 128, 0) == SOCKET_ERROR) {
					r = WSAGetLastError();
					printf("send error:%d", r);
				}
				else {
					closesocket(news);
				}
			}
		}
	}
	else {
		r = WSAGetLastError();
		printf("listen error:%d", r);
	}

	
	closesocket(s);
	WSACleanup();
	return 0;
}

