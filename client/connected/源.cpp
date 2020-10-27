//client_connected

#include "stdio.h" 
#include <time.h>
//#include <Windows.h>
#include <winsock.h>
#pragma comment(lib,"wsock32.lib")

int main() {

	int r,i=0;
	LARGE_INTEGER start, end, fequen;

	const char buf[128] = "I am a client";
	//char buf[128];
	char recvbuf[128];
	SOCKET s;
	WSAData wsa;
	sockaddr_in remote;

	WSAStartup(0x101, &wsa);

	while (i<30) {
		//memset(buf, 0, sizeof(buf));
		memset(recvbuf, 0, sizeof(recvbuf));

		s = socket(AF_INET, SOCK_STREAM, 0);

		remote.sin_family = AF_INET;
		remote.sin_addr.S_un.S_addr = inet_addr("192.168.1.107");
		remote.sin_port = htons(0x1234);
		if (connect(s, (sockaddr*)&remote, sizeof(remote)) == 0) {
			QueryPerformanceFrequency(&fequen); // 每秒跳动次数
			QueryPerformanceCounter(&start); // 统计跳动次数
			if (send(s, buf, 128, 0) == SOCKET_ERROR) {
				r = WSAGetLastError();
				printf("send error: %d\n", r);
			}
			else {
				recv(s, recvbuf, sizeof(recvbuf), 0);
				recvbuf[sizeof(recvbuf) - 1] = 0;
				QueryPerformanceCounter(&end); // 结束时一共跳动的次数
				printf("%f\n", ((double)end.QuadPart - (double)start.QuadPart) * 1000000 / ((double)fequen.QuadPart));
			}
			i++;
		}
		else {
			r = WSAGetLastError();
			printf("connect error: %d\n", r);
		}
		closesocket(s);
	}	
	WSACleanup();
	return 0;
}

