#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS 
//inet_ntoa zbog toga ide ovaj define
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include "Structs.h"
#pragma comment(lib, "ws2_32.lib")// ovo je zbog greske LINK 2019 unresolved external symbols _imp_WSA or _imp_socket ....

#define WIN32_LEAN_AND_MEAN
#define DEFAULT_BUFLEN 524
#define DEFAULT_PORT 27017
#define SAFE_DELETE_HANDLE(a) if(a){CloseHandle(a);} 



// Initializes WinSock2 library
// Returns true if succeeded, false otherwise.
bool InitializeWindowsSockets();
DWORD WINAPI FunkcijaThread1(LPVOID param);
void Subscribe(void*topic);
int Connect();

//Globalne promenljive
HANDLE signalKraja, signalKrajaNiti;
DWORD thread1ID;
CRITICAL_SECTION criticalSectionForSTDIO;
// socket used to communicate with server
SOCKET connectSocket = INVALID_SOCKET;

//end globalne promenljive


int Connect() {
	// create a socket
	connectSocket = socket(AF_INET,
		SOCK_STREAM,
		IPPROTO_TCP);

	if (connectSocket == INVALID_SOCKET)
	{
		printf("socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	// create and initialize address structure
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddress.sin_port = htons(DEFAULT_PORT);
	// connect to server specified in serverAddress and socket connectSocket
	if (connect(connectSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
	{
		printf("Unable to connect to server.\n");
		closesocket(connectSocket);
		WSACleanup();
	}
	return 0;
}

void Subscribe(void*topic) {

	char* poruka = (char*)topic;

	int iResult = send(connectSocket, poruka, strlen(poruka), 0);

	if (iResult == SOCKET_ERROR)
	{
		printf("send failed with error: %d\n", WSAGetLastError());
	}

	printf("Bytes Sent: %ld\n", iResult);

}

bool InitializeWindowsSockets()
{
	WSADATA wsaData;
	// Initialize windows sockets library for this process
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("WSAStartup failed with error: %d\n", WSAGetLastError());
		return false;
	}
	return true;
}

DWORD WINAPI FunkcijaThread1(LPVOID param) {
	////za slucaj da moze da se prijavi na vise tema, nema veze da li je iznad ili ispod recv funkcije tek nakon prijema poruke moze da se prijavi na novu temu
	SOCKET connectSocket = *(SOCKET*)param;
	while (WaitForSingleObject(signalKrajaNiti, 500) == WAIT_TIMEOUT) {

		if (_kbhit()) {
			EnterCriticalSection(&criticalSectionForSTDIO);
			char ch = _getch();
			LeaveCriticalSection(&criticalSectionForSTDIO);
			if (ch == 'x') {

				EnterCriticalSection(&criticalSectionForSTDIO);
				char poruka[21];
				printf("Unesite topic:");
				gets_s(poruka, 20);
				Subscribe((void*)poruka);
				LeaveCriticalSection(&criticalSectionForSTDIO);
			}
			else if (ch == 'q') {
				//klijent zeli da zavrsi sa radom
				ReleaseSemaphore(signalKraja, 1, NULL);
				break;
			}

		}
	}

	closesocket(connectSocket);
	WSACleanup();
	return 0;
}

