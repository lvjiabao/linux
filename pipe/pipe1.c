/*************************************************************************
	> File Name: pipe1.c
	> Author: Stone
	> Mail: shenchao5858@163.com 
	> Created Time: Sun 28 Aug 2016 04:22:11 PM CST
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<sys/wait.h>
#include<sys/types.h>
int main()
{
	int _pipe[2];
	int ret =pipe(_pipe);
	if(ret<0)
	{
		perror("pipe");
		return 1;
	}
	pid_t id=fork();
	if(id==0)
	{
		close(_pipe[0]);
		char*arr="hello!!!";
		int i=0;
		while(i<5)
		{
			write(_pipe[1],arr,strlen(arr));
			printf("child is writing...\n");
			sleep(1);
			i++;
		}
		close(_pipe[0]);
	}
	else
	{
		sleep(5);
		close(_pipe[1]);
		char arr[1024];
		int i=0;
		while(i++<100)
		{
			ssize_t size=read(_pipe[0],arr,sizeof(arr));
			arr[size]='\0';
			if(size>0)
				printf("%s\n",arr);
		}
		int status=0;
		int ret=waitpid(id,&status,0);
		if(ret==id)
			printf("wait sucess:%d,%d\n",status&0xFF,(status>>8)&0xFF);
		else
			printf("wait failure:%d,%d\n",status&0xFF,(status>>8)&0xFF);

	}
	return 0;
}
