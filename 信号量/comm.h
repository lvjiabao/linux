#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/sem.h>
#include<sys/ipc.h>
#include<string.h>
#include<errno.h>

union semun
{
	int val;
	struct semid_ds*buf;
	unsigned short*array;
	struct seminfo* _buf;
};


#define _PATH_NAME_ "/tmp"
#define _PROJ_ID_ 0x6666

int create_sem(int semset_num);
int get_sem();
int init(int sem_id,int which);
int P(int sem_id,int which);
int V(int sem_id,int which);
int destroy_sem(int sem_id);

