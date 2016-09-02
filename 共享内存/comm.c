#include "comm.h"


static int comm_create(int flag,int size)
{
	key_t key=ftok(_PATH_NAME_,_PROJ_ID_);
	if(key<0)
	{
		perror("ftok");
		return -2;
	}
	int shm_id=shmget(key,size,flag);
	if(shm_id<0)
	{
		perror("shmget");
	}
	return shm_id;
}

int create_shm(int size)
{
	int flag=IPC_CREAT|IPC_EXCL|0644;
	return comm_create(flag,size);
}
int get_shm(int size)
{
	int flag=IPC_CREAT;
	return comm_create(flag,size);
}
void* at(int shm_id)
{
	return shmat(shm_id,NULL,0);//可以把它作为malloc来理解
}
int dt(char*buf)
{
	return shmdt(buf);
}
int destroy_shm(int shm_id)
{
	if(shmctl(shm_id,IPC_RMID,NULL)<0)
	{
		perror("shmctl");
		return -1;
	}
	return 0;
}
