
/*
* A library for scheduling user threads on kernal threads. This library uses priority scheduling
* based first on the priority of a given thread and then on the order the threads arrived.
* @author Brandon Maxwell
*/
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

int DEBUGGING = 0;

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
Node* initNode(int prio);

/* Adds the given Node to the list starting with head */
void addNode(Node* n);

/* Removes the node with the lowest priority number*/
Node* removeNode();

/* Prints the Queue to the console */
void printQueue();
void printNode(Node* n);

/* Runs the function on an available kernal thread */
int runKernalThread(void* arg);

/* --------------------------------------------------------------- */

/*
* FIELDS
* ---------------------------------------------------------------
*/
int STACK_SIZE = 16384;

/* Used to lock the queue */
sem_t queueLock;

/* Used to lock number of threads */
sem_t threadsLock;

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
		printf("ERROR: max_number_of_klt was out of bounds: %d\n", 
			max_number_of_klt);
		exit(EXIT_FAILURE);
	}

	if (initProp == TRUE) {
		printf("ERROR: uthread library has already been initialized.");
		exit(EXIT_FAILURE);
	}
	initProp = TRUE;

	sem_init(&queueLock, 0, 1);
	sem_init(&threadsLock, 0 , 1);

	MAX_THREADS = max_number_of_klt;
	numThreads = 0;
	initQueue();	
}

int uthread_create(void func(), int pri_num) {
	Node* n;

	if (initProp != TRUE) {
		printf("ERROR: Threading library improperly started: system_init must be called first!\n");
		return FAILURE;
	}

	/* Add this new function into the queue*/
	n = initNode(pri_num);
	makecontext(n->context, func, 0);
	
	/* Lock our fields */
	sem_wait(&queueLock);
	addNode(n);
	sem_wait(&threadsLock);

	/*
	* Go ahead and run the function if we haven't hit the cap yet.
	*/
	if (numThreads < MAX_THREADS) {
		void* stack;
		++numThreads;
		sem_post(&queueLock);
		
		stack = malloc(STACK_SIZE); 
		stack += STACK_SIZE - 1;
		clone(runKernalThread, stack, CLONE_VM | CLONE_FILES, NULL);
		sem_post(&threadsLock);
		return 0;
	}

	sem_post(&queueLock);
	sem_post(&threadsLock);
	return 0;
}

int runKernalThread(void* arg) {
	Node* run;
	
	sem_wait(&queueLock);
	run = removeNode();
	sem_post(&queueLock);

	if (run == NULL) {
		sem_wait(&threadsLock);
		--numThreads;
		sem_post(&threadsLock);
		exit(EXIT_SUCCESS);
	}

	setcontext(run->context);
}

int uthread_yield(int pri_num) {
	Node* n;
	Node* nextNode;

	if (initProp != TRUE) {
		printf("ERROR: Threading library improperly started: system_init must be called first!");
		return FAILURE;
	}

	/* 
	* Get the current context and put it into the queue.
	* This way we never need to worry about the queue being empty.
	*/
	n = initNode(pri_num);

	sem_wait(&queueLock);
	addNode(n);

	/* Get the context with the lowest priority number so we can run it */
	nextNode = removeNode();

	sem_post(&queueLock);

	/* Then run it, updating ucp so when we get back to that context we start at the return */
	return swapcontext(n->context, nextNode->context);
}

void uthread_exit() {
	Node* n;

	if (initProp != TRUE) {
		printf("ERROR: Threading library improperly started: system_init must be called first!");
		exit(EXIT_FAILURE);
	}

	sem_wait(&queueLock);
	/* Get the next thing to run */
	n = removeNode();
	sem_post(&queueLock);

	sem_wait(&threadsLock);
	/* Make sure to update numThreads */
	--numThreads;
	sem_post(&threadsLock);

	/* Nothing more to run */
	if (n == NULL) {
		exit(EXIT_SUCCESS);
	}

	setcontext(n->context);
}
/* --------------------------------------------------------------- */



/* 
* QUEUE METHODS
* ---------------------------------------------------------------
*/
void initQueue() {
	ucontext_t empty;
	head = initNode(0);
	tail = initNode(0);
	head->next = tail;
	tail->prev = head;

	t = 0;
}

Node* initNode(int prio) {
	Node* ret = (Node*) malloc(sizeof(Node));

	ret->context = (ucontext_t*) malloc(sizeof(ucontext_t));
	getcontext(ret->context);
	ret->context->uc_stack.ss_sp = malloc(STACK_SIZE);
	ret->context->uc_stack.ss_size = STACK_SIZE;
	
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
