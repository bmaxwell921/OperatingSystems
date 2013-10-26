#define _GNU_SOURCE
#include "lib-ult.h"
//#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
//#include <ucontext.h>
//#include <semaphore.h>
#define TRUE 1
#define FALSE 0

// TYPEDEFS
// ---------------------------------------------------------------
typedef struct ucontext* ContextPtr;

typedef struct Node {
	ContextPtr context;
	int priority;
	int t; // Used to break ties
	void (*toRun)();

	struct Node* next;
	struct Node* prev;
} Node;
// ---------------------------------------------------------------

// PROTOTYPES
// ---------------------------------------------------------------
// Constructor for the queue
void initQueue();
// Mallocs, assigns, and returns a Node with the given info
Node* initNode(ContextPtr context, int prio, int t, void (*f)());

// Adds the given Node to the list starting with head
void addNode(Node* n);

// Removes the node with the 
Node* removeNode();

// Prints the Queue to the console
void printQueue();
void printNode(Node* n);
// ---------------------------------------------------------------

// FIELDS
// ---------------------------------------------------------------
// Used to lock number of threads
//sem_t lock;

int MAX_THREADS;
int numThreads;


// Dummy head and tail for the queue of nodes
Node* head;
Node* tail;
// Use this variable to set t for nodes
int t;

int initProp = FALSE;
// ---------------------------------------------------------------

// THREAD LIBRARY FUNCTIONS
// ---------------------------------------------------------------
void system_init(int max_number_of_klt) {
	if (max_number_of_klt < 1) {
		printf("ERROR: max_number_of_klt was out of bounds: %d", 
			max_number_of_klt);
		return;
	}
	MAX_THREADS = max_number_of_klt;
	numThreads = 0;

	initQueue();	

	initProp = TRUE;
}

// ---------------------------------------------------------------



// QUEUE METHODS
// ---------------------------------------------------------------
void initQueue() {
	head = initNode(NULL, 0, 0, NULL);
	tail = initNode(NULL, 0, 0, NULL);
	head->next = tail;
	tail->prev = head;

	t = 0;
}

Node* initNode(ContextPtr context, int prio, int t, void (*f)()) {
	Node* ret = (Node*) malloc(sizeof(Node));
	ret->context = context;
	ret->priority = prio;
	ret->t = t;
	ret->toRun = f;
	ret->next = NULL;
	ret->prev = NULL;
	return ret;
}

void addNode(Node* n) {
	// We always add to the front, remove will have to find the correct Node
	n->next = head->next;
	n->prev = head;

	head->next->prev = n;
	head->next = n;
}

Node* removeNode() {
	Node* min;
	Node* cur;
	// Find the node with the lowest priority number. 
	// Break ties by taking smaller t
	if (head->next == NULL) {
		// Queue is empty
		return NULL;
	}

	/*
	* The node we want to remove is the node with the lowest priority
	* and lowest t
	*/

	min = head->next;
	cur = head->next;

	// Find the node to remove
	while (cur != tail) {
		if (cur->priority < min->priority
				|| cur->priority == min->priority && cur->t < min->t) {
			min = cur;
		}
		cur = cur->next;
	}

	// Actually remove it from the queue
	min->prev->next = min->next;
	min->next->prev = min->prev;

	// Set these to NULL so when we delete the Node it doesn't do crazy stuff
	min->prev = NULL;
	min->next = NULL;

	return min;
}

void printQueue() {
	Node* cur = head;
	while (cur) {
		printNode(cur);
		cur = cur->next;
	}
}

void printNode(Node* n) {
	printf("Node with t:%d\n", n->t);
}

// ---------------------------------------------------------------
//int main(int argc, char** argv) {
//	Node* n;
//	int i;
//	initQueue();
//	t = 1;
//	
//	for (i = 0; i < 2; ++i) {
//		n = initNode(NULL, 0, t++, NULL);
//		addNode(n);
//		printQueue();
//		printf("\n");
//	}
//
//	n = removeNode();
//
//	free(n);
//
//	printf("");
//}