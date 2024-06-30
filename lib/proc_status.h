#ifndef _PROC_STATUS_H_
#define _PROC_STATUS_H_

#define PATH_LEN 2048
#define BUFF_LEN 2048
#define KEY_LEN 64
#define VALUE_LEN 512

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

int open_status(char* pid);
void close_status();

#endif