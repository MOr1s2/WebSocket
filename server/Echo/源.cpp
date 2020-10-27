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
	if (list->count < maxCount-1) {
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
	char c;
	char buf[128];
	char recvbuf[128];
	memset(buf, 0, sizeof(buf));
	memset(recvbuf, 0, sizeof(recvbuf));
	SOCKET s, news;
	WSAData wsa;
	struct sockaddr_in local,remote;
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

	local.sin_family = AF_INET;
	local.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	local.sin_port = htons(0x1234);

	if (bind(s, (sockaddr*)&local, sizeof(local)) != 0) {
		r = WSAGetLastError();
		printf("bind error: %d\n", r);
	}

	if (listen(s, 10) == 0) {
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
				len = sizeof(remote);
				news = accept(sockets.mainSocket, (sockaddr*)&remote, &len);
				if (news == SOCKET_ERROR) {
					r = WSAGetLastError();
					printf("accept error: %d\n", r);
					continue;
				} else {
					insertSocket(news, &sockets);
				}
			}
			
			for (int i = 0; i < maxCount; i++) {
				if (sockets.sockets[i] == 0) {
					continue;
				}
				news = sockets.sockets[i];
				if (FD_ISSET(news, &readfds)) {
					retval = recv(news, recvbuf, 128, 0);
					if (retval == -1) {
						r = WSAGetLastError();
						printf("recv error: %d\n", r);
						closesocket(news);
						deleteSocket(news, &sockets);
						continue;
					}
					printf("client: %s\n", recvbuf);
				}
				
				if (FD_ISSET(news, &writefds)) {
					if (_kbhit()) {
						printf("to client: ");
						/*
						c = getche();
						if (c == '\n' || c == '\r') {
							printf("to client: %s\n", buf);
							send(news, buf, 128, 0);
						} else if (c == '\b'&& strlen(buf) != 0) {
							buf[strlen(buf) - 1] = 0;
						} else {
							buf[strlen(buf)] = c;
						}
						*/
						gets_s(buf);
						send(news, buf, 128, 0);
					}

				}

			}
			FD_ZERO(&readfds);
			FD_ZERO(&writefds);
			FD_ZERO(&exceptfds);
		}
		closesocket(sockets.mainSocket);
	}
	else {
		r = WSAGetLastError();
		printf("listen error: %d\n", r);
	}

	WSACleanup();
	return 0;
}

