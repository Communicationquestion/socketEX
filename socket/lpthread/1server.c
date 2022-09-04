#include<arpa/inet.h>
#include<errno.h>
#include<netinet/in.h>
#include<pthread.h>
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
char buffer[BUFSIZ]={0};
void *thread_callback(void *args){
	int clifd= *(int *)args;
	int strLen = read(clifd,buffer,BUFSIZ);
	write(clifd,buffer,strLen);
	close(clifd);
	memset(buffer,0,BUFSIZ);
	return NULL;

}
int main(){	
	int serv_sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	struct sockaddr_in serv_addr;
	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(1234);
	int reuse = 1;
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
	socklen_t clnt_addr_size = sizeof(clnt_addr);
	while(1){
		int clnt_sock = accept(serv_sock,(struct sockaddr *)&clnt_addr,&clnt_addr_size);
		pthread_t t0;
		pthread_create(&t0,NULL,thread_callback,&clnt_sock);

	}
	close(serv_sock);
	return 0 ;
}
