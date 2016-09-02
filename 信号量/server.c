#include"comm.h"


int main()
{
	int sem_id=create_sem(1);
	init(sem_id,0);
	pid_t id=fork();
	if(id==0)
	{
		while(1)
		{
			P(sem_id,0);
			printf("A");
			fflush(stdout);
			usleep(100000);
			printf("A");
			fflush(stdout);
			usleep(100000);
			V(sem_id,0);
		}
		exit(1);
	}
	else
	{
		while(1)
		{
			P(sem_id,0);
			printf("B");
			fflush(stdout);
			usleep(100000);
			printf("B");
			fflush(stdout);
			usleep(100000);
			V(sem_id,0);
		}
		waitpid(id,NULL,0);
		destroy_sem(sem_id);
	}
	return 0;
}
