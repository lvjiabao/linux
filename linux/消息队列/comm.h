#pragma once
#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<string.h>
#include<errno.h>

#define _PATH_NAME_ "/tmp"
#define _PROJ_ID_ 0x6666

#define SERVER_T 1
#define CLIENT_T 2

typedef struct msgbug
{
	long mtype;
	char mtext[1024];
}msg_t;

int create_msg_queue();
int get_msg_queue();
int recv_msg(int msg_id,int type ,char*out);
int send_msg(int msg_id,int type,const char*msg);
int destroy_msg(int msg_id);

