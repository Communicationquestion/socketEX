#include<arpa/inet.h>
#include<netinet/in.h>
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
#define BUF_SIZE 1024
int main(){
	char *filename= "./send.file";
	FILE *fp = fopen(filename,"r");
	if(fp==NULL){
		printf("Cannot open file,press any key to exit!/n");
		return -1;

	}
	int servSock = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in sockAddr;
	memset(&sockAddr,0,sizeof(sockAddr));
	sockAddr.sin_family=PF_INET;
	sockAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	sockAddr.sin_port=htons(1234);

	if(bind(servSock,(struct sockaddr *)&sockAddr,sizeof(sockAddr))==-1){
	
		return -1;
	}
	listen(servSock,20);
	struct sockaddr_in clntAddr;
	socklen_t nSize=sizeof(clntAddr);
	int clntSock =accept(servSock,(struct sockaddr *)&clntAddr,&nSize);
	char buffer[BUF_SIZE]={0};
	int nCount;
	while((nCount=fread(buffer,1,BUF_SIZE,fp))>0){
		send(clntSock,buffer,nCount,0);
	}
	shutdown(clntSock,SHUT_WR);
	recv(clntSock,buffer,BUF_SIZE,0);
	fclose(fp);
	close(clntSock);
	close(servSock);
	return 0;

	
}
