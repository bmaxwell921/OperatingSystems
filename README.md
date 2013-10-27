OperatingSystems - Project 1
================

Included with this file is my implementation of a user threading
library. It provides priority scheduling for user threads based on 
the given priority. The library implements a many to many mapping 
scheme from user thread to kernel thread. See lib-ult.h for documentation on how the library functions work.

To use the library you just need to use:
	#include "lib-ult.c"
Then compile with
	gcc -o <Executable> <YourFile.c> -lpthread
For example, compiling the MainTest.c file can be done like so:
	gcc -o Main MainTest.c -lpthread