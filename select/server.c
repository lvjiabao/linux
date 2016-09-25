#include<stdio.h>
#include<assert.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<errno.h>
#include<arpa/inet.h>
#include<stdlib.h>

int rfds[128];
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
		close(sock);
		return 2;
	}

	if(listen(sock,5)<0)
	{
		perror("listen");
		close(sock);
		return 3;
	}
	return sock;
}

void usage(const char*proc)
{
	assert(proc);
	printf("Usage:%s:[ip] [port]\n",proc);
}

int main(int argc,char*argv[])
{
	if(3!=argc)
	{
		usage(argv[0]);
		return 4;
	}
	int listen_sock=get_sock(argv[1],atoi(argv[2]));
	int i=0;
	for(;i<128;i++)
	{
		rfds[i]=-1;
	}
	fd_set rset;
	int max_fd=0;
	while(1)
	{
		struct timeval timeout={0,0};
		FD_ZERO(&rset);

		rfds[0]=listen_sock;
		max_fd=listen_sock;

		for(i=0;i<128;i++)
		{
			if(rfds[i]>=0)
			{
				FD_SET(rfds[i],&rset);

				if(max_fd<rfds[i])
				{
					max_fd=rfds[i];
				}
			}
		}
		switch( select(max_fd+1,&rset,NULL,NULL,NULL) )
		{
			case -1:
				perror("select");
				break;
			case 0:
				printf("timeout...\n");
			default:
				{
					int j=0;
					for(;j<128;j++)
					{
						if(rfds[j]<0)
						{
							continue;
						}
						if(j==0&&FD_ISSET(rfds[j],&rset))
						{
							struct sockaddr_in client;
							socklen_t len=sizeof(client);
							int new_fd=accept(listen_sock,(struct sockaddr*)&client,&len);
							if(new_fd<0)
							{
								perror("accept");
							}
							else
							{
								printf("get a client:socket:%s:%d\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));
								int k=0;
								for(;k<128;k++)
								{
									if(rfds[k]==-1)
									{
										rfds[k]=new_fd;
										break;
									}
								}
								if(k==128)
								{
									close(new_fd);
								}
							}
						}
						else if( FD_ISSET(rfds[j],&rset))
						{
							//normal read
							char buf[1024];
							ssize_t s=read(rfds[j],buf,sizeof(buf)-1);
							if(s>0)
							{
								buf[s]=0;
								printf("client#%s\n",buf);
							}
							else if(s==0)
							{
								printf("client close...\n");
								close(rfds[j]);
								rfds[j]=-1;
							}
							else
							{
								perror("read");
							}
						}
						else
						{

						}
					}
				}
				break;
		}
	}



	return 0;
}
