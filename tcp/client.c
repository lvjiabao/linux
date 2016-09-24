#include<stdio.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>


#define IP "127.0.0.1"
#define PORT 8080


int main()
{
	int sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock<0)
	{
		perror("sock");
		close(sock);
		return 1;
	}
	struct sockaddr_in peer;
	peer.sin_family=AF_INET;
	peer.sin_port=htons(PORT);
	peer.sin_addr.s_addr=inet_addr(IP);

	char buf[1024];

	if(connect(sock,(struct sockaddr*)&peer,sizeof(peer))<0)
	{
		perror("connect");
		close(sock);
		return 2;
	}
	while(1)
	{
		ssize_t sz=read(0,buf,sizeof(buf));
		if(sz>0)
		{
			buf[sz-1]=0;
			write(sock,buf,strlen(buf));
		}
		else
		{
			perror("read");
			break;
		}
	}
	close(sock);
	return 0;
}
