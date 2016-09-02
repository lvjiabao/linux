#include"comm.h"



static int comm_sem(int flag,int semset_num)
{
	key_t key=ftok(_PATH_NAME_,_PROJ_ID_);
	if(key<0)
	{
		perror("ftok");
		return -2;
	}
	int sem_id=semget(key,semset_num,flag);
	//创建的是信号量集，即一个数组，semset_num表示数组的大小
	if(sem_id<0)
	{
		perror("semget");
	}
	return sem_id;

}

int create_sem(int semset_num)
{
	int flag=IPC_EXCL|IPC_CREAT|0644;
	return comm_sem(flag,semset_num);
}
int get_sem()
{
	int flag=IPC_CREAT;
	return comm_sem(flag,0);//获取一个已创建的信号量的时候semset_num=0
}
int init(int sem_id,int which)
{
	union semun _semun;
	_semun.val=1;//STEVAL用来把信号量初始化ihuawei已知的值，val即为SETVAL的值
	int ret=semctl(sem_id,which,SETVAL,_semun);//改变信号量
	if(ret<0)
	{
		perror("semctl");
	}
	return ret;
}

static int op_sem(int sem_id,int op,int which)//which为数组下标
{
	struct sembuf sem;
	memset(&sem,0,sizeof(sem));
	sem.sem_num=which;
	sem.sem_op=op;
	sem.sem_flg=0;//sem_undo;
	return semop(sem_id,&sem,1);//改变信号量的值
}
int P(int sem_id,int which)
{
	return op_sem(sem_id,-1,which);
}
int V(int sem_id,int which)
{
	return op_sem(sem_id,1,which);
}
int destroy_sem(int sem_id)
{
	if(semctl(sem_id,0,IPC_RMID,NULL)<0)
		perror("semctl");
	return -1;
}

