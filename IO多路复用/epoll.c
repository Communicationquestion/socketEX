#include<arpa/inet.h>
#include<errno.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/epoll.h>
#include<unistd.h>

#define MAXEVENTS 100

int initserver(int port);

int main(int argc ,char *argv[]){


	if(argc != 2){
		printf("usage:./tcpepoll port\n");
		return -1;
	}

	int listensock = initserver(atoi(argv[1]));

	printf("listensock=%d\n, listensock");


	if(listensock < 0){

		printf("initserver() failed.\n");

		return -1;
	}

	int epollfd;

	char buffer[1024];

	memset(buffer,0,sizeof(buffer));
	
	epollfd = epoll_create(1024);

	struct epoll_event ev;

	ev.data.fd = listensock;

	ev.events = EPOLLIN;

	epoll_ctl(epollfd,EPOLL_CTL_ADD,listensock,&ev);
	

	while(1){

		struct epoll_event events[MAXEVENTS];

		int infds = epoll_wait(epollfd,events,MAXEVENTS,-1);


		if(infds < 0){

			printf("epoll_wait() failed.\n");

			perror("epoll_wait()");

			break;

		}


		if(infds == 0){

			printf("epoll_wait() timeout.\n");

			continue;
		}


		for(int ii = 0; ii < infds; ii++){


			if((events[ii].data.fd == listensock) && (events[ii].events & EPOLLIN)){

				struct sockaddr_in client;
	
				socklen_t len = sizeof(client);
		
				int clientsock = accept(listensock,(struct sockaddr *)&client,&len);

			
				if(clientsock < 0){
					printf("accept() failed.\n");
					continue;
				}
					
				memset(&ev, 0, sizeof(struct epoll_event));
	
				ev.data.fd = clientsock;

				ev.events = EPOLLIN;
		
				epoll_ctl(epollfd, EPOLL_CTL_ADD, clientsock, &ev);
			
				printf("client(socket=%d) connected ok.\n", clientsock);
			
				continue;
	

			}else if(events[ii].events & EPOLLIN){

				char buffer[1024];

				memset(buffer, 0, sizeof(buffer));
		
				ssize_t isize = read(events[ii].data.fd, buffer, sizeof(buffer));

				
				if(isize <= 0){
					printf("client(eventfd=%d) disconnected.\n", events[ii].data.fd);
					
					memset(&ev, 0, sizeof(struct epoll_event));

					ev.events = EPOLLIN;

					ev.data.fd = events[ii].data.fd;

					epoll_ctl(epollfd, EPOLL_CTL_DEL, events[ii].data.fd, &ev);
					
					close(events[ii].data.fd);
					
					continue;	
				}
				printf("recv(eventfd=%d, size=%zd):%s\n", events[ii].data.fd, isize, buffer);

				write(events[ii].data.fd, buffer, strlen(buffer));


			}
		}

	}

	close(epollfd);
	
	return 0;
}

int initserver(int port){

	int sock = socket(AF_INET, SOCK_STREAM, 0);

	if(sock < 0){
	
		printf("socket() failed.\n");
		
		return -1;
	}
	
	int opt = 1;
	
	unsigned int len = sizeof(opt);
	
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, len);

	struct sockaddr_in servaddr;

	servaddr.sin_family = AF_INET;
	
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	servaddr.sin_port = htons(port);


	if(bind(sock,(struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
		
		printf("bind() failed.\n");
		
		close(sock);
		
		return -1;
	}
	

	if(listen(sock, 5) != 0){
		printf("listen() failed.\n");

		close(sock);

		return -1;
	}

	return sock;

}
