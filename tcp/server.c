#include<stdio.h>
#include<assert.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>

int get_sock(const char*ip,short port)
{
	assert(ip);
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

void showmsg(struct sockaddr_in remote)
{
	printf("client connecting... %s %d\n",inet_ntoa(remote.sin_addr),ntohs(remote.sin_port));
}

int main(int argc,char*argv[])
{
	if(argc!=3)
	{
		printf("%s [ip] [port]\n",argv[0]);
		return 4;
	}

	int listen_socket=get_sock(argv[1],atoi(argv[2]));
	
	struct sockaddr_in remote;
	socklen_t len=sizeof(remote); 
	char buf[1024];
	while(1)
	{
		int new_fd=accept(listen_socket,(struct sockaddr*)&remote,&len);
		if(new_fd<0)
		{
			perror("accept");
			continue;	
		}
		showmsg(remote);
		while(1)
		{
			ssize_t sz=read(new_fd,buf,sizeof(buf));
			if(sz>0)
			{
				buf[sz]=0;
				printf("client #:%s\n",buf);
			}
			else if(sz==0)
			{
				printf("client close...\n");
				break;
			}
			else
			{
				perror("read");
				break;
			}
		}
	}
	close(listen_socket);
	return 0;
}













