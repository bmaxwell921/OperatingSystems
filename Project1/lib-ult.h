/*
* Header file for the Threading Library
*/

#ifndef LIB_ULT_H_INCLUDED
#define LIB_ULT_H_INCLUDED

/*
* Function called before any other uthread library functions can be called.
* Initializes any necessary data structures and other necessary data.
* @Param max_number_of_klt
*						The maximum number of kernal threads that can be used to run
*						the user level threads created by this library. Can be no less than 1.
*/
void system_init(int max_number_of_klt);

/*
* Creates a new user-level thread which runs func() without any argument.
* @Param pri_num
*				The priority number with the associated thread
* @Return 
		0 if the function succeeds, -1 otherwise
*/
int uthread_create(void func(), int pri_num);

/*
* Calling thread requests to yield the kernal thread that is currently running,
* and its priority number is changed to the given number. Scheduler needs to decide
* which thread should be run next on the yielded kernal thread. 
* @Param pri_num
*				The new priority for the calling thread
* @Return
		0 unless something wrong occurs 
*/
int uthread_yield(int pri_num);

/*
* Calling thread ends its execution. Scheduler should pick one of the ready threads
* to run on the kernal thread that used to be occupied by the calling thread. If there
* are no other user threads ready to run, the kernal thread should exit.
*/
void uthread_exit();

#endif