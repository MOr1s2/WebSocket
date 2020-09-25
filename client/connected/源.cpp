//client_connected
#include "stdio.h" 
#include "time.h"
#include <winsock.h>
#pragma comment(lib,"wsock32.lib")

int main() {

	int r,i=0;
	int time[30];
	clock_t start, end;
	//const char buf[128] = "I am a client";
	char buf[128];
	char recvbuf[128];
	memset(buf, 0, sizeof(buf));
	memset(recvbuf, 0, sizeof(recvbuf));
	SOCKET s;
	WSAData wsa;
	sockaddr_in local;
	sockaddr_in remote;

	WSAStartup(0x101, &wsa);
	while (i < 30) {
		s = socket(AF_INET, SOCK_STREAM, 0);
		local.sin_family = AF_INET;
		local.sin_addr.S_un.S_addr = inet_addr("113.54.209.189");
		//local.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
		local.sin_port = htons(0x3412);

		if (bind(s, (sockaddr*)&local, sizeof(local)) != 0) {
			r = WSAGetLastError();
			printf("bind error:%d", r);
		}

		remote.sin_family = AF_INET;
		//remote.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
		remote.sin_addr.S_un.S_addr = inet_addr("192.168.1.109");
		remote.sin_port = htons(0x1234);
		if (connect(s, (sockaddr*)&remote, sizeof(remote)) == 0) {

			printf("to server:%s\n ", buf);
			scanf_s("%s", &buf, 128);
			start = clock();
			if (send(s, buf, 128, 0) == SOCKET_ERROR) {
				r = WSAGetLastError();
				printf("send error:%d", r);
			}
			else {
				recv(s, recvbuf, sizeof(recvbuf), 0);
				printf("server:%s\n", recvbuf);
				end = clock();
				time[i] = end - start;
				printf("%d ms\n", time[i]);
			}
			i++;
		}
		else {
			r = WSAGetLastError();
			printf("connect error:%d", r);
		}
		closesocket(s);
	}	
	WSACleanup();
	return 0;
}

