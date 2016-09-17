#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>
#include<stdlib.h>
#define SIZE 64

int MSG[SIZE];
sem_t space;
sem_t msg;

void*customer_run(void*val)
{
	int pos=0;
	while(1)
	{
		sleep(1);
		sem_wait(&msg);
		int msg=MSG[pos];
		pos++;
		pos%=SIZE;
		sem_post(&space);
		printf("customer is Done...%d\n",msg);
	}
}

void*produster_run(void*val)
{
	int pos=0;
	while(1)
	{
		sleep(3);
		sem_wait(&space);
		MSG[pos]=rand()%1000;
		++pos;
		pos%=SIZE;
		sem_post(&msg);
		printf("produst is Done...\n");
	}
}

int main()
{
	pthread_t customer;
	pthread_t produster;

	sem_init(&space,0,SIZE);
	sem_init(&msg,0,0);

	pthread_create(&customer,NULL,customer_run,NULL);
	pthread_create(&produster,NULL,produster_run,NULL);
	

	pthread_join(customer,NULL);
	pthread_join(produster,NULL);
	sem_destroy(&space);
	sem_destroy(&msg);
	return 0;
}
