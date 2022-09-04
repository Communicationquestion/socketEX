#include<arpa/inet.h>
#include<netinet/in.h>
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<errno.h>
#include<unistd.h>
int main(){
    int sock= socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in serv_addr;
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family =AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    serv_addr.sin_port= htons(1234);
    if(connect(sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr))==-1);
    char buffer[40];
    read(sock,buffer,sizeof(buffer)-1);
    printf("Message form server:%s\n",buffer);
    close(sock);
    return 0;
}
