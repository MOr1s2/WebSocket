//Echo

#include "stdio.h"
#include <conio.h>
#include <winsock.h>
#pragma comment(lib,"wsock32.lib")
#define maxCount 64

struct socketList {
	SOCKET mainSocket;
	int count;
	SOCKET sockets[maxCount];
};

void initList(socketList* list) {
	list->mainSocket = 0;
	list->count = 0;
	for (int i = 0; i < maxCount; i++) {
		list->sockets[i] = 0;
	}
}

void insertSocket(SOCKET s, socketList* list) {
	if (list->count < maxCount - 1) {
		for (int i = 0; i < maxCount; i++) {
			if (list->sockets[i] == 0) {
				list->sockets[i] = s;
				list->count += 1;
				break;
			}
		}
	}

}

void deleteSocket(SOCKET s, socketList* list) {
	for (int i = 0; i < maxCount; i++) {
		if (list->sockets[i] == s) {
			list->sockets[i] = 0;
			list->count -= 1;
			break;
		}
	}
}

void makefdList(socketList* list, fd_set* fdList) {
	FD_SET(list->mainSocket, fdList);
	for (int i = 0; i < maxCount; i++) {
		if (list->sockets[i] > 0) { //10038
			FD_SET(list->sockets[i], fdList);
		}
	}
}


int main() {

	int len, r, retval;
	char buf[128];
	char recvbuf[128];
	memset(buf, 0, sizeof(buf));
	memset(recvbuf, 0, sizeof(recvbuf));
	SOCKET s;
	WSAData wsa;
	struct sockaddr_in remote;
	struct socketList sockets;
	fd_set readfds, writefds, exceptfds;
	timeval timeout;
	unsigned long arg = 1;

	WSAStartup(0x101, &wsa);

	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == SOCKET_ERROR) {
		r = WSAGetLastError();
		printf("socket error: %d\n", r);
	}

	remote.sin_family = AF_INET;
	remote.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	remote.sin_port = htons(0x1234);

	if (connect(s, (sockaddr*)&remote, sizeof(remote)) == 0) {
		timeout.tv_sec = 0;
		timeout.tv_usec = 1;
		initList(&sockets);
		FD_ZERO(&readfds);
		FD_ZERO(&writefds);
		FD_ZERO(&exceptfds);
		sockets.mainSocket = s;
		arg = 1;
		ioctlsocket(sockets.mainSocket, FIONBIO, &arg);

		while (1) {
			makefdList(&sockets, &readfds);
			makefdList(&sockets, &writefds);
			makefdList(&sockets, &exceptfds);
			retval = select(0, &readfds, &writefds, &exceptfds, &timeout);
			if (retval == SOCKET_ERROR) {
				r = WSAGetLastError();
				printf("select error: %d\n", r);
				break;
			}

			if (FD_ISSET(sockets.mainSocket, &readfds)) {
				retval = recv(s, recvbuf, 128, 0);
				if (retval == -1) {
					r = WSAGetLastError();
					printf("recv error: %d\n", r);
					closesocket(s);
					deleteSocket(s, &sockets);
					continue;
				}
				printf("server: %s\n", recvbuf);	
			}

			if (FD_ISSET(sockets.mainSocket, &writefds)) {
				if (_kbhit()) {
					printf("to server: ");
					gets_s(buf);
					send(s, buf, 128, 0);
				}
			}
		FD_ZERO(&readfds);
		FD_ZERO(&writefds);
		FD_ZERO(&exceptfds);
		}
		closesocket(sockets.mainSocket);
	} else {
		r = WSAGetLastError();
		printf("connect error: %d\n", r);
	}

	WSACleanup();
	return 0;
}
