#include "lib-ult.h"
#include <stdio.h>

void func();

int main(int argc, char** argv) {
	system_init(1);
	uthread_create(func, 1);
}

void func() {
	printf("Success!");
}