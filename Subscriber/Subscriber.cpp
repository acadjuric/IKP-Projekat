#include "../Common/Subscriber.h"


int __cdecl main(int argc, char **argv)
{
	// variable used to store function return value
	int iResult;
	// Buffer used for storing incoming data
	char recvbuf[DEFAULT_BUFLEN];

	if (InitializeWindowsSockets() == false)
	{
		// we won't log anything since it will be logged
		// by InitializeWindowsSockets() function
		return 1;
	}

	//konekcija na server nije uspela
	if (Connect()) 
		return 1;
	

	char poruka[21];
	printf("Unesite topic:");
	gets_s(poruka, 20);

	Subscribe((void*)poruka);

	ARTICLE *vest;
	InitializeCriticalSection(&criticalSectionForSTDIO);
	HANDLE t1;
	t1 = CreateThread(NULL, 0, &FunkcijaThread1, &connectSocket, 0, &thread1ID);
	signalKrajaNiti = CreateSemaphore(0, 0, 1, NULL);
	signalKraja = CreateSemaphore(0, 0, 1, NULL);
	
	int brojac = 1;
	do{

		// Receive data until the client shuts down the connection
		iResult = recv(connectSocket, recvbuf, (int)(sizeof(ARTICLE)), 0);
		if (iResult > 0)
		{
			//printf("Message received from client: %s.\n", recvbuf);

			vest = (ARTICLE*)recvbuf;

			EnterCriticalSection(&criticalSectionForSTDIO);
			printf("PORUKA BROJ: %d\n", brojac++);
			printf("Naslov: %s\n", vest->topic);
			printf("Tekst: %s \n", vest->text);
			LeaveCriticalSection(&criticalSectionForSTDIO);
		}
		else if (iResult == 0)
		{
			// connection was closed gracefully
			EnterCriticalSection(&criticalSectionForSTDIO);
			printf("Connection with PubSubEngine closed.\n");
			LeaveCriticalSection(&criticalSectionForSTDIO);
			closesocket(connectSocket);
			ReleaseSemaphore(signalKrajaNiti, 1, NULL);
			break;
		}
		else
		{
			// there was an error during recv
			//printf("recv failed with error: %d\n", WSAGetLastError());
			EnterCriticalSection(&criticalSectionForSTDIO);
			printf("PubSunEngine finished. Closing socket....");
			LeaveCriticalSection(&criticalSectionForSTDIO);
			closesocket(connectSocket);
			ReleaseSemaphore(signalKrajaNiti, 1, NULL);
			break;
		}
	} while (WaitForSingleObject(signalKraja, 200) == WAIT_TIMEOUT);

	if (t1) {
		WaitForSingleObject(t1, INFINITE);
	}
	
	SAFE_DELETE_HANDLE(t1);
	SAFE_DELETE_HANDLE(signalKraja);
	SAFE_DELETE_HANDLE(signalKrajaNiti);

	DeleteCriticalSection(&criticalSectionForSTDIO);
	// cleanup
	closesocket(connectSocket);
	WSACleanup();

	return 0;
}
