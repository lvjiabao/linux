#include "comm.h"


static int comm_msg_queue(int flag)
{
	key_t key=ftok(_PATH_NAME_,_PROJ_ID_);
	if(key<0)
	{
		perror("ftok");
		return -2;
	}
	int msg_id=msgget(key,flag);
	if(msg_id<0)
	{
		perror("msgget");
	}
	return msg_id;
}


int create_msg_queue()
{
	int flag=IPC_CREAT|IPC_EXCL|0644;
	return comm_msg_queue(flag);
}
int get_msg_queue()
{
	int flag=IPC_CREAT;
	return comm_msg_queue(flag);
}
int recv_msg(int msg_id,int type,char*out)
{
	msg_t msg;
	int ret=msgrcv(msg_id,&msg,sizeof(msg.mtext),type,0);
	if(ret<0)
	{
		perror("msgrcv");
		return -1;
	}
	strcpy(out,msg.mtext);
	return 0;
}
int send_msg(int msg_id,int type,const char*msginfo)
{
	msg_t msg;
	msg.mtype=type;
	strncpy(msg.mtext,msginfo,strlen(msginfo)+1);//////////
	int ret=msgsnd(msg_id,&msg,sizeof(msg.mtext),0);
	if(ret<0)
	{
		perror("msgsnd");
		return -1;
	}
	return 0;
}
int destroy_msg(int msg_id)
{
	if(msgctl(msg_id,IPC_RMID,NULL)<0)
	{
		perror("msgctl");
		return 1;
	}
}

