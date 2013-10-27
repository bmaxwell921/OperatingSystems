#include "lib-ult.h"
#include <stdio.h>
#include <stdlib.h>

void runFirst();
void runSecond();
void runThird();

int main(int argc, char** argv) {
	system_init(2);
	uthread_create(runFirst, 1);
}

void runFirst() {
	printf("Running in runFirst right now\n");
	uthread_create(runSecond, 2);
	/* printf("result of yield:%d\n", uthread_yield(3)); */
	uthread_create(runThird, 3);
	printf("Finishing runFirst\n");

	uthread_exit();
}

void runSecond() {
	printf("Running in runSecond now.\n");

	printf("Finishing runSecond\n");
	uthread_exit();
}

void runThird() {
	printf("Running in runThird now.\n");
	printf("IT FINALLY WORKED!");

	printf("Finishing runThird\n");
	uthread_exit();
}