//ServerOICQ
#include "stdio.h"
#include <conio.h>
#include "winsock.h"
#pragma comment (lib,"wsock32.lib")
DWORD WINAPI server_proc(LPVOID lpParam);
DWORD WINAPI server_proc_test(LPVOID lpParam);
/*
#include <thread>
#include <iostream>
#include <winsock.h>
#pragma comment(lib,"wsock32.lib")
*/

//登陆
int login() {
	return 0;
}

//连接
int conn() {
	return 0;
}

//通信
int comm() {
	return 0;
}


int main(int argc, char* argv[])
{
	SOCKET s, newsock;
	struct sockaddr_in ser_addr, remote_addr;
	int len;
	WSAData wsa;
	DWORD dwThreadId, dwThrdParam;
	HANDLE hThread;

	WSAStartup(0x101, &wsa);
	s = socket(AF_INET, SOCK_STREAM, 0);
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	ser_addr.sin_port = htons(0x1234);
	bind(s, (sockaddr*)&ser_addr, sizeof(ser_addr));

	listen(s, 5);
	while (1) {
		len = sizeof(remote_addr);
		newsock = accept(s, (sockaddr*)&remote_addr, &len);
		if (newsock == -1) {
			break;
		}
		printf("accept a connection\n");
		dwThrdParam = (DWORD)newsock;
		hThread = CreateThread(
			NULL,                        // no security attributes 
			0,                           // use default stack size  
			server_proc_test,                  // thread function 
			&dwThrdParam,                // argument to thread function 
			0,                           // use default creation flags 
			&dwThreadId);                // returns the thread identifier 
//server_proc(&dwThrdParam); 
	}
	closesocket(s);
	WSACleanup();
	return 0;
}
DWORD WINAPI server_proc(LPVOID lpParam)
{
	SOCKET sock;
	char buf[129];
	int retval;
	sock = *(SOCKET*)lpParam;
	while (1) {
		memset(buf, 0, sizeof(buf));
		recv(sock, buf, sizeof(buf), 0);
		send(sock, buf, strlen(buf), 0);
	}
	return 0;
}
DWORD WINAPI server_proc_test(LPVOID lpParam)
{
	/*
	SOCKET sock;
	int i;

	sock = *(SOCKET*)lpParam;
	while (1) {
		printf("%x  ", sock);
		for (i = 0; i < 2000000; i++);//you can test how the system schedule between threads
		Sleep(2000);
	}
	printf("close a socket\n");
	closesocket(sock);
	return 0;
	*/
	SOCKET sock;
	int i;
	sock = *(SOCKET*)lpParam;
	getch();
	printf("%x\n",sock);
	return 0;
}