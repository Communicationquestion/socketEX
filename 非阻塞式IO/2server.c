#include<arpa/inet.h>
#include<asm-generic/errno.h>
#include<errno.h>
#include<fcntl.h>
#include<netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
int main(){
	int serv_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	fcntl(serv_sock,F_SETFL,fcntl(serv_sock,F_GETFL,0)|O_NONBLOCK);
	struct sockaddr_in serv_addr;
	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family= AF_INET;
	serv_addr.sin_addr.s_addr= inet_addr("127.0.0.1");
	serv_addr.sin_port=htons(1234);
	int reuse =1;
	if(setsockopt(serv_sock,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int ))==-1){
		printf("error!%s",strerror(errno));
		return -1;
	}
	if(bind(serv_sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr))==-1){
		printf("error!%ws",strerror(errno));
		return -1;


	
	
	}
	listen(serv_sock,20);
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size=sizeof(clnt_addr);
	char buffer[BUFSIZ]={0};
	int round = 0;
	typedef struct readNode{
		int fd;
		struct readNode *next;


	}readNode;
	readNode *readList=(readNode *)malloc(sizeof(readNode));
	readNode *currentNode;
	currentNode=readList;
	readNode *pre = readList;
	readNode *worker = pre->next;	
	while(1){
		errno=0;
		round++;
		int clnt_sock = accept(serv_sock,(struct sockaddr *)&clnt_addr,&clnt_addr_size);
		printf("%d\n",clnt_sock);
		if(clnt_sock==-1){
			fprintf(stderr,"Round %d\n",round);
			fprintf(stderr,"You are not be blocked.\n");
			fprintf(stderr,"errno=%d,err:\n",errno, strerror(errno));
			close(clnt_sock);
			sleep(1);
		}
		if(clnt_sock>0){
		
		
			puts("clnt_sok received");
			fcntl(clnt_sock ,F_SETFL,fcntl(clnt_sock,F_GETFL,0)|O_NONBLOCK);
			readNode *newNode=(readNode *)malloc(sizeof(readNode));
			newNode->fd=clnt_sock;
			newNode->next=NULL;
			currentNode->next=newNode;
			currentNode=newNode;

		}
		pre=readList;
		worker=pre->next;
		for(;pre->next!=NULL;pre=pre->next,worker=pre->next){
			errno = 0;
			int strLen=read(worker->fd,buffer,BUFSIZ);
			printf("error code: %d\n",errno);
			if(errno==EWOULDBLOCK){
			continue;
			}else{
			
			
				int writeLen=write(worker->fd,buffer,strLen);
				close(worker->fd);
				printf("client(eventfd=%ddisconnected.\n)",worker->fd);
				memset(buffer,0,BUFSIZ);
				if(worker->next){
					pre->next=worker->next;
					free(worker);
					worker = pre->next;
				}else{
				
				
					pre->next=NULL;
					free(worker);
					worker=pre->next;
					currentNode=pre;
					break;
				}
			}
		}
	}
close(serv_sock);
return 0;
}
