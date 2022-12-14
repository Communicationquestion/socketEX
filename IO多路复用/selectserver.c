#include<arpa/inet.h>
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/select.h>
int initserver(int port);
int main(int argc,char *argv[]){


	if(argc !=2){
		printf("usage:./tcpselect port\n");
		return -1;
	}
	int listensock=initserver(atoi(argv[1]));
	printf("listensock=%d\n",listensock);
	if(listensock<0){
	
		printf("initserver() failed.\n");
		return -1;

	}

	fd_set readfdset;
	int maxfd;
	FD_ZERO(&readfdset);
	FD_SET(listensock, &readfdset);
	maxfd=listensock;
	
	while(1){
		fd_set tmpfdset = readfdset;
		int infds=select(maxfd+1,&tmpfdset,NULL,NULL,NULL);
		printf("select infds=%d\n", infds);
		if(infds<0){
			printf("select() failed.\n");
			perror("select()");
			break;
		}
		if(infds==0){
			printf("select() timeout.\n");
			continue;
		}
		for(int eventfd=0;eventfd<=maxfd;eventfd++){
			if(FD_ISSET(eventfd,&tmpfdset)<=0)
				continue;
			
		
		if(eventfd == listensock){
			struct sockaddr_in client;
			socklen_t len=sizeof(client);
			int clientsock = accept(listensock,(struct sockaddr *)&client,&len);
			if(clientsock<0){
				printf("accept() failed.\n");
				continue;
			}
			printf("client(socket=%d) connected ok.\n",clientsock);
			FD_SET(clientsock,&readfdset);
			if(maxfd<clientsock)
				maxfd = clientsock;
			continue;
		}else{
		char buffer[1024];
		memset(buffer,0,sizeof(buffer));
		ssize_t isize=read(eventfd,buffer,sizeof(buffer));
		if(isize<=0){
			printf("client(eventfd=%d) disconnected.\n",eventfd);
			close(eventfd);
			FD_CLR(eventfd,&readfdset);
			if(eventfd==maxfd){
				for(int ii=maxfd;ii>0;ii--){
					if(FD_ISSET(ii,&readfdset)){
						maxfd=ii;
						break;
					}
				}
				printf("maxfd=%d\n",maxfd);
			}
			continue;
		}
		printf("recv(eventfd=%d,size=%zd):%s\n",eventfd,isize,buffer);
		write(eventfd,buffer,strlen(buffer));
		}

		}
	}
	return 0;

}
int initserver(int port){
	int sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock<0){
		printf("socket() failed.\n");
		return -1;

	}
	int opt = 1;
	unsigned int len=sizeof(opt);
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,len);
	struct sockaddr_in servaddr;
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_port=htons(port);
	if(bind(sock,(struct sockaddr *)&servaddr,sizeof(servaddr))<0){
		printf("bind() failed.\n");
		close(sock);
		return -1;
	}
	if(listen(sock,5)!=0){
		printf("listen() failed.\n");
		close(sock);
		return -1;
	}
	return sock;
}


