#include<stdio.h>
#include<errno.h>
#include<sys/epoll.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

static void Usage(const char*proc)
{
	assert(proc);
	printf("Usage:%s [ip] [prot]\n",proc);
}

static int set_nonblock(int fd)
{
	int fl=fcntl(fd,F_GETFL);
	fcntl(fd,F_SETFL,fl|O_NONBLOCK);
}

int my_read(int fd,char*buf,int len)
{
	assert(buf);
	ssize_t total=0;
	ssize_t s=0;
	while((s=read(fd,buf+total,len-1-total)>0)&&errno!=EAGAIN)
	{
		total+=s;
	}
	return total;
}

int get_sock(const char*ip,short port)
{
	assert(ip);
	assert(port>0);
	int sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock<0)
	{
		perror("socket");
		return 1;
	}

	struct sockaddr_in local;
	local.sin_family=AF_INET;
	local.sin_port=htons(port);
	local.sin_addr.s_addr=inet_addr(ip);

	if(bind(sock,(struct sockaddr*)&local,sizeof(local))<0)
	{
		perror("bind");
		return 2;
	}

	if(listen(sock,5)<0)
	{
		perror("listen");
		return 3;
	}
	return sock;
}

int main(int argc,char*argv[])
{
	if(3!=argc)
	{
		Usage(argv[0]);
		return 4;
	}
	int listen_socket=get_sock(argv[1],atoi(argv[2]));
	int epfd=epoll_create(256);
	if(epfd<0)
	{
		perror("epoll_create");
		return 5;
	}

	struct epoll_event ev;
	ev.events=EPOLLIN;
	ev.data.fd=listen_socket;
	epoll_ctl(epfd,EPOLL_CTL_ADD,listen_socket,&ev);
	int nums=0;

	struct epoll_event ready_events[64];
	int len=64;
	int timeout=-1;

	while(1)
	{
		switch((nums=epoll_wait(epfd,ready_events,len,timeout)))
		{
			case 0:
				printf("timeout...\n");
				break;
			case -1:
				perror("epoll_wait");
				break;
			default:
				{
					int i=0;
					for(;i<nums;i++)
					{
						int fd=ready_events[i].data.fd;
						if(fd==listen_socket&&(ready_events[i].events&EPOLLIN))
						{
							struct sockaddr_in client;
							socklen_t len=sizeof(client);
							int new_fd=accept(listen_socket,(struct sockaddr*)&client,&len);
							if(new_fd<0)
							{
								perror("accept");
								continue;
							}
							printf("get anew client...%s %d\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));
							ev.events=EPOLLIN|EPOLLET;//read
							ev.data.fd=new_fd;
							set_nonblock(new_fd);
							epoll_ctl(epfd,EPOLL_CTL_ADD,new_fd,&ev);
						}
						else//normal I/O
						{
							if(ready_events[i].events&EPOLLIN)
							{
								char buf[1024];
								//my_read
								ssize_t s=read(fd,buf,sizeof(buf)-1);
								if(s>0)
								{
									buf[s]=0;
									printf("client #:%s\n",buf);
									
									ev.events=EPOLLOUT|EPOLLET;
									ev.data.fd=fd;
									epoll_ctl(epfd,EPOLL_CTL_MOD,fd,&ev);
								}
								else if(s==0)
								{
									epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
									close(fd);
									printf("client close...\n");
								}
								else
								{
									perror("read");
								}
							}
							else if(ready_events[i].events&EPOLLOUT)
							{
								char buf[1024];
								sprintf(buf,"HTTP/1.0 200 OK\r\n\r\n<html><h1>hello world</h1></html>");
								//my_write
								write(fd,buf,strlen(buf));
								epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
								close(fd);
							}
							else
							{}
						}
					}
				}
				break;
		}
	}
	return 0;
}
