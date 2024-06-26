#include <sched.h>
#include <stdio.h>
#include <unistd.h>

#define STACK_SIZE 10000
char child_stack[STACK_SIZE + 1];


