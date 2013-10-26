#define _GNU_SOURCE
#include "lib-ult.h"
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <semaphore.h>
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
} Node;
// ---------------------------------------------------------------

// PROTOTYPES
// ---------------------------------------------------------------
// Mallocs, assigns, and returns a Node with the given info
Node* initNode(ContextPtr context, int prio, int t, void (*f)());

// Adds the given Node to the list starting with head
void addNode(Node* n);

// Removes the node with the 
Node* removeNode();
// ---------------------------------------------------------------

// FIELDS
// ---------------------------------------------------------------
// Used to lock number of threads
sem_t lock;

int MAX_THREADS;
int numThreads;


// Dummy head for the queue of nodes
Node* head;
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

	// Init Queue
	head = initNode(NULL, 0, 0, NULL);
	t = 0;

	initProp = TRUE;
}

// ---------------------------------------------------------------



// QUEUE METHODS
// ---------------------------------------------------------------
Node* initNode(ContextPtr context, int prio, int t, void (*f)()) {
	Node* ret = (Node*) malloc(sizeof(Node));
	ret->context = context;
	ret->priority = prio;
	ret->t = t;
	ret->toRun = f;
	ret->next = NULL;
}

void addNode(Node* n) {
	// Queue itself is out of order, remove method will have
	// to find the right place
	n->next = head->next;
	head->next = n;
}

Node* removeNode() {
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

	Node* min = head->next;
	Node* cur = head->next;
	while (cur) {
		if (cur->priority < min->priority
			|| cur->priority == min->priority && cur->t < min->t) {
			min = cur;
		}
		cur = cur->next;
	}

}

// ---------------------------------------------------------------
