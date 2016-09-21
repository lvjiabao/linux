
#include<stdio.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>


int main()
{
	int ret=mkfifo("./.log",0664);
	if(ret<0)
	{
		perror("mkfifo");
		return 1;
	}
	int fd=open("./.log",O_WRONLY);
	if(fd<0)
	{
		perror("open");
		return 2;
	}
	char buf[1024];
	while(1)
	{
		scanf("%s",buf);
		ssize_t size=write(fd,buf,strlen(buf));
		if(size<=0)
		{
			printf("write error or write end\n");
			break;
		}
		if(0==strncmp(buf,"quit",4))
		{
			break;
		}
	}
	close(fd);
	return 0;
}
