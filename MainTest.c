#include "lib-ult.h"
#include <stdio.h>
#include <stdlib.h>

void runFirst();
void runSecond();

int main(int argc, char** argv) {
	system_init(1);
	uthread_create(runFirst, 1);
}

void runFirst() {
	printf("Running in runFirst right now\n");
	uthread_create(runSecond, 2);
	uthread_yield(3);
	printf("Back in runFirst!\n");
}

void runSecond() {
	printf("Running in runSecond now, hopefully after the switch\n");
	exit(-1);
}