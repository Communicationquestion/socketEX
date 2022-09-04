#include<errno.h>
#include<netinet/in.h>
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
#define BUF_SIZE 100
int main(){
	int sock =socket(AF_INET,SOCK_DGRAM,0);
	struct sockaddr_in servAddr;

	memset(&servAddr,0,sizeof(servAddr));
	servAddr.sin_family= AF_INET;
	servAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servAddr.sin_port=htons(1234);
	if(bind(sock,(struct sockaddr *)&servAddr,sizeof(servAddr))==-1){
		printf("error!%s",strerror(errno));
		return -1;
		
	}
	struct sockaddr_in clntAddr;
	socklen_t nSize=sizeof(clntAddr);
	char buffer[BUF_SIZE];
	while(1){
	
	int strLen = recvfrom(sock,buffer,BUF_SIZE,0,(struct sockaddr *)&clntAddr,&nSize);
sendto(sock,buffer,strLen,0,(struct sockaddr *)&clntAddr,nSize);
	}
	close(sock);
	return 0;
		
}
