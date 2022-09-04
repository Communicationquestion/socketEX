#include<arpa/inet.h>
#include<netinet/in.h>
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
#define BUF_SIZE 100
int main(){

	int sock = socket(AF_INET,SOCK_DGRAM,0);
	struct sockaddr_in servAddr;
	memset(&servAddr,0,sizeof(servAddr));
	servAddr.sin_family=PF_INET;
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAddr.sin_port=htons(1234);
	struct sockaddr fromAddr;
	socklen_t addrLen =sizeof(fromAddr);
	while(1){
	char buffer[BUF_SIZE]={0};
	printf("Input a string:");
	fgets(buffer,BUF_SIZE,stdin);
	sendto(sock,buffer,strlen(buffer),0,(struct sockaddr *)&servAddr, sizeof(servAddr));
	int strLen= recvfrom(sock,buffer,BUF_SIZE,0,&fromAddr,&addrLen);
	buffer[strLen]=0;
	printf("Message from server: %s\n",buffer);

	}
	close(sock);
	return 0;
}
