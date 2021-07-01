#pragma once
#include <WinSock2.h>
#define max_topic 21

//Lista
typedef struct uticnica {
	SOCKET acceptedSocket;
	struct uticnica *next;
}UTICNICA;
//End Lista

//Queue
typedef struct artikal {
	char topic[max_topic];
	char text[491];
} ARTICLE;

typedef struct queue {
	struct node *front;
	struct node *back;
} QUEUE;

typedef struct node {
	struct artikal data;
	struct node *next;
} NODE;
// End queue

//HashTable 
typedef struct subscribers {

	char topic[max_topic];
	uticnica * acceptedSocketsForTopic;
	struct subscribers *next;
}subscribers;
//End HashTable
