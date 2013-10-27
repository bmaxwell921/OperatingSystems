#define _GNU_SOURCE
#include "lib-ult.h"
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <semaphore.h>
#define TRUE 1
#define FALSE 0
#define FAILURE -1

int DEBUGGING = 1;

// TYPEDEFS
// ---------------------------------------------------------------
typedef struct Node {
	ucontext_t* context;
	int priority;
	int t; // Used to break ties
	
	struct Node* next;
	struct Node* prev;
} Node;
// ---------------------------------------------------------------

// PROTOTYPES
// ---------------------------------------------------------------
// Constructor for the queue
void initQueue();
// Mallocs, assigns, and returns a Node with the given info
Node* initNode(ucontext_t* context, int prio, int t);

// Adds the given Node to the list starting with head
void addNode(Node* n);

// Removes the node with the 
Node* removeNode();

// Prints the Queue to the console
void printQueue();
void printNode(Node* n);

int setUpContext(ucontext_t ucp, void (*f)());
// ---------------------------------------------------------------

// FIELDS
// ---------------------------------------------------------------
// Used to lock number of threads
sem_t lock;

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

	sem_init(&lock, 0, 1);

	MAX_THREADS = max_number_of_klt;
	numThreads = 0;

	initQueue();	

	initProp = TRUE;
}

int uthread_create(void func(), int pri_num) {
	// Context we'll use for the function
	ucontext_t newCont;
	if (initProp != TRUE) {
		printf("ERROR: Threading library improperly started: system_init must be called first!");
		return FAILURE;
	}

	if(setUpContext(newCont, func) == FAILURE) {
		return FAILURE;
	}

	// lock the number of threads
	sem_wait(&lock);
	++numThreads;

	/*
	* Go ahead and run the function if we haven't hit the cap yet.
	* Less than or equal here since we've already incremented the count for the thread that would be created
	*/
	if (numThreads <= MAX_THREADS) {
		if (DEBUGGING) {
			printf("Enough threads left...firing it up!");
		}
		sem_post(&lock);
		
		setcontext(&newCont);

		// setcontext doesn't return on success
		return FAILURE;
	}

	// Otherwise we'll need to put this guy into the queue to wait until someone else is done
	Node* n = initNode(&newCont, pri_num, t++);
	addNode(n);
	sem_post(&lock);
}

int setUpContext(ucontext_t ucp, void func()) {
	if(getcontext(&ucp) == FAILURE) {
		return FAILURE;
	}
	ucp.uc_link = NULL;
	ucp.uc_stack.ss_sp = (void*) malloc(16384);
	ucp.uc_stack.ss_size = 16384;
	ucp.uc_stack.ss_flags = 0;
	makecontext(&ucp, func, 0);

	return 0;
}

int uthread_yield(int pri_num) {
	if (initProp != TRUE) {
		printf("ERROR: Threading library improperly started: system_init must be called first!");
		return FAILURE;
	}

	// TODO

}
// ---------------------------------------------------------------



// QUEUE METHODS
// ---------------------------------------------------------------
void initQueue() {
	head = initNode(NULL, 0, 0);
	tail = initNode(NULL, 0, 0);
	head->next = tail;
	tail->prev = head;

	t = 0;
}

Node* initNode(ucontext_t* context, int prio, int t) {
	Node* ret = (Node*) malloc(sizeof(Node));
	ret->context = context;
	ret->priority = prio;
	ret->t = t;
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
	if (head->next == tail) {
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
