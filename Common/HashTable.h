#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <WinSock2.h>
#include "List.h"
#define _CRT_SECURE_NO_WARNINGS
//#define max 20
#define table_size 10


unsigned int HashFunction(const char * topic) {

	int length = strlen(topic);
	unsigned int hash_value = 0;
	for (int i = 0; i < length; i++) {

		hash_value += topic[i];
		hash_value = (hash_value *topic[i]) % table_size;
	}

	return hash_value;

}

bool AddToTable(subscribers **tabela, subscribers *s) {
	if (s == NULL) return false;
	int index = HashFunction(s->topic);
	//ovde negde malloc odraditi

	s->next = tabela[index];
	tabela[index] = s;

	return true;
}

subscribers * CreateSubscriber(const char *topic) {

	subscribers * novi = (subscribers *)malloc(sizeof(subscribers));
	if (novi == NULL) return NULL;

	strcpy_s(novi->topic, topic);
	novi->acceptedSocketsForTopic = NULL;
	return novi;
}

//bool AddToTable1(char *topic) {
//	int index = HashFunction(topic);
//	subscribers * temp = (subscribers*)malloc(sizeof(subscribers));
//	if (temp == NULL) return false;
//
//	temp->next = tabela[index];
//	temp->acceptedSocketsForTopic = NULL;
//	tabela[index] = temp;
//	return true;
//}

void printTable(subscribers**tabela) {
	printf("**** POCETAK ISPISA TABELE *****\n");
	for (int i = 0; i < table_size; i++) {
		if (tabela[i] == NULL) {
			printf("%i \t NULL\n", i);
		}
		else {
			printf("%i\t", i);
			subscribers * temp = tabela[i];
			while (temp != NULL)
			{
				printf("%s -> [ ", temp->topic);
				uticnica * aca = temp->acceptedSocketsForTopic;
				while (aca != NULL) {
					printf(" %d - ", aca->acceptedSocket);
					aca = aca->next;
				}
				printf(" ] --");
				temp = temp->next;
			}
			printf("\n");
		}
	}
	printf("**** KRAJ ISPISA TABELE *****\n");
}


void DeleteSubscriberFromListOfSubscribers(subscribers**tabela, SOCKET socket) {
	//Sub moze da se prijavi na vise tema pa ga treba obrisati iz svih lista a ne samo iz jedne, zato je void a ne bool 
	for (int i = 0; i < table_size; i++) {
		if (tabela[i] == NULL) continue;
		else {
			subscribers *temp = tabela[i];
			while (temp != NULL)
			{
				//u if ide remove umesto find da ne bi dzaba 2 puta prolazili kroz istu listu (optimalnost), Remove svakako prolazi kroz listu ako postoji socket -> obrisace ga
				if (Remove(&temp->acceptedSocketsForTopic, socket)/*FindInList(&temp->acceptedSocketsForTopic, socket)*/) {
					printf("Nasao ga i brisem iz liste...\n");
					//Remove(&temp->acceptedSocketsForTopic, socket);
				}
				temp = temp->next;
			}
		}
	}


}



void initTable(subscribers**tabela) {
	for (int i = 0; i < table_size; i++) {
		tabela[i] = NULL;

	}
}

subscribers * FindSubscriberInTable(subscribers**tabela, const char * topic) {
	int index = HashFunction(topic);
	subscribers *temp = tabela[index];
	while (temp != NULL && strcmp(temp->topic, topic) != 0) {
		temp = temp->next;
	}
	return temp;
}

bool DeleteFromTable(subscribers**tabela, char *topic) {
	int index = HashFunction(topic);
	subscribers *temp = tabela[index];
	subscribers *prev = NULL;
	while (temp != NULL && strcmp(temp->topic, topic) != 0) {
		prev = temp;
		temp = temp->next;
	}
	if (temp == NULL) {
		//return NULL;
		return false;
	}
	if (prev == NULL) {
		//brisanje pocetnog covra
		tabela[index] = temp->next;
	}
	else {
		prev->next = temp->next;
	}

	deleteList(&temp->acceptedSocketsForTopic); //ovo ce osloboditi memoriju za svaki socket u listi
	//ovde odraditi free(temp) i vratiti true;
	free(temp);
	temp = NULL;
	return true;
	//return temp;
}

void DeleteAllTable(subscribers**tabela) {
	for (int i = 0; i < table_size; i++) {
		if (tabela[i] == NULL) continue;
		else {
			subscribers *temp = tabela[i];
			while (tabela[i] != NULL) {
				DeleteFromTable(tabela, tabela[i]->topic);
			}
		}
	}
}