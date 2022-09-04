#include<arpa/inet.h>
#include<errno.h>
#include<netinet/in.h>
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
int main(){
    int serv_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
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
    struct sockaddr_storage clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    int clnt_sock= accept(serv_sock,(struct sockaddr *)&clnt_addr,&clnt_addr_size);
    char str[]="Hello World";
    write(clnt_sock,str,sizeof(str));
    close(clnt_sock);
    close(serv_sock);
    return 0;
}
