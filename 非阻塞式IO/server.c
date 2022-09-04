#include<arpa/inet.h>
#include<errno.h>
#include<fcntl.h>
#include<netinet/in.h>
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
int main(){
	int serv_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	fcntl(serv_sock,F_SETFL,fcntl(serv_sock,F_GETFL,0)|O_NONBLOCK);
	struct sockaddr_in serv_addr;
	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	serv_addr.sin_port=htons(1234);
	int reuse=1;
	if(setsockopt(serv_sock,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int))==-1){
	
	
		printf("error!%s",strerror(errno));
		return -1;
	}
	if(bind(serv_sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr))==-1){
		printf("error!%s",strerror(errno));
		return -1;
	}
	listen(serv_sock,20);
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size=sizeof(clnt_addr);
	char buffer[BUFSIZ]={0};
	int round = 0;
	while(1){
		errno =0;
		round++;
		int clnt_sock = accept(serv_sock,(struct sockaddr *)&clnt_addr,&clnt_addr_size);
		printf("%d\n",clnt_sock);
		if(clnt_sock==-1){
			fprintf(stderr,"Round %d\n",round);
			fprintf(stderr,"You are not be blocked.\n");
			fprintf(stderr,"errno = %d,err:%s\n",errno,strerror(errno));
			close(clnt_sock);
			sleep(1);
			continue;
		}
		fcntl(clnt_sock,F_SETFL,fcntl(clnt_sock,F_GETFL,0)|O_NONBLOCK);
		int strLen = read(clnt_sock,buffer,BUFSIZ);
		while(strLen==-1){
			printf("Our server is none blocked ,You cna do something when waiting for ""client data.\n");
			strLen=read(clnt_sock,buffer,BUFSIZ);
			sleep(1);
		}
		int writeLen=write(clnt_sock,buffer,strLen);
		while(writeLen==-1){
			printf("Our server is none blocked,You can do something when waiting for""client data.\n");
			strLen=write(clnt_sock,buffer,strLen);
			sleep(1);
		}
		close(clnt_sock);
		memset(buffer,0,BUFSIZ);

	}
	close(serv_sock);
	return 0;

}
