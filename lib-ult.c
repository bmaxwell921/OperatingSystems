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

/*
* TYPEDEFS
* ---------------------------------------------------------------
*/
typedef struct Node {
	ucontext_t* context;
	int priority;
	int t; /* Used to break ties */
	
	struct Node* next;
	struct Node* prev;
} Node;
/* --------------------------------------------------------------- */

/*
* PROTOTYPES
* ---------------------------------------------------------------
*/

/* Constructor for the queue */
void initQueue();

/* Mallocs, assigns, and returns a Node with the given info */
Node* initNode(ucontext_t* context, int prio, int t);

/* Adds the given Node to the list starting with head */
void addNode(Node* n);

/* Removes the node with the lowest priority number*/
Node* removeNode();

/* Prints the Queue to the console */
void printQueue();
void printNode(Node* n);

/* Gets the current context and sets it up to run f */
int setUpContext(ucontext_t ucp, void (*f)());

/* Runs the function on an available kernal thread */
void runFunction(void (*func)());

/* Wrapper function used to match clone's first argument */
int wrapperFunc(void* func());
/* --------------------------------------------------------------- */

/*
* FIELDS
* ---------------------------------------------------------------
*/

/* Used to lock number of threads */
sem_t lock;

int MAX_THREADS;
int numThreads;


/* Dummy head and tail for the queue of nodes */
Node* head;
Node* tail;
/* Use this variable to set t for nodes */
int t;

int initProp = FALSE;
/* --------------------------------------------------------------- */

/*
* THREAD LIBRARY FUNCTIONS
* ---------------------------------------------------------------
*/

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
	/* Context we'll use for the function */
	ucontext_t newCont;
	Node* n;

	if (initProp != TRUE) {
		printf("ERROR: Threading library improperly started: system_init must be called first!");
		return FAILURE;
	}

	/* Lock our fields */
	sem_wait(&lock);

	/*
	* Go ahead and run the function if we haven't hit the cap yet.
	*/
	if (numThreads < MAX_THREADS) {
		++numThreads;
		if (DEBUGGING) {
			printf("Enough threads left...firing it up!\n");
		}
		sem_post(&lock);
		
		runFunction(func);

		return 0;
	}

	if (DEBUGGING) {
		printf("Not enough threads left, throwing it in the queue\n");
	}
	/* Otherwise we'll need to put this guy into the queue to wait until someone else is done */
	if(setUpContext(newCont, func) == FAILURE) {
		return FAILURE;
	}

	n = initNode(&newCont, pri_num, t);
	addNode(n);
	sem_post(&lock);
	return 0;
}

int setUpContext(ucontext_t ucp, void func()) {
	if(getcontext(&ucp) == FAILURE) {
		return FAILURE;
	}
	ucp.uc_stack.ss_sp = (void*) malloc(16384);
	ucp.uc_stack.ss_size = 16384;
	makecontext(&ucp, func, 0);

	return 0;
}

void runFunction(void (*func)()) {
	void* stack = (void*) malloc(16384); 
	stack += 16383;

	/*
	* clone's first parameter is
	*	int (*func)() 
	* but func comes in as 
	*	void (*func)()
	* so we need to wrap up the function call into another method. 
	* NOTE: Doing this causes a warning, but when I run it everything works fine.
	* I'd rather do this than have to rework my queue to match the many2one-mappings.c file
	*/

	clone(wrapperFunc, stack, CLONE_VM | CLONE_FILES, func);
}

int wrapperFunc(void* func()) {
	func();
	return 0;
}

int uthread_yield(int pri_num) {
	ucontext_t ucp;
	ucontext_t* runNext;
	Node* n;

	if (initProp != TRUE) {
		printf("ERROR: Threading library improperly started: system_init must be called first!");
		return FAILURE;
	}

	/* 
	* Get the current context and put it into the queue.
	* This way we never need to worry about the queue being empty.
	*/
	if (getcontext(&ucp) == FAILURE) {
		return FAILURE;
	}

	sem_wait(&lock);

	n = initNode(&ucp, pri_num, t);
	addNode(n);

	/* Get the context with the lowest priority number so we can run it */
	n = removeNode();

	/* TODO free n here to prevent a memory leak? */
	runNext = n->context;
	/* n->context = NULL;
	* free(n);
	*/


	if (DEBUGGING) {
		printf("Should start running:\n\t");
		printNode(n);
	}

	sem_post(&lock);

	/* Then run it, updating ucp so when we get back to that context we start at the return */
	if (swapcontext(&ucp, n->context) == FAILURE) {
		return FAILURE;
	}

	/* A successful run of swapcontext shouldn't return until runNext's process completes*/
	return 0;
}

void uthread_exit() {
	Node* n;

	sem_wait(&lock);
	/* Get the next thing to run */
	n = removeNode();

	/* Make sure to update numThreads */
	--numThreads;

	/* Nothing more to run */
	if (n == NULL) {
		return;
	}

	sem_post(&lock);

	setcontext(n->context);
}
/* --------------------------------------------------------------- */



/* 
* QUEUE METHODS
* ---------------------------------------------------------------
*/
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
	ret->t = t++;
	ret->next = NULL;
	ret->prev = NULL;
	return ret;
}

void addNode(Node* n) {
	/* We always add to the front, remove will have to find the correct Node */
	n->next = head->next;
	n->prev = head;

	head->next->prev = n;
	head->next = n;
}

Node* removeNode() {
	Node* min;
	Node* cur;
	/* Find the node with the lowest priority number. 
	*  Break ties by taking smaller t
	*/
	if (head->next == tail) {
		/* Queue is empty */
		return NULL;
	}

	/*
	* The node we want to remove is the node with the lowest priority
	* and lowest t
	*/

	min = head->next;
	cur = head->next;

	/* Find the node to remove */
	while (cur != tail) {
		if (cur->priority < min->priority
				|| cur->priority == min->priority && cur->t < min->t) {
			min = cur;
		}
		cur = cur->next;
	}

	/* Actually remove it from the queue */
	min->prev->next = min->next;
	min->next->prev = min->prev;

	/* Set these to NULL so when we delete the Node it doesn't do crazy stuff */
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
	printf("Node with priority:%d and t:%d\n", n->priority, n->t);
}
/* --------------------------------------------------------------- */
