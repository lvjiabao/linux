#include<stdio.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>
#define _PATH_NAME_ "/tmp"
#define _PROJ_ID_ 0x6666


int create_shm(int size);
int get_shm(int size);
void* at(int shm_id);
int dt(char*buf);
int destroy_shm(int shm_id);
