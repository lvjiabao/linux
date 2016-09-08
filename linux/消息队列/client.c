

#include "comm.h"
 
int main()
{
	int id=get_msg_queue();
	char buf[1024];
	while(1)
	{
		memset(buf,0,sizeof(buf));
		printf("please enter:");
		fflush(stdout);///////因为没有\n，所以会输出到缓冲中去
		ssize_t size=read(0,buf,sizeof(buf));//
		if(size>0)
			buf[size-1]=0;
		send_msg(id,CLIENT_T,buf);
		memset(buf,0,sizeof(buf));
		recv_msg(id,SERVER_T,buf);
		printf("server:%s\n",buf);
	}
	return 0;
}
