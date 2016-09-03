/*************************************************************************
	> File Name: pipe.c
	> Author: Stone
	> Mail: shenchao5858@163.com 
	> Created Time: Sun 28 Aug 2016 03:36:15 PM CST
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>


int main()
{
	int _pipe[2];
	int ret=pipe(_pipe);
	if(ret<0)
	{
		perror("pipe");
		return 1;
	}
	pid_t id=fork();
	if(id==0)
	{
		close(_pipe[0]);
		int i=0;
		char *arr="hello!!!";
		while(i++<5)
		{
			write(_pipe[1],arr,strlen(arr));
			printf("child is write...\n");
			sleep(1);
		}
	}
	else
	{
		sleep(5);
		close(_pipe[1]);
		int i=0;
		char arr[1024];
		while(i++<5)
		{
			memset(arr,'\0',sizeof(arr));
			ssize_t size=read(_pipe[0],arr,sizeof(arr));
			if(size>0)
				printf("%s\n",arr);
		}
	}
 
	return 0;
}
