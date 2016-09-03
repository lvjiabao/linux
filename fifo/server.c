/*************************************************************************
	> File Name: server.c
	> Author: Stone
	> Mail: shenchao5858@163.com 
	> Created Time: Sun 28 Aug 2016 05:52:28 PM CST
 ************************************************************************/

#include<stdio.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
 
int main()
{
	int fd=open("./.log",O_RDONLY);
	if(fd<0)
	{
		perror("open");
		return 1;
	}
	char arr[1024];
	while(1)
	{
		ssize_t size=read(fd,arr,sizeof(arr));
		if(size<=0)
		{
			printf("read error or end\n");
			break;
		}
		arr[size]='\0';
		printf("%s\n",arr);
		if(0==strncmp(arr,"quit",4))
			break;
	}
	close(fd);

	return 0;
}
