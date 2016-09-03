/*************************************************************************
	> File Name: wait.c
	> Author: Stone
	> Mail: shenchao5858@163.com 
	> Created Time: Sun 28 Aug 2016 02:45:13 PM CST
 ************************************************************************/

#include<stdio.h>
#include<errno.h>
#include<string.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<stdlib.h>
 
int main()
{
	pid_t id=fork();
	if(id==0)
	{
		printf("is child:%d\n",getpid());
		sleep(10);
		exit(111);
	}
	else
	{
		int status=0;
		pid_t ret= waitpid(-1,&status,0);
		if(ret==id)
		{
			printf("pid:%d,sign:%d,exit:%d\n",id,status&0xFF,((status>>8)&0xFF));
		}
		else
		{
			printf("%d,%s\n",errno,strerror(errno));
		}
	}
	return 0;
}
