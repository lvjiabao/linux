/*************************************************************************
	> File Name: client.c
	> Author: Stone
	> Mail: shenchao5858@163.com 
	> Created Time: Thu 01 Sep 2016 04:59:00 PM CST
 ************************************************************************/

#include "comm.h"
 
int main()
{
	int id=get_msg_queue();
	char buf[1024];
	while(1)
	{
		memset(buf,0,sizeof(buf));
		printf("please enter:");
		fflush(stdout);///////
		ssize_t size=read(0,buf,sizeof(buf));
		if(size>0)
			buf[size-1]=0;
		send_msg(id,CLIENT_T,buf);
		memset(buf,0,sizeof(buf));
		recv_msg(id,SERVER_T,buf);
		printf("server:%s\n",buf);
	}
	return 0;
}
