#include "comm.h"

int main()
{
	int shm_id=create_shm(4096);
	pid_t id=fork();
	if(id==0)
	{
		int shmid=get_shm(4096);
		char*buf=(char*)at(shmid);
		int i=0;
		while(1)
		{
			buf[i]='A';
			sleep(1);
			i++;
		}
		dt(buf);
		exit(1);
	}
	else
	{
		char*buf=(char*)at(shm_id);
		sleep(1);
		while(1)
		{
			sleep(1);
			printf("%s\n",buf);
		}
		dt(buf);
		waitpid(id,NULL,0);
		destroy_shm(shm_id);
	}
	return 0;
}
