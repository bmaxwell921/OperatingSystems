#include "lib-ult.c"
#include <stdio.h>
#include <stdlib.h>

void runFirst();
void runSecond();
void runThird();

int main(int argc, char** argv) {
	system_init(0	);
	uthread_create(runFirst, 1);
	uthread_create(runSecond, 2);
	uthread_create(runThird, 2);
}







/* Priority 1 -> 3 */
void runFirst() {
	printf("Starting runFirst now\n");

	printf("Finishing runFirst\n");
	uthread_exit();
}

/* Priority 2 */
void runSecond() {
	printf("Starting runSecond now.\n");


	printf("Finishing runSecond\n");
	uthread_exit();
}

/* Priority 4 */
void runThird() {
	printf("Starting runThird now.\n");

	printf("Finishing runThird\n");
	uthread_exit();
}