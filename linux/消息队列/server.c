/*************************************************************************
	> File Name: server.c
	> Author: Stone
	> Mail: shenchao5858@163.com 
	> Created Time: Thu 01 Sep 2016 05:15:35 PM CST
 ************************************************************************/

#include "comm.h"
 
int main()
{
	int id=create_msg_queue();
	char buf[1024];
	while(1)
	{
		memset(buf,0,sizeof(buf));
		recv_msg(id,CLIENT_T,buf);
		printf("client:%s\n",buf);
	
		memset(buf,0,sizeof(buf));
		printf("please enter:");
		fflush(stdout);///////
		ssize_t size=read(0,buf,sizeof(buf));
		if(size>0)
			buf[size-1]=0;	
		send_msg(id,SERVER_T,buf);
	}
	destroy_msg(id);
	return 0;
}
