#pragma once

#include<stdio.h>
#include<stdlib.h>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h> //zbog SD_SEND u funkciji zatvori sve sockete
#include "../Common/Structs.h"



bool Add(uticnica **head, SOCKET data) {
	uticnica * new_node;
	new_node = (uticnica *)malloc(sizeof(uticnica));
	if (new_node == NULL) {
		return false;
	}

	new_node->acceptedSocket = data;
	new_node->next = *head;
	*head = new_node;
	return true;
}

bool Remove(uticnica **head, SOCKET sock) {
	uticnica * current = *head;
	uticnica * previous = NULL;

	if (current == NULL) {
		return false;
	}

	while (current->acceptedSocket != sock) {

		//if it is last node
		if (current->next == NULL) {
			return false;
		}
		else {
			//store reference to current link
			previous = current;
			//move to next link
			current = current->next;
		}
	}

	if (current == *head) {
		//change first to point to next link
		*head = (*head)->next;
	}
	else {
		//bypass the current link
		previous->next = current->next;
	}

	free(current);
	current = NULL;
	return true;
}

void deleteList(uticnica **head) {
	uticnica *temp = NULL;
	uticnica *current = *head;

	while (current != NULL) {
		temp = current;
		current = current->next;
		free(temp);
		temp = NULL;
	}
	*head = current;
}
bool FindInList(uticnica **head, SOCKET s) {

	uticnica *temp = NULL;
	uticnica *current = *head;
	while (current != NULL) {
		if (current->acceptedSocket == s)
			return true;
		current = current->next;
	}

	return false;
}
void print_list(uticnica * head) {
	if (head == NULL) {
		printf("List is empty!");
		return;
	}

	uticnica * current = head;

	while (current != NULL) {
		printf("%d\n", current->acceptedSocket);
		current = current->next;
	}

	printf("\n");
}

void ZatvoriSveSocketeZaListu(uticnica * lista) {
	//uticnica * trenutni = lista; // trenutni je head tj. pocetak liste
	int iResult;
	//Ugasi svaki socket;
	while (lista != NULL) {
		iResult = shutdown(lista->acceptedSocket, SD_SEND);
		if (iResult == SOCKET_ERROR)
		{
			printf("shutdown ZATVORI SVE SOCKETE FUNKCIJA  failed with error: %d\n", WSAGetLastError());
			closesocket(lista->acceptedSocket);
			WSACleanup();
			//return 1;
			//continue;
		}
		//zatvori svaki socket publisheras ili subscribera
		closesocket(lista->acceptedSocket);
		lista = lista->next;
	}
}
