#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

typedef struct list
{
	struct list*_next;
	int _val;
}msg_list,*p_list;

p_list head=NULL;
pthread_mutex_t lock;
pthread_cond_t cond;

void init(p_list list)
{
	if(list)
	{
		list->_next=NULL;
		int _val=0;
	}
}

void*produster_run(void*val)
{
	while(1)
	{
		sleep(2);
		p_list p=malloc(sizeof(msg_list));
		pthread_mutex_lock(&lock);
		init(p);
		p->_next=head;
		p->_val=rand()%1000;
		head=p;
		pthread_mutex_unlock(&lock);
		printf("prodect is done...%d\n",p->_val);
		pthread_cond_signal(&cond);
	}
	return NULL;
}

void*customer_run(void*val)
{
	p_list p=NULL;
	while(1)
	{
		pthread_mutex_lock(&lock);
		while(NULL==head)
		{
			pthread_cond_wait(&cond,&lock);
		}
		p=head;
		head=head->_next;
		pthread_mutex_unlock(&lock);
		printf("custome id done...%d\n",p->_val);
		free(p);
		p=NULL;
	}
	return NULL;
}

int main()
{
	pthread_mutex_init(&lock,NULL);
	pthread_cond_init(&cond,NULL);

	pthread_t produster;
	pthread_t customer;
	pthread_create(&produster,NULL,produster_run,NULL);
	pthread_create(&customer,NULL,customer_run,NULL);

	pthread_join(produster,NULL);
	pthread_join(customer,NULL);
	
	pthread_mutex_destroy(&lock);
	pthread_cond_destroy(&cond);
	return 0;
}
